#include "studenthelperwiget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StudentHelperWiget w;
    w.show();

//    int b[]{1, 2, 3, 4, 5};
//    int c{1};
//    for (auto x : b)
//        qDebug() << x;

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
