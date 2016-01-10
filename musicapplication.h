#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

#include <QtWidgets/QApplication>
#include <QtCore/QSharedMemory>

class MusicWindow;

class MusicApplication : public QApplication
{
    Q_OBJECT

    public:
        MusicApplication(int &argc, char **argv);
        ~MusicApplication();
        bool isRunning();

    private:
        bool hasRun;
        QSharedMemory sharedMemory;
};

#endif // MUSICAPPLICATION_H
