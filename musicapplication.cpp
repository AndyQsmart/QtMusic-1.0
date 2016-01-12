#include "musicapplication.h"
#include <QIcon>

MusicApplication::MusicApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    //setWindowIcon(QIcon(":/icons/res/icons/app_icon"));
    sharedMemory.setKey("QtMusic");
    if (sharedMemory.attach())
        hasRun = true;
    else
    {
        hasRun = false;
        //create shared memory.
        if (!sharedMemory.create(1))
        {
            qDebug("Unable to create single instance.");
            return;
        }
    }
}

MusicApplication::~MusicApplication()
{
}

bool MusicApplication::isRunning()
{
    return hasRun;
}
