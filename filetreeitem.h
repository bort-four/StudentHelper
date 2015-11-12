#ifndef FILETREEITEM_H
#define FILETREEITEM_H

#include <QtCore>
#include <QPixmap>

class FolderItem;
class FileItem;


class File : public QObject
{
    Q_OBJECT

public:
    File(QString fullFileName);
    ~File();

    QString getName() const;
    QString getFullName() const;
    const QStringList* getTagListPtr() const;
    const QStringList &getTags() const;
    bool isSelectedToPrint() const; // not actual
    const QPixmap* getImage() const;

    QString getTagString() const;
    void inputTagsFromString(const QString &tagString);

    void addTag(QString tag);
    void setSelectedToPrint(bool selection);    // not actual

    int getLinkCount() const;
    void setLinkCount(int newCount);

signals:
    void tagsChenged();
    void selectionChenged(bool);
    void selectionChanged(File*,bool);

private:
    QString _fullName, _name;
    QStringList _tags;
    bool _isSelectedToPrint;
    QPixmap* _pixMapPtr;
    int _linkCount;
};



class FileTreeItem : public QObject
{
    Q_OBJECT

public:

    virtual ~FileTreeItem() {}

    virtual bool isFile() const { return false; }
    virtual bool isFolder() const { return false; }

    virtual FolderItem* toFolder();
    virtual FileItem* toFile();

    virtual QString getName() const;

    virtual FileTreeItem* getChild(int num);
    virtual const FileTreeItem* getChild(int num) const;
    virtual FolderItem* getParent();
    virtual int getChildCount() const;

    virtual bool addChild(FileTreeItem* childPtr);
    virtual bool removeChild(FileTreeItem* childPtr);

    virtual void debbugOutput(int space = 0);

    /*
    enum SelectionState{
        NOT_SELECTED = Qt::Unchecked,
        SELECTED = Qt::Checked,
        PARTIALLY_SELECTED = Qt::PartiallyChecked
    };
    */

//    virtual SelectionState getSelectionState() const;

//signals:
//    void selectionStateCnahged(FileTreeItem::SelectionState);

protected:
    FileTreeItem(QObject* parPtr = 0);
};



class FolderItem : public FileTreeItem
{
    Q_OBJECT

public:
    FolderItem(QString name, QObject* parPtr = 0);

    virtual ~FolderItem();

    virtual bool isFolder() const;
    virtual FolderItem* toFolder();

    virtual FileTreeItem* getChild(int num);
    virtual const FileTreeItem* getChild(int num) const;
    virtual int getChildCount() const;

    virtual bool addChild(FileTreeItem* childPtr);
    virtual bool removeChild(FileTreeItem* childPtr);

    virtual QString getName() const;
    void setName(QString newName);

    int getChildFolderCount() const;

//    void setSelectionRecursive(bool selection);

//public slots:
//    void onChildCelectionStateChanged(SelectionState state);

signals:
    void nameChanged(QString);
    void structureChanged();
    void fileAdded(File *);
    void fileRemoved(File *);

private:
    bool removeChildRecursive(FileTreeItem* childPtr);

    QList<FolderItem *> _folders;
    QList<FileItem *> _files;
};



class FileItem : public FileTreeItem
{
    Q_OBJECT

public:
    FileItem(File* filePtr, QObject* parPtr = 0);

    virtual ~FileItem();

    virtual bool isFile() const;

    virtual FileItem* toFile();

    virtual QString getName();
    File* getFilePtr();
    const File* getFilePtr() const;

//    virtual SelectionState getSelectionState() const;

//public slots:
//    void onFileSelectionChanged(bool isSelected);

signals:
    void fileTagsChanged();

private:
    File* _filePtr;
};



#endif // FILETREEITEM_H
