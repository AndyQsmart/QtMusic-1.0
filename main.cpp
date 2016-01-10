#include <QApplication>
#include "musicapplication.h"
#include "UI/musicwindow.h"

int main(int argc, char *argv[])
{
    MusicApplication app(argc, argv);
    if (app.isRunning())
        return 0;
    MusicWindow musicWindow;
    musicWindow.show();
    return app.exec();
}
