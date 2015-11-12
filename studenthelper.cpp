#include <QtCore>
#include "studenthelper.h"
#include "filetreeitem.h"


// //// Inplementation of StudentHelper

StudentHelper::StudentHelper(QObject *parPtr)
    : QObject(parPtr), _rootFolderPtr(new FolderItem("root folder"))
{
    connect(_rootFolderPtr, SIGNAL(fileAdded(File*)),
            this,           SLOT(onFileAdded(File*)));

    connect(_rootFolderPtr, SIGNAL(fileRemoved(File*)),
            this,           SLOT(onFileRemoved(File*)), Qt::QueuedConnection);

    try
    {
        restoreSettings();

        /*
        FolderItem* math_folder = new FolderItem("Math", _rootFolderPtr);
        FolderItem* geo_folder = new FolderItem("Geography", _rootFolderPtr);
        FolderItem* pc_folder = new FolderItem("PC Architecture", _rootFolderPtr);

        File* rus_map = new File("RussiaMap.jpg");
        File* usa_map = new File("UsaMap.png");
        File* lap_file = new File("LaplasEq.jpg");
        File* pca_file = new File("PCArchitecture.jpg");

        rus_map->inputTagsFromString("Map, Russia");
        usa_map->inputTagsFromString("Map, USA");
        lap_file->inputTagsFromString("Diff Equations, Laplas");
        pca_file->inputTagsFromString("Computer Science, PC Architecture, PC Scheme");

        addFile(rus_map, geo_folder);
        addFile(usa_map, geo_folder);
        addFile(lap_file, math_folder);
        addFile(pca_file, pc_folder);
        */
    }
    catch (QString str)
    {
        qDebug() << str;
    }

}

StudentHelper::~StudentHelper()
{
    saveSettings();

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
//<<<<<<< HEAD
//    _fileList.append(filePtr);
//=======
//    _fileList.append(filePtr);
//    connect(filePtr, SIGNAL(selectionChanged(File*,bool)), this, SIGNAL(printQueueChanged(File*,bool)));
//>>>>>>> 676d0475f8c7a080ace673a536fd900e2ee1ae4d

    if (folderPtr != NULL)
    {
        folderPtr->addChild(new FileItem(filePtr));
//        filePtr->addTheme(folderPtr->getName());
    }
    else
        _rootFolderPtr->addChild(new FileItem(filePtr));
}

File* StudentHelper::findFileByPath(const QString& path)
{
    QList<File*>::iterator it = _fileList.begin();
    for( ; it != _fileList.end(); ++it)
    {
        if((*it)->getFullName() == path)
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


void StudentHelper::saveSettings()
{
    QSettings settings("MMCS","StudentHelper");
    settings.clear();

//    return;

    settings.setValue("globalFileListCount", _fileList.size());
    settings.beginWriteArray("globalFileList");

    for (int i = 0; i < _fileList.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("fullName", _fileList[i]->getFullName());
        settings.setValue("tagString", _fileList[i]->getTagString());
    }

    settings.endArray();

    writeFolderSettings(settings, _rootFolderPtr);
}


void StudentHelper::restoreSettings()
{
    QSettings settings("MMCS","StudentHelper");

    int fileCount = settings.value("globalFileListCount", 0).toInt();
    _fileList.clear();

    settings.beginReadArray("globalFileList");

    for (int i = 0; i < fileCount; ++i)
    {
        settings.setArrayIndex(i);
        QString fileFullName = settings.value("fullName", "___").toString();
        File *filePtr = new File(fileFullName);
        filePtr->inputTagsFromString(settings.value("tagString", "").toString());

        _fileList.append(filePtr);
    }

    settings.endArray();

    readFolderSettings(settings, _rootFolderPtr);

    foreach (File *filePtr, _fileList)
        if (filePtr->getLinkCount() == 0)
            _fileList.removeAll(filePtr);
}


void StudentHelper::writeFolderSettings(QSettings &settings, FolderItem *folderPtr)
{
    // write child folders
    settings.setValue("folderCount", folderPtr->getChildFolderCount());
    settings.beginWriteArray("folders");

    for (int i = 0; i < folderPtr->getChildFolderCount(); ++i)
    {
        settings.setArrayIndex(i);
        FolderItem *childFolderPtr = folderPtr->getChild(i)->toFolder();

        if (childFolderPtr == NULL)
            throw QString("StudentHelper::writeFolderSettings(): invalid child folder");

        settings.setValue("folderName", childFolderPtr->getName());

        writeFolderSettings(settings, childFolderPtr);
    }
    settings.endArray();

    // write child files
    settings.setValue("fileCount", folderPtr->getChildCount() - folderPtr->getChildFolderCount());
    settings.beginWriteArray("files");

    for (int i = folderPtr->getChildFolderCount(); i < folderPtr->getChildCount(); ++i)
    {
        settings.setArrayIndex(i - folderPtr->getChildFolderCount());
        FileItem *childFilePtr = folderPtr->getChild(i)->toFile();

        if (childFilePtr == NULL)
            throw QString("StudentHelper::writeFolderSettings(): invalid child file");

        File *filePtr = childFilePtr->getFilePtr();
        int numInGlobalList = _fileList.indexOf(filePtr);

        if (numInGlobalList == -1)
            throw QString("StudentHelper::writeFolderSettings(): can't find file in global list");

        settings.setValue("numInGlobalList", numInGlobalList);
    }
    settings.endArray();
}

void StudentHelper::readFolderSettings(QSettings &settings, FolderItem *folderPtr)
{
    // read child folders
    int folderCount = settings.value("folderCount", 0).toInt();
    settings.beginReadArray("folders");

    for (int i = 0; i < folderCount; ++i)
    {
        settings.setArrayIndex(i);
        QString folderName = settings.value("folderName", "[]").toString();
        FolderItem *childFolderPtr = new FolderItem(folderName, folderPtr);

        readFolderSettings(settings, childFolderPtr);
    }
    settings.endArray();

    // read child files
    int fileCount = settings.value("fileCount", 0).toInt();
    settings.beginReadArray("files");

    for (int i = 0; i < fileCount; ++i)
    {
        settings.setArrayIndex(i);
        int numInGlobalList = settings.value("numInGlobalList", -1).toInt();

        if (numInGlobalList == -1)
            throw QString("StudentHelper::readFolderSettings(): invalid file num");

        File *filePtr = _fileList.at(numInGlobalList);
        new FileItem(filePtr, folderPtr);
    }
    settings.endArray();
}


void StudentHelper::onFileAdded(File *filePtr)
{
    if (_fileList.indexOf(filePtr) == -1)
        _fileList.append(filePtr);

    connect(filePtr, SIGNAL(selectionChanged(File*,bool)), this, SIGNAL(printQueueChanged(File*,bool)));

//    else
//        filePtr->setLinkCount(filePtr->getLinkCount() + 1);
}

void StudentHelper::onFileRemoved(File *filePtr)
{
//    filePtr->setLinkCount(filePtr->getLinkCount() - 1);

    int linksCount = filePtr->getLinkCount();

    if (linksCount == 0)
    {
//        qDebug() << "file " << filePtr->getName() << " removed";
        _fileList.removeAll(filePtr);
    }
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
