# Wordle Solver

A Python-based application designed to assist players in solving Wordle puzzles by recommending the best guesses based on letter frequency analysis.

## Features

- **Initial Recommendations:** The application provides a list of the top five recommended starting words based on letter frequency analysis.
- **Interactive GUI:** A simple graphical interface using `tkinter` where users can input their guesses and the results, and receive the best next guess based on the remaining possible words.
- **Customizable Word List:** The application loads valid Wordle words from a text file, allowing easy customization and updates.

## Project Structure

```plaintext
wordle_solver/
│
├── README.md                   # Project documentation
├── valid_words.txt             # List of valid Wordle words
├── starting_guesses.txt        # Generated list of top five starting words
├── wordle_solver.py            # Core logic for word filtering and guess recommendation
└── wordle_solver_gui.py        # Tkinter-based GUI for user interaction
```

## Installation

### Prerequisites

- **Python 3.x**
- Recommended to use a virtual environment to manage dependencies.

### Clone the Repository

```bash
git clone https://github.com/yourusername/wordle_solver.git
cd wordle_solver
```

### Install Required Libraries

This project requires only standard Python libraries. However, if you need to install `tkinter` on your system, you can do so as follows:

- **Ubuntu/Debian:**
  ```bash
  sudo apt-get install python3-tk
  ```

- **macOS:**
  `tkinter` should be installed with Python by default. If not, you may need to install it via Homebrew:
  ```bash
  brew install python-tk
  ```

- **Windows:**
  `tkinter` comes pre-installed with Python.

## Usage

### 1. Generate Initial Starting Guesses

Before running the GUI, you need to generate the top five starting guesses based on the word list:

```bash
python wordle_solver.py
```

This will read the `valid_words.txt` file and create a `starting_guesses.txt` file containing the top five recommended starting words.

### 2. Run the Wordle Solver GUI

To launch the GUI application:

```bash
python wordle_solver_gui.py
```

### 3. Interact with the GUI

- **Starting Guesses:** The GUI will display the top five recommended starting words from `starting_guesses.txt`.
- **Input Your Guess:** Enter your Wordle guess in the provided input field.
- **Input the Result:** Enter the result using `b`, `g`, and `y` to indicate black, green, and yellow feedback from Wordle (e.g., "bgybb").
- **Submit:** Click the "Submit" button to receive the next best guess.

### 4. Customize the Word List

If you want to update the word list:

1. Replace or update the `valid_words.txt` file with your own list of valid words (one word per line).
2. Run the `wordle_solver.py` script again to generate a new `starting_guesses.txt` file.

## How It Works

### Word Filtering

The core functionality revolves around filtering the list of possible words based on the feedback provided for each guess. The filtering is done using the following rules:

- **Green (`g`)**: The letter is in the correct position.
- **Yellow (`y`)**: The letter is in the word but in the wrong position.
- **Black (`b`)**: The letter is not in the word.

### Letter Frequency Analysis

The initial recommended guesses are based on maximizing the coverage of unique letters. The words are ranked based on the frequency of their letters across the entire word list.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request or open an Issue if you have suggestions for improvements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgments

- Inspired by the popular [Wordle](https://www.nytimes.com/games/wordle/index.html) game.
