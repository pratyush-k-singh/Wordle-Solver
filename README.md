# Wordle Solver

This application provides an enhanced Qt-based graphical interface to assist players in solving Wordle puzzles. The solver employs sophisticated letter frequency analysis and pattern matching to suggest optimal guesses, making it an invaluable tool for both novice and experienced Wordle players.

It is important to note that this tool uses a limited set of common words, and as Wordle is constantly updating their game to include more and more words, this tool may not always be able to successfully generate the final word. Personal judgement should always be exercised.

## Features

The Wordle Solver offers a comprehensive set of features designed to enhance the Wordle playing experience:

The modern, intuitive interface includes clear instructions, real-time feedback, and a clean design that makes solving Wordle puzzles more efficient. The application provides optimal starting words based on letter frequency analysis and updates its suggestions after each guess using advanced filtering algorithms.

Key features include:
- Intelligent word recommendations based on detailed pattern analysis
- Real-time input validation with immediate feedback
- Clear visual feedback using color-coded status messages
- One-click word selection from recommendations
- Ability to reset the game at any time
- Progress tracking with remaining word count display

## Project Structure

The project follows a well-organized structure:

```plaintext
wordle_solver/
│
├── README.md                      # Project documentation
├── LICENSE                        # MIT License
├── data/
│   ├── valid_words.txt           # Comprehensive list of valid Wordle words
│   └── starting_guesses.txt      # Generated optimal starting words
├── src/
│   ├── main.cpp                  # Application entry point
│   ├── wordle_solver.cpp         # Core solving algorithms
│   └── wordle_solver_window.cpp  # GUI implementation
├── include/
│   ├── wordle_solver.hpp         # Core solver declarations
│   └── wordle_solver_window.hpp  # GUI component declarations
└── CMakeLists.txt                # Build configuration
```

## Installation

### Prerequisites

- C++17 compatible compiler
- CMake 3.10 or later
- Qt 6.x (Qt 6.8 or later recommended)
- MinGW-w64 (for Windows) or appropriate compiler for your platform

### Building the Application

1. Clone the repository and navigate to the project directory:
   ```bash
   git clone [repository-url]
   cd wordle_solver
   ```

2. Create and navigate to the build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure with CMake:
   - For Windows with MinGW:
     ```bash
     cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/[version]/mingw_64" ..
     ```
   - For other platforms:
     ```bash
     cmake -DCMAKE_PREFIX_PATH=[Qt installation path] ..
     ```

4. Build the project:
   ```bash
   cmake --build .
   ```

## Using the Application

1. Launch the application by running the executable from the build directory.

2. The interface displays:
   - Clear instructions for use
   - A list of recommended starting words
   - Input fields for your guess and the Wordle feedback
   - Status updates and remaining word count

3. For each guess:
   - Enter your 5-letter guess
   - Input the Wordle feedback using:
     - 'b' for gray/black (letter not in word)
     - 'y' for yellow (letter in wrong position)
     - 'g' for green (letter in correct position)
   - The application will provide optimal next guesses based on the feedback

4. The solver automatically updates its recommendations and displays:
   - The number of remaining possible words
   - A list of optimal next guesses
   - Clear status messages about the current state

## Word Lists

The application uses two primary data files:
- `valid_words.txt`: Contains the complete list of valid 5-letter words
- `starting_guesses.txt`: Contains pre-calculated optimal starting words

Both files should be present in the `data` directory for the application to function correctly.

## Technical Implementation

The solver employs several sophisticated algorithms:
- Pattern matching for precise word filtering
- Letter frequency analysis for optimal guess suggestions
- Real-time input validation and feedback
- Efficient word list management and updates

The Qt-based interface ensures responsive performance and a consistent user experience across platforms.

## License

This project is licensed under the MIT License. See the LICENSE file for complete terms.
