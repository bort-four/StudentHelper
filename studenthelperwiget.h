#ifndef STUDENTHELPERWIGET_H
#define STUDENTHELPERWIGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "studenthelper.h"
#include "filebrowserwidget.h"
#include "searcherwidget.h"
#include "printerwidget.h"

namespace Ui {
class StudentHelperWiget;
}


class StudentHelperWiget : public QWidget
{
    Q_OBJECT

public:
    explicit StudentHelperWiget(QWidget *parent = 0);
    ~StudentHelperWiget();

    void setStudentHelper(StudentHelper* stHelperPtr);
    StudentHelper* getStudentHelper();

private:
    Ui::StudentHelperWiget *ui;
    StudentHelper* _stHelperPtr;
    FileBrowserWidget* _browserWidget;
    SearcherWidget* _searcherWidget;
    PrinterWidget* _printerWidget;
};



#endif // STUDENTHELPERWIGET_H
