#include "wordle_solver_window.hpp"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>
#include <QRegularExpressionMatch>

WordleSolverWindow::WordleSolverWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Wordle Solver");
    setMinimumSize(600, 400);

    createWidgets();
    createLayouts();
    setupConnections();
    
    remainingWords = loadWords(VALID_WORDS_PATH);
    displayStartingWords();
}

void WordleSolverWindow::createWidgets() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create instruction label with detailed help
    instructionLabel = new QLabel(generateHelpText(), this);
    instructionLabel->setStyleSheet("padding: 10px; background: #f0f0f0; border-radius: 4px;");
    instructionLabel->setWordWrap(true);
    
    guessList = new QListWidget(this);
    guessList->setAlternatingRowColors(true);
    guessList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    guessList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    guessList->setStyleSheet(
        "QListWidget { background-color: white; }"
        "QListWidget::item { color: black; }"
        "QListWidget::item:alternate { background-color: #e6f3ff; }"
        "QListWidget::item:selected { background-color: #0078d4; color: white; }"
    );
    
    guessEntry = new QLineEdit(this);
    guessEntry->setPlaceholderText("Enter your guess (5 letters)");
    guessEntry->setMaxLength(5);
    
    resultEntry = new QLineEdit(this);
    resultEntry->setPlaceholderText("Enter result (e.g., bgybb)");
    resultEntry->setMaxLength(5);
    
    submitButton = new QPushButton("Submit", this);
    submitButton->setEnabled(false);
    
    resetButton = new QPushButton("Reset Game", this);
    resetButton->setStyleSheet("background-color: #ff9800;");
    
    remainingCountLabel = new QLabel("Remaining words: " + QString::number(remainingWords.size()), this);
    remainingCountLabel->setAlignment(Qt::AlignRight);
    
    statusLabel = new QLabel("Status: Ready to start", this);
    statusLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    statusLabel->setWordWrap(true);
}

void WordleSolverWindow::createLayouts() {
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Add instruction section
    mainLayout->addWidget(instructionLabel);
    
    // Create group box for recommended words
    QGroupBox *recommendedGroup = new QGroupBox("Recommended Words", this);
    QVBoxLayout *recommendedLayout = new QVBoxLayout(recommendedGroup);
    recommendedLayout->addWidget(guessList);
    recommendedLayout->addWidget(remainingCountLabel);
    mainLayout->addWidget(recommendedGroup);
    
    // Create input group
    QGroupBox *inputGroup = new QGroupBox("Make Your Guess", this);
    QGridLayout *inputLayout = new QGridLayout(inputGroup);
    
    inputLayout->addWidget(new QLabel("Guess:", this), 0, 0);
    inputLayout->addWidget(guessEntry, 0, 1);
    inputLayout->addWidget(new QLabel("Result:", this), 1, 0);
    inputLayout->addWidget(resultEntry, 1, 1);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(resetButton);
    inputLayout->addLayout(buttonLayout, 2, 0, 1, 2);
    
    mainLayout->addWidget(inputGroup);
    
    // Add status label at the bottom
    mainLayout->addWidget(statusLabel);
}

void WordleSolverWindow::setupConnections() {
    connect(submitButton, &QPushButton::clicked, this, &WordleSolverWindow::processGuess);
    connect(resetButton, &QPushButton::clicked, this, &WordleSolverWindow::resetGame);
    connect(guessEntry, &QLineEdit::textChanged, this, &WordleSolverWindow::validateInput);
    connect(resultEntry, &QLineEdit::textChanged, this, &WordleSolverWindow::validateInput);
    connect(guessList, &QListWidget::itemClicked, this, &WordleSolverWindow::onGuessSelected);
}

QString WordleSolverWindow::generateHelpText() const {
    return "Instructions:\n"
           "1. Enter your 5-letter guess in the guess field\n"
           "2. After trying the word in Wordle, enter the result using:\n"
           "   • b (gray/black) - Letter not in word\n"
           "   • y (yellow) - Letter in word but wrong position\n"
           "   • g (green) - Letter in correct position\n"
           "3. Click Submit to get your next best guesses\n"
           "4. Click on any suggested word to copy it to the guess field";
}

