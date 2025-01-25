#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QGroupBox>
#include <QRegularExpression>
#include "wordle_solver.hpp"

class WordleSolverWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit WordleSolverWindow(QWidget *parent = nullptr);
    virtual ~WordleSolverWindow() = default;

public slots:
    void processGuess();
    void validateInput();
    void resetGame();
    void onGuessSelected(QListWidgetItem* item);

private:
    // Setup methods
    void createWidgets();
    void createLayouts();
    void setupConnections();
    
    // Utility methods
    QStringList loadWords(const QString &filename);
    void displayStartingWords();
    void updateStatusMessage(const QString &message, bool isError = false);
    QString generateHelpText() const;
    
    // Widgets
    QWidget *centralWidget;
    QListWidget *guessList;
    QLineEdit *guessEntry;
    QLineEdit *resultEntry;
    QPushButton *submitButton;
    QPushButton *resetButton;
    QLabel *statusLabel;
    QLabel *instructionLabel;
    QLabel *remainingCountLabel;
    
    // Game state
    QStringList startingWords;
    QStringList remainingWords;
    QStringList guessHistory;
    
    // Constants
    const QString VALID_WORDS_PATH = "../data/valid_words.txt";
    const QString STARTING_GUESSES_PATH = "../data/starting_guesses.txt";
    const int MAX_DISPLAYED_GUESSES = 7;
    const QRegularExpression VALID_RESULT_PATTERN{"^[bgy]{5}$"};
};
