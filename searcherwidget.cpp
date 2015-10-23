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

    connect( ui->name_search_button,    SIGNAL(clicked(bool)),          this, SLOT(searchTypeSelected())        );
    connect( ui->tag_search_button,     SIGNAL(clicked(bool)),          this, SLOT(searchTypeSelected())        );
    connect( ui->theme_search_button,   SIGNAL(clicked(bool)),          this, SLOT(searchTypeSelected())        );
    connect( ui->find_button,           SIGNAL(clicked(bool)),          this, SLOT(baseSearching())             );
    connect( ui->local_find_button,     SIGNAL(clicked(bool)),          this, SLOT(localSearching())            );
    connect( ui->selectAllButton,       SIGNAL(clicked(bool)),          this, SLOT(selectAll())                 );
    connect( ui->printQueueButton,      SIGNAL(clicked(bool)),          this, SLOT(addToPrintQueue())           );
}

SearcherWidget::~SearcherWidget()
{
    delete ui;
}

void SearcherWidget::showInfo(const QStringList& tag_lst, const QStringList& theme_lst)
{
    QString tags_str, theme_str;
    for( QStringList::const_iterator it = tag_lst.constBegin(); it != tag_lst.constEnd(); ++it )
    {
        tags_str += *it + ", ";
    }
    if (tags_str.count() != 0)
    {
        tags_str.remove(tags_str.count()-2,2);
    }
    for( QStringList::const_iterator it = theme_lst.constBegin(); it != theme_lst.constEnd(); ++it )
    {
        theme_str += *it + ", ";
    }
    if (theme_str.count() != 0)
    {
        theme_str.remove(theme_str.count()-2,2);
    }
    ui->tags_label_field->setText(tags_str);
    ui->theme_label_field->setText(theme_str);
}

void SearcherWidget::clearResultList()
{
    QListWidget& res_lst = *ui->FoundObjectsList;
    disconnect(&res_lst, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showSelectedItem(QListWidgetItem*)));
    res_lst.clear();
}

void SearcherWidget::localSearching()
{
    searchStart( *temp_searching_results );
}

void SearcherWidget::baseSearching()
{
    searchStart( *helper_data->getFileListPtr() );
}

void SearcherWidget::searchStart(const QList<File*>& data)
{
    clearResultList();
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
    else    // theme searching
    {
        return;
//        query_string.replace(", ", ",");
//        QStringList themes = query_string.split(",");
//        QList<File*> *res_list = new QList<File*>,
//                     *data_list = new QList<File*>(data);
//        for(QStringList::const_iterator it = themes.begin(); it != themes.end(); ++it)
//        {
//            const QString& theme_name = *it;
//            for(QList<File*>::const_iterator itd = data_list->begin(); itd != data_list->end(); ++itd)
//            {
//                const QStringList& theme_list = *(*itd)->getThemesListPtr();
//                for(QStringList::const_iterator it2 = theme_list.begin(); it2 != theme_list.end(); ++it2)
//                {
//                    const QString& theme = it2->toLower();
//                    if (theme.contains(theme_name))
//                    {
//                        res_list->push_back(*itd);
//                        break;
//                    }
//                }
//            }
//            data_list = new QList<File*>(*res_list);
//            res_list->clear();
//        }
//        result = data_list;
    }
    if( result->empty() )
    {
        clearResultList();
        return;
    }
/*
    FolderItem* f = new FolderItem("");
    f->addChild(new FileItem(result->at(0)));
    FileBrowserWidget* bro = new FileBrowserWidget(f);
    ui->verticalLayout_2->addWidget(bro);
*/
    temp_searching_results = result;
    QListWidget& searching_results = *ui->FoundObjectsList;
    searching_results.clear();
    for(int i = 0; i < result->size(); ++i)
    {
        QListWidgetItem* item = new QListWidgetItem(result->at(i)->getName(), &searching_results);
        item->setFlags( item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
    connect(ui->FoundObjectsList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showSelectedItem(QListWidgetItem*)));
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
    else if (ui->tag_search_button->isChecked())
    {
        searching_type = 1;
    }
    else
    {
        searching_type = 2;
    }
}

void SearcherWidget::showSelectedItem(QListWidgetItem* item)
{
    QString name = item->text();
    QLabel* lbl = new QLabel();
    lbl->setPixmap(QPixmap(name));
    ui->FoundObjectsMonitor->setWidget(lbl);

    QList<File*>& res = *temp_searching_results;
    for(QList<File*>::const_iterator it = res.begin(); it != res.end(); ++it)
    {
        if ( name == (*it)->getName() )
        {
            showInfo( *(*it)->getTagListPtr(), (QStringList() << "") /**(*it)->getThemesListPtr()*/ );
            break;
        }
    }
}

void SearcherWidget::addToPrintQueue()
{
    if (ui->FoundObjectsList->count() == 0)
        return;
    QListWidget& lst = *ui->FoundObjectsList;
    for(int i = 0; i < lst.count(); ++i)
    {
       if (lst.item(i)->checkState() == Qt::Checked)
       {
           helper_data->addToPrintQueue(temp_searching_results->at(i));
       }
    }
}

void SearcherWidget::selectAll()
{
    if (ui->FoundObjectsList->count() == 0)
        return;
    QListWidget& lst = *ui->FoundObjectsList;
    for(int i = 0; i < lst.count(); ++i)
    {
       lst.item(i)->setCheckState(Qt::Checked);
    }
}








