#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "mainwindow.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

#ifndef QT_NO_OPENGL
    MainWindow widget;
    widget.setWindowTitle("НА ЧТО Я ТРАЧУ 5К");
    widget.setFixedSize(1000, 720);
    widget.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif

    return a.exec();
}
