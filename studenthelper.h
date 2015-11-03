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
    File* findFileByName(const QString& name);
    void addToPrintQueue(File* filePtr);
    void deleteFromPrintQueue(File* filePtr);

    void saveSettings();
    void restoreSettings();

public slots:
    void onFileAdded(File *filePtr);
    void onFileRemoved(File *filePtr);

signals:
    void printQueueChanged(File*,bool);

private:
    void writeFolderSettings(QSettings &settings, FolderItem *folderPtr);
    void readFolderSettings(QSettings &settings, FolderItem *folderPtr);

    QList<File*> _fileList;
    FolderItem*  _rootFolderPtr;
    QList<File*> _printQueue;
};




#endif // STUDENTHELPER_H
