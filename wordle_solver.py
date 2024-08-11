import os
from wordfreq import word_frequency

def load_words(filename):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(script_dir, filename)
    with open(file_path, 'r') as file:
        words = [line.strip() for line in file]
    return words

valid_words = load_words("valid_words.txt")

def filter_words(words, filters):
    filtered_words = words
    for func, args in filters:
        filtered_words = func(filtered_words, *args)
    return list(filtered_words)

def filter_words_by_min_count(words, letter, min_count):
    return (word for word in words if word.count(letter) >= min_count)

def filter_words_by_exact_count(words, letter, exact_count):
    return (word for word in words if word.count(letter) == exact_count)

def filter_words_by_position(words, letter, position):
    return (word for word in words if word[position] == letter)

def filter_words_by_exclusion_at(words, letter, position):
    return (word for word in words if word[position] != letter)

def filter_words_by_exclusion(words, letter):
    return (word for word in words if letter not in word)

def count_letter_frequencies(words):
    frequencies = {}
    for word in words:
        word_freq = {}
        for char in word:
            if char in word_freq:
                word_freq[char] += 1
            else:
                word_freq[char] = 1
        for char, count in word_freq.items():
            if char in frequencies:
                frequencies[char] += count
            else:
                frequencies[char] = count
    return frequencies

def get_letter_counts_from_guesses(guesses):
    letter_counts = {}
    for word, feedback in guesses:
        temp_counts = {}
        for index, char in enumerate(word):
            if feedback[index] in {'g', 'y'}:
                if char in temp_counts:
                    temp_counts[char] += 1
                else:
                    temp_counts[char] = 1
        for char in temp_counts:
            if char in letter_counts:
                letter_counts[char] = max(letter_counts[char], temp_counts[char])
            else:
                letter_counts[char] = temp_counts[char]
    return letter_counts

def get_green_letters_from_guesses(guesses):
    green_letters = {}
    for word, feedback in guesses:
        for index, char in enumerate(word):
            if feedback[index] == 'g':
                green_letters[index] = char
    return green_letters

def get_restrictions_from_guesses(guesses):
    excluded_letters = set()
    exact_counts = {}
    position_restrictions = {}
    for word, feedback in guesses:
        occurrences = {}
        for index, char in enumerate(word):
            if char not in occurrences:
                occurrences[char] = {}
            occurrences[char][index] = feedback[index]
        for char, feedbacks in occurrences.items():
            feedback_types = set(feedbacks.values())
            if 'b' in feedback_types:
                count = sum(1 for feedback in feedbacks.values() if feedback in {'g', 'y'})
                exact_counts[char] = count
                excluded_positions = {i for i, feedback in feedbacks.items() if feedback in {'b', 'y'}}
                if char not in position_restrictions:
                    position_restrictions[char] = set()
                position_restrictions[char].update(excluded_positions)
            else:
                excluded_positions = {i for i, feedback in feedbacks.items() if feedback == 'y'}
                if char not in position_restrictions:
                    position_restrictions[char] = set()
                position_restrictions[char].update(excluded_positions)
    return excluded_letters, exact_counts, position_restrictions

def calculate_frequencies(words):
    frequencies = {}
    total_frequency = 0
    for word in words:
        freq = word_frequency(word, 'en')
        frequencies[word] = freq
        total_frequency += freq
    return {word: freq / total_frequency for word, freq in frequencies.items()}

def expected_remaining_words(words, frequencies):
    expected_counts = {}
    for guess in words:
        total_count = 0
        for answer in words:
            probability = frequencies.get(answer, 0)
            remaining_count = count_remaining_words_after_guess(guess, answer, words)
            total_count += remaining_count * probability
        expected_counts[guess] = total_count
    return expected_counts

def count_remaining_words_after_guess(guess, answer, words):
    feedback = generate_feedback(guess, answer)
    filtered_words = filter_words(words, [(filter_words_by_feedback, (guess, feedback))])
    return len(filtered_words)

def generate_feedback(guess, answer):
    feedback = {}
    for index in range(5):
        if guess[index] == answer[index]:
            feedback[index] = "g"
        elif guess[index] not in answer:
            feedback[index] = "b"
    if len(feedback) == 5:
        return ''.join(feedback.get(i, "b") for i in range(5))
    filtered_guess = {i: char for i, char in enumerate(guess) if i not in feedback}
    filtered_answer = [char for i, char in enumerate(answer) if i not in feedback or feedback.get(i) != "g"]
    for index in range(5):
        if index not in feedback:
            char = filtered_guess.get(index)
            if char in filtered_answer:
                feedback[index] = "y"
                filtered_answer.remove(char)
            else:
                feedback[index] = "b"
    return ''.join(feedback.get(i, "b") for i in range(5))

def filter_words_by_feedback(words, guess, feedback):
    remaining_words = filter_words(words, [
        (filter_words_by_min_count, (char, count))
        for char, count in get_letter_counts_from_guesses([(guess, feedback)]).items()
    ])
    remaining_words = filter_words(remaining_words, [
        (filter_words_by_position, (letter, index))
        for index, letter in get_green_letters_from_guesses([(guess, feedback)]).items()
    ])
    remaining_words = filter_words(remaining_words, [
        (filter_words_by_exclusion, (letter,))
        for letter in get_restrictions_from_guesses([(guess, feedback)])[0]
    ])
    remaining_words = filter_words(remaining_words, [
        (filter_words_by_exact_count, (letter, count))
        for letter, count in get_restrictions_from_guesses([(guess, feedback)])[1].items()
    ])
    remaining_words = filter_words(remaining_words, [
        (filter_words_by_exclusion_at, (letter, position))
        for letter, positions in get_restrictions_from_guesses([(guess, feedback)])[2].items()
        for position in positions
    ])
    return remaining_words

def find_best_guesses(remaining_words, top_n=3):
    frequencies = calculate_frequencies(remaining_words)
    expected_words = expected_remaining_words(remaining_words, frequencies)
    sorted_words = sorted(expected_words, key=expected_words.get)
    return sorted_words[:top_n]
