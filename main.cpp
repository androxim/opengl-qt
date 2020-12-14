#include "widget.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setlocale(LC_NUMERIC,"C");

    // smoothing
    QSurfaceFormat fmt;
    fmt.setSamples(16);
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow w;

    w.setFixedSize(557,236);
    w.setWindowTitle("OpenGL flow controlled by attention");

    w.show();

    return a.exec();
}
