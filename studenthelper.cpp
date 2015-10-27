#include <QtCore>
#include "studenthelper.h"
#include "filetreeitem.h"


// //// Inplementation of StudentHelper

StudentHelper::StudentHelper(QObject *parPtr)
    : QObject(parPtr)
{
    //_fileModel = new FileModel(this);

    try
    {/*
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

        FolderItem *folder =*/ //new FolderItem("folder1", _rootFolderPtr);
//        new FileItem("file1", _rootFolderPtr);
//        FolderItem *folder2 = new FolderItem("folder2", _rootFolderPtr);
//        new FileItem("file2", _rootFolderPtr);
//        new FileItem("file3", _rootFolderPtr);
//        new FileItem("file2_1", folder2);

        //_rootFolderPtr->debbugOutput();
        _rootFolderPtr = new FolderItem("root folder");

        FolderItem* math_folder = new FolderItem("Math", _rootFolderPtr);
        FolderItem* geo_folder = new FolderItem("Geography", _rootFolderPtr);
        FolderItem* pc_folder = new FolderItem("PC Architecture", _rootFolderPtr);

        File* rus_map = new File("RussiaMap.jpg");
        File* usa_map = new File("UsaMap.png");
        File* lap_file = new File("LaplasEq.jpg");
        File* pca_file = new File("PCArchitecture.jpg");

        rus_map->addTag("Map");
        rus_map->addTag("Russia");
        usa_map->addTag("Map");
        usa_map->addTag("USA");
        lap_file->addTag("Diff Equations");
        lap_file->addTag("Laplas");
        pca_file->addTag("Computer Science");
        pca_file->addTag("PC Architecture");
        pca_file->addTag("PC Scheme");

        addFile(rus_map, geo_folder);
        addFile(usa_map, geo_folder);
        addFile(lap_file, math_folder);
        addFile(pca_file, pc_folder);
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
    {
        folderPtr->addChild(new FileItem(filePtr));
//        filePtr->addTheme(folderPtr->getName());
    }
    else
        _rootFolderPtr->addChild(new FileItem(filePtr));
}

File* StudentHelper::findFileByName(const QString& name)
{
    QList<File*>::iterator it = _fileList.begin();
    for( ; it != _fileList.end(); ++it)
    {
        if((*it)->getName() == name)
        {
            return *it;
        }
    }
    return NULL;
}

QList<File*> *StudentHelper::getFileListPtr()
{
    return &_fileList;
}

//QList<File*>* StudentHelper::getPrintQueuePtr()
//{
//    return &_printQueue;
//}

//void StudentHelper::addToPrintQueue(File* filePtr)
//{
//    if(_printQueue.contains(filePtr))
//        return;
//    _printQueue.push_back(filePtr);
//    emit printQueueChanged(filePtr,true);
//}

//void StudentHelper::deleteFromPrintQueue(File* filePtr)
//{
//    if( _printQueue.removeOne(filePtr) )
//    {
//        emit printQueueChanged(filePtr,false);
//    }
//}


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
