#include <QApplication>
#include "musicapplication.h"
#include "UI/musicwindow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // 高分辨率适配
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    MusicApplication app(argc, argv);
    if (app.isRunning())
        return 0;
    MusicWindow musicWindow;
    musicWindow.show();
    return app.exec();
}
