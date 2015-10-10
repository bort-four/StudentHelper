#include "studenthelperwiget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StudentHelperWiget w;
    w.show();

    return a.exec();
}
