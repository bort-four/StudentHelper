#include <QtCore>
#include "studenthelper.h"
#include "filetreeitem.h"


// //// Inplementation of StudentHelper

StudentHelper::StudentHelper(QObject *parPtr)
    : QObject(parPtr)
{
    //_fileModel = new FileModel(this);

    try
    {
        _rootFolderPtr = new FolderItem("root folder");

        addFile(new File("test_file1"));
        addFile(new File("test_file2"));

        FolderItem* folder1 = new FolderItem("test_folder1", _rootFolderPtr);

        addFile(new File("test_file3"));

        FolderItem* folder2 = new FolderItem("folder2", folder1);
        FolderItem* folder3 = new FolderItem("folder3", folder2);
        addFile(new File("test_file4"), folder2);
        addFile(new File("test_file5"), folder3);

        File* filePtr = new File("file_6");
        filePtr->addTag("tag1");
        filePtr->addTag("tag2");
        filePtr->addTag("tag3");
        addFile(filePtr);

        /*FolderItem *folder =*/ //new FolderItem("folder1", _rootFolderPtr);
//        new FileItem("file1", _rootFolderPtr);
//        FolderItem *folder2 = new FolderItem("folder2", _rootFolderPtr);
//        new FileItem("file2", _rootFolderPtr);
//        new FileItem("file3", _rootFolderPtr);
//        new FileItem("file2_1", folder2);

        //_rootFolderPtr->debbugOutput();
    }
    catch (QString str)
    {
        qDebug() << str;
    }

}

StudentHelper::~StudentHelper()
{
    for (int i = 0; i < _fileList.count(); ++i)
        delete _fileList[i];
}

FolderItem *StudentHelper::getRootFolder()
{
    return _rootFolderPtr;
}

const FolderItem *StudentHelper::getRootFolder() const
{
    return _rootFolderPtr;
}

void StudentHelper::addFile(File *filePtr, FolderItem *folderPtr)
{
    _fileList.append(filePtr);

    if (folderPtr != NULL)
        folderPtr->addChild(new FileItem(filePtr));
    else
        _rootFolderPtr->addChild(new FileItem(filePtr));
}

QList<File*> *StudentHelper::getFileListPtr()
{
    return &_fileList;
}


/*
StudentHelper::PageId StudentHelper::getPageId() const
{
    return _currentPageId;
}

bool StudentHelper::setPage(StudentHelper::PageId id)
{
    _currentPageId = id;
    return true;
}
*/
