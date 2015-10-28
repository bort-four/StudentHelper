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

    void addFile(File* filePtr, FolderItem* folderPtr = NULL);
    File* findFileByName(const QString& name);

    // setters
    //bool setPage(PageId id);
signals:
    void printQueueChanged(File*,bool);

private:
    QList<File*> _fileList;
    FolderItem*  _rootFolderPtr;
};




#endif // STUDENTHELPER_H
