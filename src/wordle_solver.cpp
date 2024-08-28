#include "wordle_solver.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <numeric>

// Load words from a file
std::vector<std::string> loadWords(const std::string& filename) {
    std::vector<std::string> words;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            words.push_back(line);
        }
    }
    return words;
}

// Filter words based on feedback
std::vector<std::string> filterWordsByFeedback(const std::vector<std::string>& words, const std::string& guess, const std::string& feedback) {
    // Filter based on the feedback provided
    auto letterCounts = getLetterCountsFromGuesses({{guess, feedback}});
    auto greenLetters = getGreenLettersFromGuesses({{guess, feedback}});
    auto [excludedLetters, exactCounts, positionRestrictions] = getRestrictionsFromGuesses({{guess, feedback}});

    std::vector<std::string> filteredWords = words;
    filteredWords = filterWordsByExclusion(filteredWords, excludedLetters);
    filteredWords = filterWordsByPosition(filteredWords, greenLetters);
    filteredWords = filterWordsByExactCount(filteredWords, exactCounts);
    filteredWords = filterWordsByExclusionAt(filteredWords, positionRestrictions);

    return filteredWords;
}

// Function to find the best guesses based on remaining words
std::vector<std::string> findBestGuesses(const std::vector<std::string>& remainingWords, int topN) {
    auto frequencies = calculateFrequencies(remainingWords);
    auto expectedWords = expectedRemainingWords(remainingWords, frequencies);
    std::vector<std::string> sortedWords(remainingWords);
    std::sort(sortedWords.begin(), sortedWords.end(), [&expectedWords](const std::string& a, const std::string& b) {
        return expectedWords.at(a) < expectedWords.at(b);
    });
    if (sortedWords.size() > topN) {
        sortedWords.resize(topN);
    }
    return sortedWords;
}

// Calculate the frequencies of words
std::unordered_map<std::string, double> calculateFrequencies(const std::vector<std::string>& words) {
    std::unordered_map<std::string, double> frequencies;
    double totalFrequency = 0.0;
    for (const auto& word : words) {
        double freq = 1.0; // Placeholder for actual frequency calculation
        frequencies[word] = freq;
        totalFrequency += freq;
    }
    for (auto& [word, freq] : frequencies) {
        freq /= totalFrequency;
    }
    return frequencies;
}

// Expected remaining words based on guesses
std::unordered_map<std::string, double> expectedRemainingWords(const std::vector<std::string>& words, const std::unordered_map<std::string, double>& frequencies) {
    std::unordered_map<std::string, double> expectedCounts;
    for (const auto& guess : words) {
        double totalCount = 0.0;
        for (const auto& answer : words) {
            double probability = frequencies.at(answer);
            int remainingCount = countRemainingWordsAfterGuess(guess, answer, words);
            totalCount += remainingCount * probability;
        }
        expectedCounts[guess] = totalCount;
    }
    return expectedCounts;
}

// Count remaining words after a guess
int countRemainingWordsAfterGuess(const std::string& guess, const std::string& answer, const std::vector<std::string>& words) {
    std::string feedback = generateFeedback(guess, answer);
    std::vector<std::string> filteredWords = filterWordsByFeedback(words, guess, feedback);
    return filteredWords.size();
}

// Generate feedback based on guess and answer
std::string generateFeedback(const std::string& guess, const std::string& answer) {
    std::string feedback(guess.size(), 'b');
    for (size_t i = 0; i < guess.size(); ++i) {
        if (guess[i] == answer[i]) {
            feedback[i] = 'g';
        }
    }
    std::unordered_map<size_t, char> filteredGuess;
    std::vector<char> filteredAnswer;
    for (size_t i = 0; i < guess.size(); ++i) {
        if (feedback[i] != 'g') {
            filteredGuess[i] = guess[i];
            if (feedback[i] != 'y') {
                filteredAnswer.push_back(answer[i]);
            }
        }
    }
    for (size_t i = 0; i < guess.size(); ++i) {
        if (feedback[i] == 'b') {
            char c = filteredGuess[i];
            auto it = std::find(filteredAnswer.begin(), filteredAnswer.end(), c);
            if (it != filteredAnswer.end()) {
                feedback[i] = 'y';
                filteredAnswer.erase(it);
            }
        }
    }
    return feedback;
}
