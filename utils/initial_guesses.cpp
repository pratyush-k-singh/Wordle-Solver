#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <filesystem>

// Load words from a file into a vector of strings
std::vector<std::string> load_words(const std::string& filename) {
    std::vector<std::string> words;
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("File not found: " + filename);
    }

    std::string word;
    while (std::getline(file, word)) {
        words.push_back(word);
    }

    return words;
}

// Count the frequency of each letter across the list of words
std::unordered_map<char, int> count_letter_frequencies(const std::vector<std::string>& words) {
    std::unordered_map<char, int> letter_freq;

    for (const auto& word : words) {
        std::set<char> unique_letters(word.begin(), word.end());  // Set to ensure unique letters
        for (char letter : unique_letters) {
            letter_freq[letter]++;
        }
    }

    return letter_freq;
}

// Find the best single words that maximize letter coverage
std::vector<std::string> get_best_single_words(const std::vector<std::string>& words, int top_n = 5) {
    std::unordered_map<char, int> letter_freq = count_letter_frequencies(words);

    std::vector<std::pair<int, std::string>> scored_words;

    for (const auto& word : words) {
        int score = 0;
        std::set<char> unique_letters(word.begin(), word.end());
        for (char letter : unique_letters) {
            score += letter_freq[letter];
        }
        scored_words.emplace_back(score, word);
    }

    // Sort words by their score in descending order
    std::sort(scored_words.rbegin(), scored_words.rend());

    std::vector<std::string> best_words;
    for (int i = 0; i < std::min(top_n, (int)scored_words.size()); ++i) {
        best_words.push_back(scored_words[i].second);
    }

    return best_words;
}

// Save the list of best guesses to a file
void save_initial_guesses(const std::string& filename, const std::vector<std::string>& guesses) {
    std::ofstream file(filename);

    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    for (const auto& guess : guesses) {
        file << guess << std::endl;
    }
}

int main() {
    std::string input_filename = "../data/valid_words.txt";
    std::string output_filename = "../data/starting_guesses.txt";

    try {
        std::vector<std::string> words = load_words(input_filename);

        std::cout << "Loaded " << words.size() << " words from " << input_filename << "." << std::endl;

        if (words.empty()) {
            std::cout << "The word list is empty." << std::endl;
            return 0;
        }

        std::vector<std::string> best_guesses = get_best_single_words(words);

        if (!best_guesses.empty()) {
            std::cout << "Top initial guesses: ";
            for (const auto& guess : best_guesses) {
                std::cout << guess << " ";
            }
            std::cout << std::endl;

            save_initial_guesses(output_filename, best_guesses);
            std::cout << "Best initial guesses saved to " << output_filename << std::endl;
        } else {
            std::cout << "No guesses found." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}
