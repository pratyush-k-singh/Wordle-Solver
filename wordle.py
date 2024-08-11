import os
import tkinter as tk
from tkinter import ttk
from wordle_solver import filter_words_by_feedback, find_best_guesses, load_words

class WordleSolverApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Wordle Solver")
        self.root.geometry("450x300")  # Set the window size to 450x300

        self.create_widgets()
        self.remaining_words = load_words("valid_words.txt")  # Start with all valid words

    def create_widgets(self):
        self.guess_frame = ttk.Frame(self.root, padding="10")
        self.guess_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        self.guess_frame.columnconfigure(0, weight=1)

        self.guess_label = ttk.Label(self.guess_frame, text="Recommended starting guesses:", anchor="center")
        self.guess_label.grid(row=0, column=0, sticky=tk.W, pady=(0, 10))

        self.guess_listbox = tk.Listbox(self.guess_frame, height=5, width=50, selectmode=tk.SINGLE)
        self.guess_listbox.grid(row=1, column=0, sticky=(tk.W, tk.E))

        self.guess_scrollbar = ttk.Scrollbar(self.guess_frame, orient=tk.VERTICAL, command=self.guess_listbox.yview)
        self.guess_scrollbar.grid(row=1, column=1, sticky=(tk.N, tk.S))
        self.guess_listbox.config(yscrollcommand=self.guess_scrollbar.set)

        self.input_frame = ttk.Frame(self.root, padding="10")
        self.input_frame.grid(row=1, column=0, sticky=(tk.W, tk.E))
        self.input_frame.columnconfigure(0, weight=1)

        self.input_guess_label = ttk.Label(self.input_frame, text="Enter your guess:")
        self.input_guess_label.grid(row=0, column=0, sticky=tk.W)

        self.guess_entry = ttk.Entry(self.input_frame, justify="center")
        self.guess_entry.grid(row=0, column=1, sticky=(tk.W, tk.E), padx=5)

        self.result_label = ttk.Label(self.input_frame, text="Enter the result (bgy):")
        self.result_label.grid(row=1, column=0, sticky=tk.W)

        self.result_entry = ttk.Entry(self.input_frame, justify="center")
        self.result_entry.grid(row=1, column=1, sticky=(tk.W, tk.E), padx=5)

        self.submit_button = ttk.Button(self.input_frame, text="Submit", command=self.process_guess)
        self.submit_button.grid(row=2, column=0, columnspan=2, pady=5)

        self.status_label = ttk.Label(self.root, text="Status: Ready", wraplength=250, anchor="center")
        self.status_label.grid(row=2, column=0, sticky=(tk.W, tk.E), pady=10)

        self.starting_words = self.load_starting_words("starting_guesses.txt")
        self.display_starting_words()

    def load_starting_words(self, filename):
        """
        Load starting words from a file.
        """
        script_dir = os.path.dirname(os.path.abspath(__file__))
        file_path = os.path.join(script_dir, filename)
        words = []
        try:
            with open(file_path, 'r') as file:
                words = [line.strip() for line in file]
        except FileNotFoundError:
            print(f"File not found: {file_path}")
            raise
        return words

    def display_starting_words(self):
        """
        Display the starting words in the Listbox.
        """
        self.guess_listbox.delete(0, tk.END)
        for word in self.starting_words:
            self.guess_listbox.insert(tk.END, word)

    def process_guess(self):
        """
        Process the user's guess and update the results.
        """
        guess = self.guess_entry.get().strip()
        result = self.result_entry.get().strip()

        if len(guess) != 5 or len(result) != 5 or not set(result).issubset({'b', 'g', 'y'}):
            self.status_label.config(text="Invalid input. Please enter a valid guess and result.")
            return

        self.status_label.config(text="Processing...")

        # Update the remaining words based on the feedback
        previous_remaining_words = self.remaining_words[:]
        self.remaining_words = filter_words_by_feedback(self.remaining_words, guess, result)
        
        if len(self.remaining_words) == 1:
            if self.remaining_words[0] == guess:
                self.status_label.config(text=f"Congratulations on finding the word: {guess}!")
            else:
                self.status_label.config(text=f"The word must be: {self.remaining_words[0]}")
        elif len(self.remaining_words) == 0:
            self.status_label.config(text="No possible words remaining. Please check your input.")
        else:
            best_choices = find_best_guesses(self.remaining_words, 7)
            self.status_label.config(text=f"The best choices for your next guess are: {', '.join(best_choices)}")

def main():
    root = tk.Tk()
    app = WordleSolverApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
