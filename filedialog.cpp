#include "filedialog.h"
#include "ui_filedialog.h"

FileDialog::FileDialog(FolderItem* rootFolderPtr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);
    _browserPtr = new FileBrowserWidget(rootFolderPtr, this);
    _browserPtr->setEdittingEnabled(false);
    _browserPtr->setSelectionEnabled(false);
    ui->centralLayout->addWidget(_browserPtr);

    connect(_browserPtr,    SIGNAL(currFileWidgetChanged(FileWiget*)),
            this,           SLOT(onCurrFileWidgetChanged(FileWiget*)));

    onCurrFileWidgetChanged(_browserPtr->getCurrFileWidget());
}

FileDialog::~FileDialog()
{
    delete ui;
}

File *FileDialog::getFilePtr()
{
    FileWiget *wgPtr = _browserPtr->getCurrFileWidget();

    if (wgPtr == NULL)
        return NULL;

    return wgPtr->getFilePtr();
}

void FileDialog::onCurrFileWidgetChanged(FileWiget */*wgPtr*/)
{
    ui->chooseButton->setEnabled(getFilePtr() != NULL);
}

void FileDialog::on_cancelButton_clicked()
{
    close();
}

void FileDialog::on_chooseButton_clicked()
{
    accept();
}
