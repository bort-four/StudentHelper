#include "filebrowserwidget.h"
#include "ui_filebrowserwidget.h"
#include "ui_filewidget.h"
#include <QSpacerItem>
#include <QToolButton>


// //// Service functions

void clearLayout(QLayout *layoutPtr)
{
    while (layoutPtr->itemAt(0) != NULL)
    {
        QLayoutItem* item = layoutPtr->itemAt(0);

        if (item->widget())
            delete item->widget();
        else if (item->layout())
            delete item->layout();
    }
}



void setMouseTrackingRecursive(QWidget *widgetPtr, bool value)
{
    widgetPtr->setMouseTracking(value);

    foreach (QObject* objPtr, widgetPtr->children()) {
        if (dynamic_cast<QWidget*>(objPtr))
            setMouseTrackingRecursive(dynamic_cast<QWidget*>(objPtr), value);
    }
}

// /////////////////////////


FileBrowserWidget::FileBrowserWidget(FolderItem* rootFolderPtr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileBrowserWidget),
    _rootFolderPtr(NULL), _currFolderPtr(NULL), _currFileWgPtr(NULL)
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

    QLayout* layoutPtr = ui->itemLayout;

    clearLayout(layoutPtr);

    if (folderPtr == NULL) return;

    // create new widgets
    /*
    if (_currFolderPtr != _rootFolderPtr)
    {
        QPushButton* backButton = new QPushButton("..", this);
        layoutPtr->addWidget(backButton);
        connect(backButton, SIGNAL(pressed()), this, SLOT(onBackPressed()));
    }
    */

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
        {
            FileWiget* fileWgPtr = new FileWiget(itemPtr->toFile());
            layoutPtr->addWidget(fileWgPtr);

            connect(fileWgPtr,  SIGNAL(opened()),
                    this,       SLOT(onFileOpened()));
        }
    }

    _currFileWgPtr = NULL;

    // generate path
    clearLayout(ui->pathLayout);
    FolderItem* folderPtr2 = _currFolderPtr;

    for (; folderPtr2 != NULL; folderPtr2 = folderPtr2->getParent())
    {
        QToolButton* buttonPtr = new QToolButton();
        buttonPtr->setText(folderPtr2->getName());

        connect(buttonPtr,  SIGNAL(clicked()),
                this,       SLOT(onPathPressed()));

        ui->pathLayout->insertWidget(0, buttonPtr);
    }
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

void FileBrowserWidget::onFileOpened()
{
    FileWiget* newFileWgPtr = dynamic_cast<FileWiget*>(sender());

    if (_currFileWgPtr != NULL && _currFileWgPtr != newFileWgPtr)
        _currFileWgPtr->close();

    _currFileWgPtr = newFileWgPtr;
}

void FileBrowserWidget::onPathPressed()
{
    QToolButton* senderPtr = dynamic_cast<QToolButton*>(sender());

    if (senderPtr == NULL) return;

    int parNum = ui->pathLayout->count() - 1 - ui->pathLayout->indexOf(senderPtr);
    FolderItem *newFolderPtr = _currFolderPtr;

    for (int i = 0; i < parNum; ++i)
        newFolderPtr = newFolderPtr->getParent();

    setCurrFolder(newFolderPtr);
}




// //// FileWiget

FileWiget::FileWiget(FileItem *itemPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::FileWidget), _itemPtr(itemPtr), _isOpen(false)
{
    ui->setupUi(this);
    ui->nameLabel->setText(_itemPtr->getName());
    ui->bottomGroup->setHidden(true);

    const QStringList* tags = itemPtr->getFilePtr()->getTagListPtr();

    for (int i = 0; i < tags->count(); ++i)
    {
        QToolButton* tagButtonPtr = new QToolButton();
        tagButtonPtr->setText(tags->at(i));
        ui->tagLayout->addWidget(tagButtonPtr);
    }

    ui->imageLabel->setPixmap(*itemPtr->getFilePtr()->getImage());
    ui->imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);

    //QPixmap miniPixmap(*itemPtr->getFilePtr()->getImage());
    //;
    //ui->miniatureLabel->setScaledContents(true);
    ui->miniatureLabel->setPixmap(itemPtr->getFilePtr()->getImage()
                                  ->scaledToHeight(ui->headGroup->height() * 1.4,
                                                   Qt::SmoothTransformation));
    //ui->miniatureLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    setMouseTrackingRecursive(this, true);
}

void FileWiget::open()
{
    ui->bottomGroup->setHidden(false);
    ui->miniatureLabel->setHidden(true);

    _isOpen = true;
    emit opened();
}

void FileWiget::close()
{
    ui->bottomGroup->setHidden(true);
    ui->miniatureLabel->setHidden(false);

    _isOpen = false;
    emit closed();
}


void FileWiget::leaveEvent(QEvent *)
{
    if (_isHeadUnderCursor)
        QApplication::restoreOverrideCursor();

    _isHeadUnderCursor = false;
}

void FileWiget::mousePressEvent(QMouseEvent *)
{
    if (!_isHeadUnderCursor) return;

    if (!_isOpen)
         open();
    else close();
}

void FileWiget::mouseMoveEvent(QMouseEvent *)
{
    bool lastValue = _isHeadUnderCursor;

    _isHeadUnderCursor = ui->headGroup->rect()
            .contains(ui->headGroup->mapFromGlobal(QCursor::pos()));

    if (!lastValue && _isHeadUnderCursor)
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
    else if (lastValue && !_isHeadUnderCursor)
        QApplication::restoreOverrideCursor();
}


// //// FolderWiget


FolderWiget::FolderWiget(FolderItem *itemPtr, QWidget *parent)
    : QLabel(itemPtr->getName(), parent), _itemPtr(itemPtr)
{
    setFrameShape(QFrame::StyledPanel);
    setMargin(2);
    setMouseTracking(true);
    //new QLabel(itemPtr->getName(), this);
}


void FolderWiget::leaveEvent(QEvent *)
{
    QApplication::restoreOverrideCursor();
}

void FolderWiget::enterEvent(QEvent *)
{
    QApplication::setOverrideCursor(Qt::PointingHandCursor);
}

void FolderWiget::mousePressEvent(QMouseEvent *)
{
    QApplication::restoreOverrideCursor();
    emit pressed();
}




