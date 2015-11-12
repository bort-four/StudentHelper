#include "studenthelperwiget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StudentHelperWiget w;
    w.show();

    try
    {
        return a.exec();
    }
    catch (QString message)
    {
        qDebug() << "Exception:" << message;
        return 1;
    }
}
