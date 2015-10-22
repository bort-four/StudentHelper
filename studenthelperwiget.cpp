#include "studenthelperwiget.h"
#include "ui_studenthelperwiget.h"

#include <QtCore>
#include <QStandardItemModel>
#include <QLabel>
#include "filebrowserwidget.h"


StudentHelperWiget::StudentHelperWiget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentHelperWiget)
{
    ui->setupUi(this);

    setStudentHelper(new StudentHelper(this));

    _searcherWidget = new SearcherWidget(NULL, _stHelperPtr);
    ui->searchTab->setLayout(new QHBoxLayout);
    ui->searchTab->layout()->addWidget(_searcherWidget);

    _printerWidget = new PrinterWidget(NULL, _stHelperPtr);
    ui->printTab->setLayout(new QHBoxLayout);
    ui->printTab->layout()->addWidget(_printerWidget);

    //setLayout();

//    _fileWidgetPtr = new FileListWiget(ui->fileTab);
//    ui->fileTab->setLayout(new QGridLayout());
//    ui->fileTab->layout()->addWidget(_fileWidgetPtr);
//    _fileWidgetPtr->show();

}

StudentHelperWiget::~StudentHelperWiget()
{
    delete ui;
}

void StudentHelperWiget::setStudentHelper(StudentHelper *stHelperPtr)
{
    _stHelperPtr = stHelperPtr;
    _browserWidget = new FileBrowserWidget(_stHelperPtr->getRootFolder());
    ui->fileTab->layout()->addWidget(_browserWidget);
}

StudentHelper *StudentHelperWiget::getStudentHelper()
{
    return _stHelperPtr;
}


