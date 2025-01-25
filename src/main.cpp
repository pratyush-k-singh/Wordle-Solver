#include <QApplication>
#include "wordle_solver_window.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application-wide style sheet for enhanced UI
    app.setStyleSheet(R"(
        QWidget {
            font-size: 10pt;
        }
        QPushButton {
            padding: 8px 16px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
        QPushButton:pressed {
            background-color: #3d8b40;
        }
        QPushButton:disabled {
            background-color: #cccccc;
            color: #666666;
        }
        QLineEdit {
            padding: 8px;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
        QListWidget {
            border: 1px solid #ddd;
            border-radius: 4px;
            background-color: white;
        }
        QLabel {
            color: #333;
        }
        QGroupBox {
            font-weight: bold;
            border: 1px solid #ddd;
            border-radius: 4px;
            margin-top: 1ex;
            padding: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 3px;
            margin-left: 10px;
        }
    )");

    WordleSolverWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
