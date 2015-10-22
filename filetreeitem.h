#ifndef FILETREEITEM_H
#define FILETREEITEM_H

#include <QtCore>
#include <QPixmap>

class FolderItem;
class FileItem;


class File
{
public:
    File(QString name = "нет имени");
    ~File();

    QString getName() const;
    const QStringList* getTagListPtr() const;
    bool isSelectedToPrint() const;
    const QPixmap* getImage() const;

    void addTag(QString tag);
    void setSelectedToPrint(bool selection);

private:
    QString _name;
    QStringList _tags;
    bool _isSelectedToPrint;
    QPixmap* _pixMapPtr;


// //// TODO: remove theme
public:
    QStringList* getThemesListPtr() { return &_themes; }
    void addTheme(const QString& new_theme) { _themes.push_back(new_theme); }
private:
    QStringList _themes;
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

    virtual QString getName();
    virtual bool setName(QString newName);

    virtual QVariant getValue() const;
    virtual bool setValue(QVariant value);

    virtual FileTreeItem* getChild(int num);
    virtual FolderItem* getParent();
    virtual int getChilCount() const;

    virtual bool addChild(FileTreeItem* childPtr);
    virtual bool removeChild(FileTreeItem* childPtr);

    virtual void debbugOutput(int space = 0);

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
    virtual int getChilCount() const;

    virtual bool addChild(FileTreeItem* childPtr);
    virtual bool removeChild(FileTreeItem* childPtr);

    virtual QString getPath();
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

private:
    File* _filePtr;
};



#endif // FILETREEITEM_H
