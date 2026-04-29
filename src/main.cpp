#include <QApplication>
#include <QMainWindow>
#include <QLabel>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setStyle("Fusion");
    QPalette dark;
    dark.setColor(QPalette::Window,          QColor(30, 30, 30));
    dark.setColor(QPalette::WindowText,      QColor(220, 220, 220));
    dark.setColor(QPalette::Base,            QColor(40, 40, 40));
    dark.setColor(QPalette::AlternateBase,   QColor(50, 50, 50));
    dark.setColor(QPalette::Text,            QColor(220, 220, 220));
    dark.setColor(QPalette::Button,          QColor(50, 50, 50));
    dark.setColor(QPalette::ButtonText,      QColor(220, 220, 220));
    dark.setColor(QPalette::Highlight,       QColor(0, 120, 215));
    dark.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
    app.setPalette(dark);

    QMainWindow w;
    w.setWindowTitle("Service Port Scanner — Prototype");
    w.setCentralWidget(new QLabel("SPS — GUI placeholder"));
    w.resize(1200, 700);
    w.show();

    return app.exec();
}
