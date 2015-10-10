#include "filebrowserwidget.h"
#include "ui_filebrowserwidget.h"
#include <QSpacerItem>


FileBrowserWidget::FileBrowserWidget(FolderItem* rootFolderPtr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileBrowserWidget),
    _rootFolderPtr(NULL), _currFolderPtr(NULL)
{
    ui->setupUi(this);
    setRootFolder(rootFolderPtr);
}

FileBrowserWidget::~FileBrowserWidget()
{
    delete ui;
}


FolderItem *FileBrowserWidget::getRootFolder()
{
    return _rootFolderPtr;
}

FolderItem *FileBrowserWidget::getCurrFolder()
{
    return _currFolderPtr;
}

void FileBrowserWidget::setRootFolder(FolderItem *folderPtr)
{
    _rootFolderPtr = folderPtr;
    setCurrFolder(folderPtr);
}

void FileBrowserWidget::setCurrFolder(FolderItem *folderPtr)
{
    if (_currFolderPtr == folderPtr)
        return;

    _currFolderPtr = folderPtr;

    QLayout* layoutPtr = ui->scrollAreaWidgetContents->layout();

    // remove old widgets
    while (layoutPtr->count() > 0)
    {
        QLayoutItem* itemPtr = layoutPtr->takeAt(0);

        if (itemPtr->widget())
            delete itemPtr->widget();

        delete itemPtr;
    }

    if (folderPtr == NULL) return;

    // create new widgets
    if (_currFolderPtr != _rootFolderPtr)
    {
        QPushButton* backButton = new QPushButton("..", this);
        layoutPtr->addWidget(backButton);
        connect(backButton, SIGNAL(pressed()), this, SLOT(onBackPressed()));
    }

    for (int itemNum = 0; itemNum < _currFolderPtr->getChilCount(); ++itemNum)
    {
        FileTreeItem* itemPtr = _currFolderPtr->getChild(itemNum);

        if (itemPtr->isFolder())
        {
            FolderWiget* folderPtr = new FolderWiget(itemPtr->toFolder());
            layoutPtr->addWidget(folderPtr);

            connect(folderPtr, SIGNAL(pressed()), this, SLOT(onFolderPress()));
        }
        else if (itemPtr->isFile())
            layoutPtr->addWidget(new FileWiget(itemPtr->toFile()));
    }

    layoutPtr->addItem(new QSpacerItem(1, 1,
                                       QSizePolicy::Expanding,
                                       QSizePolicy::Expanding));

    ui->pathLabel->setText(_currFolderPtr->getPath());
}


void FileBrowserWidget::onFolderPress()
{
    FolderWiget* folderPtr = dynamic_cast<FolderWiget*>(sender());

    if (folderPtr == NULL) return;

    setCurrFolder(folderPtr->getFolderPtr());
}

void FileBrowserWidget::onBackPressed()
{
    if (_currFolderPtr->getParent() != NULL && _currFolderPtr != _rootFolderPtr)
        setCurrFolder(_currFolderPtr->getParent());
}





