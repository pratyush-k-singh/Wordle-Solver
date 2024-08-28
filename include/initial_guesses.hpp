#ifndef INITIAL_GUESSES_HPP
#define INITIAL_GUESSES_HPP

#include <vector>
#include <string>

// Load words from a file into a vector of strings
std::vector<std::string> load_words(const std::string& filename);

// Count the frequency of each letter across the list of words
std::unordered_map<char, int> count_letter_frequencies(const std::vector<std::string>& words);

// Find the best single words that maximize letter coverage
std::vector<std::string> get_best_single_words(const std::vector<std::string>& words, int top_n = 5);

// Save the list of best guesses to a file
void save_initial_guesses(const std::string& filename, const std::vector<std::string>& guesses);

#endif // INITIAL_GUESSES_HPP