QStringList WordleSolverWindow::loadWords(const QString &filename) {
    QStringList words;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString word = in.readLine().trimmed().toLower();
            if (word.length() == 5) {
                words.append(word);
            }
        }
    } else {
        updateStatusMessage("Error: Could not load word list: " + filename, true);
    }
    return words;
}

void WordleSolverWindow::displayStartingWords() {
    startingWords = loadWords(STARTING_GUESSES_PATH);
    guessList->clear();
    guessList->addItems(startingWords);
    remainingCountLabel->setText("Remaining words: " + QString::number(remainingWords.size()));
}

void WordleSolverWindow::validateInput() {
    QString guess = guessEntry->text().trimmed().toLower();
    QString result = resultEntry->text().trimmed().toLower();
    
    bool isValidGuess = guess.length() == 5 && remainingWords.contains(guess);
    bool isValidResult = result.isEmpty() || VALID_RESULT_PATTERN.match(result).hasMatch();
    
    submitButton->setEnabled(isValidGuess && result.length() == 5 && isValidResult);
    
    // Provide immediate feedback
    if (guess.length() == 5 && !remainingWords.contains(guess)) {
        updateStatusMessage("Warning: '" + guess + "' is not in the word list", true);
    } else if (!isValidResult && !result.isEmpty()) {
        updateStatusMessage("Use only b, g, y for the result", true);
    } else {
        statusLabel->clear();
    }
}

void WordleSolverWindow::processGuess() {
    QString guess = guessEntry->text().trimmed().toLower();
    QString result = resultEntry->text().trimmed().toLower();

    if (guess.length() != 5 || result.length() != 5 || !VALID_RESULT_PATTERN.match(result).hasMatch()) {
        updateStatusMessage("Invalid input. Please check your guess and result.", true);
        return;
    }

    updateStatusMessage("Processing...");

    // Convert to std::string for the solver
    std::vector<std::string> currentWords;
    for (const QString &word : remainingWords) {
        currentWords.push_back(word.toStdString());
    }

    // Filter words based on the guess
    std::vector<std::string> filtered = filterWordsByFeedback(
        currentWords,
        guess.toStdString(),
        result.toStdString()
    );

    // Update remaining words
    remainingWords.clear();
    for (const std::string &word : filtered) {
        remainingWords.append(QString::fromStdString(word));
    }

    // Update display count
    remainingCountLabel->setText("Remaining words: " + QString::number(remainingWords.size()));

    // Update display based on remaining words
    if (remainingWords.size() == 1) {
        if (remainingWords[0] == guess) {
            updateStatusMessage("🎉 Congratulations! You found the word: " + guess);
        } else {
            updateStatusMessage("The word must be: " + remainingWords[0]);
        }
        guessList->clear();
        guessList->addItem(remainingWords[0]);
    } else if (remainingWords.isEmpty()) {
        updateStatusMessage("No possible words remaining. Please check your input.", true);
    } else {
        // Get best guesses
        std::vector<std::string> bestGuessesStd = findBestGuesses(
            std::vector<std::string>(filtered.begin(), filtered.end()),
            MAX_DISPLAYED_GUESSES
        );
        
        // Update display
        guessList->clear();
        for (const std::string &word : bestGuessesStd) {
            guessList->addItem(QString::fromStdString(word));
        }
        
        updateStatusMessage(QString::number(remainingWords.size()) + 
                          " possible words remain. Try one of the suggested words.");
    }

    // Store in history
    guessHistory.append(guess + " -> " + result);
}

void WordleSolverWindow::resetGame() {
    remainingWords = loadWords(VALID_WORDS_PATH);
    guessEntry->clear();
    resultEntry->clear();
    guessHistory.clear();
    displayStartingWords();
    updateStatusMessage("Game reset. Ready to start!");
}

void WordleSolverWindow::updateStatusMessage(const QString &message, bool isError) {
    statusLabel->setText(message);
    statusLabel->setStyleSheet(isError ? 
        "font-weight: bold; padding: 5px; color: #d32f2f;" :
        "font-weight: bold; padding: 5px; color: #333;");
}

void WordleSolverWindow::onGuessSelected(QListWidgetItem* item) {
    if (item) {
        guessEntry->setText(item->text());
        resultEntry->setFocus();
    }
}
