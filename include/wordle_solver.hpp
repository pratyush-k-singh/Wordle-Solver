#pragma once

#include <string>
#include <vector>
#include <unordered_map>

// Core filtering functions
std::vector<std::string> filterWordsByFeedback(const std::vector<std::string>& words, const std::string& guess, const std::string& feedback);
std::vector<std::string> filterWordsByExclusion(const std::vector<std::string>& words, const std::vector<char>& excludedLetters);
std::vector<std::string> filterWordsByPosition(const std::vector<std::string>& words, const std::unordered_map<int, char>& greenLetters);
std::vector<std::string> filterWordsByExactCount(const std::vector<std::string>& words, const std::unordered_map<char, int>& exactCounts);
std::vector<std::string> filterWordsByExclusionAt(const std::vector<std::string>& words, const std::vector<std::pair<int, char>>& positionRestrictions);

// Feedback and guess analysis functions
std::string generateFeedback(const std::string& guess, const std::string& answer);
std::unordered_map<char, int> getLetterCountsFromGuesses(const std::vector<std::pair<std::string, std::string>>& guesses);
std::unordered_map<int, char> getGreenLettersFromGuesses(const std::vector<std::pair<std::string, std::string>>& guesses);
std::tuple<std::vector<char>, std::unordered_map<char, int>, std::vector<std::pair<int, char>>> 
getRestrictionsFromGuesses(const std::vector<std::pair<std::string, std::string>>& guesses);

// Best guess calculation functions
std::vector<std::string> findBestGuesses(const std::vector<std::string>& remainingWords, int topN);
std::unordered_map<std::string, double> calculateFrequencies(const std::vector<std::string>& words);
std::unordered_map<std::string, double> expectedRemainingWords(const std::vector<std::string>& words, const std::unordered_map<std::string, double>& frequencies);
int countRemainingWordsAfterGuess(const std::string& guess, const std::string& answer, const std::vector<std::string>& words);