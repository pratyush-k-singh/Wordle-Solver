import os
from collections import Counter

def load_words(filename):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(script_dir, filename)
    with open(file_path, 'r') as file:
        words = [line.strip() for line in file]
    return words

def count_letter_frequencies(words):
    """
    Count the frequency of each letter across the list of words.

    Args:
        words (list of str): List of candidate words.

    Returns:
        dict: A dictionary with letters as keys and their frequencies as values.
    """
    letter_freq = Counter()
    for word in words:
        letter_freq.update(word)  # Count each letter in the word
    return letter_freq

def get_best_single_words(words, top_n=5):
    """
    Find the best single words that maximize letter coverage.

    Args:
        words (list of str): List of candidate words.
        top_n (int): Number of top words to return.

    Returns:
        list: A list of the top words.
    """
    letter_freq = count_letter_frequencies(words)
    sorted_words = sorted(words, key=lambda word: sum(letter_freq[char] for char in set(word)), reverse=True)
    return sorted_words[:top_n]

def save_initial_guesses(filename, guesses):
    """
    Save the list of single words to a file in the same directory as the script.

    Args:
        filename (str): The name of the file to save to.
        guesses (list of str): The list of single words to save.
    """
    script_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(script_dir, filename)
    with open(file_path, 'w') as file:
        for word in guesses:
            file.write(f"{word}\n")

def main():
    """
    Main function to read the word list from a file, calculate the best initial guesses, 
    and save them to a file.
    """
    input_filename = "valid_words.txt"
    output_filename = "starting_guesses.txt"
    try:
        words = load_words(input_filename)
        
        print(f"Loaded {len(words)} words from {input_filename}.")
        
        if not words:
            print("The word list is empty.")
            return
        
        best_guesses = get_best_single_words(words)
        
        if best_guesses:
            print(f"Top initial guesses: {', '.join(best_guesses)}")
            save_initial_guesses(output_filename, best_guesses)
            print(f"Best initial guesses saved to {output_filename}")
        else:
            print("No guesses found.")

    except FileNotFoundError:
        print(f"File not found: {input_filename}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
