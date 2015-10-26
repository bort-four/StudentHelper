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
class FileWidget;
}

class FileWiget;

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

public slots:
    void onFolderPress();
    void onBackPressed();
    void onFileOpened();
    void onPathPressed();

private:
    Ui::FileBrowserWidget *ui;
    FolderItem *_rootFolderPtr,
               *_currFolderPtr;
    FileWiget *_currFileWgPtr;
};



class FileWiget : public QWidget
{
    Q_OBJECT

public:
    explicit FileWiget(FileItem* itemPtr, QWidget *parent = 0);
    ~FileWiget() {}

    FileItem* getFileItemPtr() { return _itemPtr; }
    bool isOpen() const { return _isOpen; }

public slots:
    void open();
    void close();
    void updateControlsVisible();
    void togleTagsMode(bool enableEdit);
    void updateTags();
    void onTagEditingFinished();

    void onFileUpdated();

signals:
    void opened();
    void closed();

protected:
    virtual void leaveEvent(QEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

private slots:
    void on_printCheckBox_stateChanged(int arg1);
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_applyButton_clicked();

private:
    Ui::FileWidget *ui;
    FileItem* _itemPtr;
    bool _isHeadUnderCursor, _isOpen;
};


///*
class FolderWiget : public QLabel
{
    Q_OBJECT

public:
    explicit FolderWiget(FolderItem* itemPtr, QWidget *parent = 0);

    ~FolderWiget() {}

    FolderItem* getFolderPtr() { return _itemPtr; }

signals:
    void pressed();

protected:
    virtual void leaveEvent(QEvent *);
    virtual void enterEvent(QEvent *);
    virtual void mousePressEvent(QMouseEvent *);

private:
    FolderItem* _itemPtr;
};
//*/

/*
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
*/

#endif // FILEBROWSERWIDGET_H
