#include "studenthelperwiget.h"
#include "ui_studenthelperwiget.h"

#include <QtCore>
#include <QStandardItemModel>
#include <QLabel>
#include "filebrowserwidget.h"
#include "printcompositor.h"


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

    connect(_browserWidget, SIGNAL(printRequested(File*)), _stHelperPtr,    SIGNAL(sendToPrint(File*))   );
    connect(_browserWidget, SIGNAL(tagClicked(QString)),   _searcherWidget, SLOT(tagSearchInit(QString)) );
    connect(_browserWidget, SIGNAL(tagClicked(QString)),    this,           SLOT(openSearchTab()) );

    /*
    // PrintCompositor example
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("/home/bort-4/image.pdf");

    PrintCompositor compositor(printer);

    for (auto x : *getStudentHelper()->getFileListPtr())
        compositor.addPixmap(*x->getImage());

    compositor.composite();
    */
}

StudentHelperWiget::~StudentHelperWiget()
{
    delete ui;
}

void StudentHelperWiget::setStudentHelper(StudentHelper *stHelperPtr)
{
    _stHelperPtr = stHelperPtr;
    _browserWidget = new FileBrowserWidget(_stHelperPtr->getRootFolder());
    _browserWidget->setEdittingEnabled(true);
    _browserWidget->setPrintEnabled(true);
    ui->fileTab->layout()->addWidget(_browserWidget);

    /*
    connect(_browserWidget, SIGNAL(printRequested(File*)),
            this,           SLOT(onPrintRequested(File*)));

    connect(_browserWidget, SIGNAL(tagClicked(QString)),
            this,           SLOT(onTagClicked(QString)));
    */
}

StudentHelper *StudentHelperWiget::getStudentHelper()
{
    return _stHelperPtr;
}

void StudentHelperWiget::onPrintRequested(File *filePtr)
{
    qDebug() << "Requested print for file" << filePtr->getName();
}

void StudentHelperWiget::onTagClicked(QString tag)
{
    qDebug() << "tag clicked:" << tag;
}

void StudentHelperWiget::openSearchTab()
{
    ui->tabWidget->setCurrentWidget(ui->searchTab);
}


