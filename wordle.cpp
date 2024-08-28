#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSet>
#include "wordle_solver.hpp"

class WordleSolverApp : public QWidget {
    Q_OBJECT

public:
    WordleSolverApp(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Wordle Solver");
        setFixedSize(450, 300);

        createWidgets();
        remainingWords = loadWords("valid_words.txt"); // Start with all valid words

        displayStartingWords();
    }

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *inputLayout;
    QLabel *guessLabel;
    QListWidget *guessList;
    QLineEdit *guessEntry;
    QLineEdit *resultEntry;
    QPushButton *submitButton;
    QLabel *statusLabel;
    QStringList startingWords;
    QStringList remainingWords;

    void createWidgets() {
        mainLayout = new QVBoxLayout(this);

        // Starting guesses label and list
        guessLabel = new QLabel("Recommended starting guesses:", this);
        mainLayout->addWidget(guessLabel);

        guessList = new QListWidget(this);
        mainLayout->addWidget(guessList);

        // Input fields
        inputLayout = new QHBoxLayout();
        mainLayout->addLayout(inputLayout);

        guessEntry = new QLineEdit(this);
        guessEntry->setPlaceholderText("Enter your guess");
        inputLayout->addWidget(guessEntry);

        resultEntry = new QLineEdit(this);
        resultEntry->setPlaceholderText("Enter the result (bgy)");
        inputLayout->addWidget(resultEntry);

        // Submit button
        submitButton = new QPushButton("Submit", this);
        connect(submitButton, &QPushButton::clicked, this, &WordleSolverApp::processGuess);
        mainLayout->addWidget(submitButton);

        // Status label
        statusLabel = new QLabel("Status: Ready", this);
        mainLayout->addWidget(statusLabel);
    }

    QStringList loadWords(const QString &filename) {
        QStringList words;
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString word = in.readLine().trimmed();
                if (!word.isEmpty()) {
                    words.append(word);
                }
            }
        } else {
            QMessageBox::critical(this, "Error", "File not found: " + filename);
        }
        return words;
    }

    void displayStartingWords() {
        startingWords = loadWords("starting_guesses.txt");
        guessList->clear();
        guessList->addItems(startingWords);
    }

    void processGuess() {
        QString guess = guessEntry->text().trimmed();
        QString result = resultEntry->text().trimmed();

        if (guess.length() != 5 || result.length() != 5 || !result.contains(QRegExp("^[bgy]{5}$"))) {
            statusLabel->setText("Invalid input. Please enter a valid guess and result.");
            return;
        }

        statusLabel->setText("Processing...");

        // Update the remaining words based on the feedback
        QStringList previousRemainingWords = remainingWords;
        remainingWords = filterWordsByFeedback(remainingWords, guess, result);

        if (remainingWords.size() == 1) {
            if (remainingWords[0] == guess) {
                statusLabel->setText("Congratulations on finding the word: " + guess + "!");
            } else {
                statusLabel->setText("The word must be: " + remainingWords[0]);
            }
        } else if (remainingWords.isEmpty()) {
            statusLabel->setText("No possible words remaining. Please check your input.");
        } else {
            QStringList bestChoices = findBestGuesses(remainingWords, 7);
            statusLabel->setText("The best choices for your next guess are: " + bestChoices.join(", "));
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WordleSolverApp solverApp;
    solverApp.show();

    return app.exec();
}
