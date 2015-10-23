#ifndef STUDENTHELPER_H
#define STUDENTHELPER_H

#include <QAbstractItemModel>
#include <QException>
#include "filetreeitem.h"


class StudentHelper : public QObject
{
    Q_OBJECT

public:
    StudentHelper(QObject* parPtr = 0);
    ~StudentHelper();

    //enum PageId {FILE_PAGE, SEARCH_PAGE, PRINT_PAGE};

    // getters
    FolderItem* getRootFolder();
    const FolderItem* getRootFolder() const;

    QList<File*>* getFileListPtr();
    QList<File*>* getPrintQueuePtr();

    void addFile(File* filePtr, FolderItem* folderPtr = NULL);
    void addToPrintQueue(File* filePtr);
    void deleteFromPrintQueue(File* filePtr);

    // setters
    //bool setPage(PageId id);
signals:
    printQueueChanged(File*,bool);

private:
    QList<File*> _fileList;
    FolderItem*  _rootFolderPtr;
    QList<File*> _printQueue;
};




#endif // STUDENTHELPER_H
