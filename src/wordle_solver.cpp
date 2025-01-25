#include "wordle_solver.hpp"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <numeric>

// Helper function implementations
std::unordered_map<char, int> getLetterCountsFromGuesses(const std::vector<std::pair<std::string, std::string>>& guesses) {
    std::unordered_map<char, int> counts;
    for (const auto& [guess, feedback] : guesses) {
        for (size_t i = 0; i < guess.length(); ++i) {
            if (feedback[i] != 'b') {
                counts[guess[i]]++;
            }
        }
    }
    return counts;
}

std::unordered_map<int, char> getGreenLettersFromGuesses(const std::vector<std::pair<std::string, std::string>>& guesses) {
    std::unordered_map<int, char> greenLetters;
    for (const auto& [guess, feedback] : guesses) {
        for (size_t i = 0; i < guess.length(); ++i) {
            if (feedback[i] == 'g') {
                greenLetters[i] = guess[i];
            }
        }
    }
    return greenLetters;
}

std::tuple<std::vector<char>, std::unordered_map<char, int>, std::vector<std::pair<int, char>>> 
getRestrictionsFromGuesses(const std::vector<std::pair<std::string, std::string>>& guesses) {
    std::vector<char> excludedLetters;
    std::unordered_map<char, int> exactCounts;
    std::vector<std::pair<int, char>> positionRestrictions;

    for (const auto& [guess, feedback] : guesses) {
        for (size_t i = 0; i < guess.length(); ++i) {
            if (feedback[i] == 'b') {
                excludedLetters.push_back(guess[i]);
            } else if (feedback[i] == 'y') {
                positionRestrictions.emplace_back(i, guess[i]);
            }
        }
    }

    return {excludedLetters, exactCounts, positionRestrictions};
}

std::vector<std::string> filterWordsByExclusion(const std::vector<std::string>& words, const std::vector<char>& excludedLetters) {
    std::vector<std::string> filtered;
    std::copy_if(words.begin(), words.end(), std::back_inserter(filtered),
        [&](const std::string& word) {
            for (char c : excludedLetters) {
                if (word.find(c) != std::string::npos) return false;
            }
            return true;
        });
    return filtered;
}

std::vector<std::string> filterWordsByPosition(const std::vector<std::string>& words, const std::unordered_map<int, char>& greenLetters) {
    std::vector<std::string> filtered;
    std::copy_if(words.begin(), words.end(), std::back_inserter(filtered),
        [&](const std::string& word) {
            for (const auto& [pos, c] : greenLetters) {
                if (word[pos] != c) return false;
            }
            return true;
        });
    return filtered;
}

std::vector<std::string> filterWordsByExactCount(const std::vector<std::string>& words, const std::unordered_map<char, int>& exactCounts) {
    std::vector<std::string> filtered;
    std::copy_if(words.begin(), words.end(), std::back_inserter(filtered),
        [&](const std::string& word) {
            std::unordered_map<char, int> counts;
            for (char c : word) counts[c]++;
            for (const auto& [c, count] : exactCounts) {
                if (counts[c] != count) return false;
            }
            return true;
        });
    return filtered;
}

std::vector<std::string> filterWordsByExclusionAt(const std::vector<std::string>& words, const std::vector<std::pair<int, char>>& positionRestrictions) {
    std::vector<std::string> filtered;
    std::copy_if(words.begin(), words.end(), std::back_inserter(filtered),
        [&](const std::string& word) {
            for (const auto& [pos, c] : positionRestrictions) {
                if (word[pos] == c) return false;
            }
            return true;
        });
    return filtered;
}

// Core functions from your original code
std::vector<std::string> filterWordsByFeedback(const std::vector<std::string>& words, const std::string& guess, const std::string& feedback) {
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

std::vector<std::string> findBestGuesses(const std::vector<std::string>& remainingWords, int topN) {
    auto frequencies = calculateFrequencies(remainingWords);
    auto expectedWords = expectedRemainingWords(remainingWords, frequencies);
    std::vector<std::string> sortedWords(remainingWords);
    
    std::sort(sortedWords.begin(), sortedWords.end(), 
        [&expectedWords](const std::string& a, const std::string& b) {
            return expectedWords.at(a) < expectedWords.at(b);
        });
        
    if (sortedWords.size() > static_cast<size_t>(topN)) {
        sortedWords.resize(topN);
    }
    return sortedWords;
}

std::unordered_map<std::string, double> calculateFrequencies(const std::vector<std::string>& words) {
    std::unordered_map<std::string, double> frequencies;
    double totalFrequency = 0.0;
    for (const auto& word : words) {
        double freq = 1.0;
        frequencies[word] = freq;
        totalFrequency += freq;
    }
    for (auto& [word, freq] : frequencies) {
        freq /= totalFrequency;
    }
    return frequencies;
}

std::unordered_map<std::string, double> expectedRemainingWords(
    const std::vector<std::string>& words,
    const std::unordered_map<std::string, double>& frequencies) {
    
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

int countRemainingWordsAfterGuess(const std::string& guess, const std::string& answer, const std::vector<std::string>& words) {
    std::string feedback = generateFeedback(guess, answer);
    std::vector<std::string> filteredWords = filterWordsByFeedback(words, guess, feedback);
    return filteredWords.size();
}

std::string generateFeedback(const std::string& guess, const std::string& answer) {
    std::string feedback(guess.size(), 'b');
    
    // First pass: Mark green letters
    for (size_t i = 0; i < guess.size(); ++i) {
        if (guess[i] == answer[i]) {
            feedback[i] = 'g';
        }
    }
    
    // Second pass: Mark yellow letters
    std::unordered_map<char, int> remainingLetters;
    for (size_t i = 0; i < answer.size(); ++i) {
        if (feedback[i] != 'g') {
            remainingLetters[answer[i]]++;
        }
    }
    
    for (size_t i = 0; i < guess.size(); ++i) {
        if (feedback[i] == 'b' && remainingLetters[guess[i]] > 0) {
            feedback[i] = 'y';
            remainingLetters[guess[i]]--;
        }
    }
    
    return feedback;
}
