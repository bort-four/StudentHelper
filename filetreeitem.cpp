#include "filetreeitem.h"

// //// Implementation of File

File::File(QString name)
    : _name(name), _isSelectedToPrint(false), _pixMapPtr(NULL)
{
    _pixMapPtr = new QPixmap(name);
}

File::~File()
{
    delete _pixMapPtr;
}

QString File::getName() const
{
    return _name;
}

const QStringList *File::getTagListPtr() const
{
    return &_tags;
}

const QStringList &File::getTags() const
{
    return _tags;
}

bool File::isSelectedToPrint() const
{
    return _isSelectedToPrint;
}

const QPixmap *File::getImage() const
{
    return _pixMapPtr;
}


QString File::getTagString() const
{
    QString str;

    for (int i = 0; i < _tags.count(); ++i)
    {
        str += _tags.at(i);

        if (i < _tags.count() - 1)
            str += ", ";
    }

    return str;
}

void File::inputTagsFromString(const QString &tagString)
{
    QStringList tags = tagString.split(',');

    _tags.clear();

    foreach (QString tag, tags)
    {
        QString newTag = tag.trimmed();

        if (newTag != "")
            _tags.append(newTag);
    }
}


void File::addTag(QString tag)
{
    _tags.append(tag);
}

void File::setSelectedToPrint(bool selection)
{
    _isSelectedToPrint = selection;
}





// //// Implementation of FileTreeItem

FileTreeItem::FileTreeItem(QObject *parPtr)
    : QObject(parPtr)
{
    if (dynamic_cast<FileTreeItem*>(parPtr))
        dynamic_cast<FileTreeItem*>(parPtr)->addChild(this);
}


FolderItem *FileTreeItem::toFolder()
{
    throw QString("Invalid cast to FolderItem*");
}

FileItem *FileTreeItem::toFile()
{
    throw QString("Invalid cast to FileItem*");
}

QString FileTreeItem::getName() { return objectName(); }

bool FileTreeItem::setName(QString newName) { setObjectName(newName); return true; }

QVariant FileTreeItem::getValue() const { return QVariant(); }

bool FileTreeItem::setValue(QVariant) { return false; }

FileTreeItem *FileTreeItem::getChild(int) { throw QString("Try to get child from not a folder"); }

FolderItem *FileTreeItem::getParent() { return dynamic_cast<FolderItem*>(parent()); }

int FileTreeItem::getChilCount() const { return 0; }

bool FileTreeItem::addChild(FileTreeItem *) { throw QString("Try to add child into not a folder"); }

bool FileTreeItem::removeChild(FileTreeItem *) { throw QString("Try to remove child from not a folder"); }


void FileTreeItem::debbugOutput(int space)
{
    QString str;

    for (int i = 0; i < space; i++)
        str += "  ";

    if (isFolder())
        str += "[folder] ";

    str += getName();

    qDebug() << str;

    for (int i = 0; i < getChilCount(); ++i)
        getChild(i)->debbugOutput(space + 1);
}



// //// Implementation of FolderItem

FolderItem::FolderItem(QString name, QObject *parPtr) : FileTreeItem(parPtr)
{
    setObjectName(name);
}

FolderItem::~FolderItem() {}

bool FolderItem::isFolder() const { return true; }

FolderItem *FolderItem::toFolder()
{
    return this;
}

FileTreeItem *FolderItem::getChild(int num) { return dynamic_cast<FileTreeItem*>(children().at(num)); }

int FolderItem::getChilCount() const { return children().size(); }

bool FolderItem::addChild(FileTreeItem *childPtr) { childPtr->setParent(this); return true; }

bool FolderItem::removeChild(FileTreeItem *childPtr) { childPtr->setParent(NULL); return true; }


QString FolderItem::getPath()
{
    if (getParent() == NULL)
        return "";

    QString str = getName();
    FolderItem* folderPtr = getParent();

    while (folderPtr != NULL)
    {
        if (folderPtr->getParent() != NULL)
            str = folderPtr->getName() + "/" + str;

        folderPtr = folderPtr->getParent();
    }

    return str;
}


// //// Implementation of FileItem

FileItem::FileItem(File *filePtr, QObject *parPtr) : FileTreeItem(parPtr)
{
    _filePtr = filePtr;
    //_isSelected = false;
    //setObjectName(name);
}

FileItem::~FileItem() {}

bool FileItem::isFile() const { return true; }

FileItem *FileItem::toFile()
{
    return this;
}

QString FileItem::getName()
{
    return getFilePtr()->getName();
}

File *FileItem::getFilePtr()
{
    return _filePtr;
}

/*
bool FileItem::isSelected() const { return _isSelected; }

bool FileItem::setSelection(bool selection) { return _isSelected = selection; }

const QStringList &FileItem::getTags() const { return _tagList; }

QStringList &FileItem::getTags() { return _tagList; }
*/
