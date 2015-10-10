#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QGridLayout>
#include "filetreeitem.h"


namespace Ui {
class FileBrowserWidget;
}


class FileBrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWidget(FolderItem* rootFolderPtr = NULL, QWidget *parent = 0);
    ~FileBrowserWidget();

    FolderItem* getRootFolder();
    FolderItem* getCurrFolder();

    void setRootFolder(FolderItem* folderPtr);
    void setCurrFolder(FolderItem* folderPtr);

private:
    Ui::FileBrowserWidget *ui;
    FolderItem *_rootFolderPtr,
               *_currFolderPtr;

public slots:
    void onFolderPress();
    void onBackPressed();
};



class FileWiget : public QFrame
{
    Q_OBJECT

public:
    explicit FileWiget(FileItem* itemPtr, QWidget *parent = 0)
        : QFrame(parent), _itemPtr(itemPtr)
    {
        setFrameShape(QFrame::StyledPanel);

        QString name = _itemPtr->getName() + " ";
        QStringList* tags = itemPtr->getFilePtr()->getTagListPtr();

        for (int i = 0; i < tags->count(); ++i)
            name += " [" + tags->at(i) + "]";

        setLayout(new QGridLayout(this));
        layout()->addWidget(new QLabel(name, this));
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }

    ~FileWiget() {}

    FileItem* getFilePtr() { return _itemPtr; }

private:
    FileItem* _itemPtr;
};



class FolderWiget : public QPushButton
{
    Q_OBJECT

public:
    explicit FolderWiget(FolderItem* itemPtr, QWidget *parent = 0)
        : QPushButton(itemPtr->getName(), parent)
        , _itemPtr(itemPtr)
    {}

    ~FolderWiget() {}

    FolderItem* getFolderPtr() { return _itemPtr; }

private:
    FolderItem* _itemPtr;
};


#endif // FILEBROWSERWIDGET_H
