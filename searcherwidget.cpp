#include "searcherwidget.h"
#include "ui_searcherwidget.h"

#include "filebrowserwidget.h"

SearcherWidget::SearcherWidget(QWidget *parent, StudentHelper *hlpr) :
    QWidget(parent),
    ui(new Ui::SearcherWidget)
{
    ui->setupUi(this);

    helper_data = hlpr;
    searching_type = 0;
    browser = new FileBrowserWidget;
    ui->horizontalLayout_5->addWidget(browser);

    connect( ui->name_search_button,    SIGNAL(clicked(bool)),  this, SLOT(searchTypeSelected())        );
    connect( ui->tag_search_button,     SIGNAL(clicked(bool)),  this, SLOT(searchTypeSelected())        );
    connect( ui->find_button,           SIGNAL(clicked(bool)),  this, SLOT(baseSearching())             );
    connect( ui->local_find_button,     SIGNAL(clicked(bool)),  this, SLOT(localSearching())            );
    connect( ui->selectAllButton,       SIGNAL(clicked(bool)),  this, SLOT(selectAll())                 );

}

SearcherWidget::~SearcherWidget()
{
    delete ui;
}

void SearcherWidget::localSearching()
{
    if (temp_searching_results == NULL)
        return;
    if (temp_searching_results->empty())
        return;
    searchStart( *temp_searching_results );
}

void SearcherWidget::baseSearching()
{
    if (helper_data->getFileListPtr()->empty())
        return;
    searchStart( *helper_data->getFileListPtr() );
}

void SearcherWidget::searchStart(const QList<File*>& data)
{
    QString query_string = prepareQueryString();
    if( query_string.isEmpty() )
    {
        return;
    }
    QList<File*>* result = new QList<File*>;
    if (searching_type == 0)    // name searching
    {
        for(QList<File*>::const_iterator it = data.begin(); it != data.end(); ++it)
        {
            QString name = (*it)->getName().toLower();
            if (name.contains(query_string))
            {
                result->push_back(*it);
            }
        }
    }
    else if (searching_type == 1)   // tag searching
    {
        query_string.replace(", ", ",");
        QStringList tags = query_string.split(",");
        QList<File*> *res_list = new QList<File*>,
                     *data_list = new QList<File*>(data);
        for(QStringList::const_iterator it = tags.begin(); it != tags.end(); ++it)
        {
            const QString& tag_name = *it;
            for(QList<File*>::const_iterator itd = data_list->begin(); itd != data_list->end(); ++itd)
            {
                const QStringList& tag_list = *(*itd)->getTagListPtr();
                for(QStringList::const_iterator it2 = tag_list.begin(); it2 != tag_list.end(); ++it2)
                {
                    const QString& tag = it2->toLower();
                    if (tag.contains(tag_name))
                    {
                        res_list->push_back(*itd);
                        break;
                    }
                }
            }
            data_list = new QList<File*>(*res_list);
            res_list->clear();
        }
        result = data_list;
    }
    if( result->empty() )
    {
        return;
    }

    temp_searching_results = result;
    FolderItem* res_folder = new FolderItem("Результаты");
    for(int i = 0; i < result->size(); ++i)
    {
        res_folder->addChild(new FileItem(result->at(i)));
    }
    browser->setRootFolder(res_folder);
}

QString SearcherWidget::prepareQueryString()
{
    QString query = ui->query_text_line->text().toLower();
    while( query.startsWith(' ') )
        query.remove(0,1);
    while( query.endsWith(' ') )
        query.remove(query.length()-1,1);
    return query;
}

void SearcherWidget::searchTypeSelected()
{
    if (ui->name_search_button->isChecked())
    {
        searching_type = 0;
    }
    else
    {
        searching_type = 1;
    }
}

void SearcherWidget::selectAll()
{
    if (browser->getRootFolder() == NULL)
        return;
    FolderItem& f = *browser->getRootFolder();
    for(int i = 0; i < f.getChilCount(); ++i)
    {
        File* file = f.getChild(i)->toFile()->getFilePtr();
        file->setSelectedToPrint(true);
        emit helper_data->printQueueChanged(file, true);
    }
}








