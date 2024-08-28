#ifndef WORDLE_SOLVER_HPP
#define WORDLE_SOLVER_HPP

#include <vector>
#include <string>

// Define the namespace if you're using one
namespace wordle {

// Enum for feedback from Wordle guesses
enum class Feedback {
    CORRECT,     // Correct letter and position
    PRESENT,     // Correct letter, wrong position
    ABSENT       // Letter is not in the word
};

// Class to handle the Wordle solver logic
class WordleSolver {
public:
    // Constructor
    WordleSolver(const std::vector<std::string>& validWords, const std::vector<std::string>& startingGuesses);

    // Method to make a guess
    std::string makeGuess(const std::vector<std::string>& possibleWords);

    // Method to process feedback
    void processFeedback(const std::string& guess, const std::vector<Feedback>& feedback);

    // Method to get the list of possible words
    std::vector<std::string> getPossibleWords() const;

private:
    // List of valid words
    std::vector<std::string> validWords_;

    // List of starting guesses
    std::vector<std::string> startingGuesses_;

    // Internal list of possible words after filtering
    std::vector<std::string> possibleWords_;

    // Method to filter possible words based on feedback
    void filterPossibleWords(const std::string& guess, const std::vector<Feedback>& feedback);
};

} // namespace wordle

#endif // WORDLE_SOLVER_HPP
