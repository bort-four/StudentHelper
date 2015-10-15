#include "searcherwidget.h"
#include "ui_searcherwidget.h"

#include <QStringListModel>
#include <QItemSelectionModel>
#include <QPainter>

SearcherWidget::SearcherWidget(QWidget *parent, StudentHelper *hlpr) :
    QWidget(parent),
    ui(new Ui::SearcherWidget)
{
    ui->setupUi(this);

    helper_data = hlpr;
    searching_type = 0;

    connect( ui->query_text_line,       SIGNAL(textChanged(QString)),   this, SLOT(querySpecification(QString))  );
    connect( ui->name_search_button,    SIGNAL(clicked(bool)),          this, SLOT(searchTypeSelected())  );
    connect( ui->tag_search_button,     SIGNAL(clicked(bool)),          this, SLOT(searchTypeSelected())  );
    connect( ui->theme_search_button,   SIGNAL(clicked(bool)),          this, SLOT(searchTypeSelected())  );
    connect( ui->find_button,           SIGNAL(clicked(bool)),          this, SLOT(searchStart())         );
}

SearcherWidget::~SearcherWidget()
{
    delete ui;
}

void SearcherWidget::showInfo(const QStringList& lst, const QString& theme)
{
    QString tags_str;
    for( QStringList::const_iterator it = lst.constBegin(); it != lst.constEnd(); ++it )
    {
        tags_str += *it + ", ";
    }
    if (tags_str.count() != 0)
    {
        tags_str.remove(tags_str.count()-2,2);
    }
    ui->tags_label_field->setText(tags_str);
    ui->theme_label_field->setText(theme);
}

void SearcherWidget::searchStart()
{
    if (searching_type == 0)
    {
        QVector<File*>* result = new QVector<File*>;
        QList<File*>& flist = *helper_data->getFileListPtr();
        for(QList<File*>::iterator it = flist.begin(); it != flist.end(); ++it)
        {
            const QString& name = (*it)->getName();
            if (name.contains(query_string))
            {
                result->push_back(*it);
            }
        }
        if( result->empty() )    // очистить старую модель!
        {
            //ui->FoundObjectsListView->setModel(new QStringListModel);
            //return;
        }
        temp_searching_results = result;
        QStringListModel* model = new QStringListModel;
        QStringList str_list;
        for(int i = 0; i < result->size(); ++i)
        {
            str_list.push_back(result->at(i)->getName());
        }
        model->setStringList(str_list);
        ui->FoundObjectsListView->setModel(model);
        connect(ui->FoundObjectsListView, SIGNAL(clicked(QModelIndex)), this, SLOT(showSelectedItem(QModelIndex)));
    }
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

void SearcherWidget::querySpecification(QString query)
{
    query_string = query;
}

void SearcherWidget::showSelectedItem(QModelIndex index)
{
    QListView* list = dynamic_cast<QListView*>( sender() );
    QString name = list->model()->data(index).toString();
    QLabel* lbl = new QLabel();
    lbl->setPixmap(QPixmap(name));
    ui->FoundObjectsMonitor->setWidget(lbl);

    QVector<File*>& res = *temp_searching_results;
    for(int i = 0; i < res.size(); ++i)
    {
        if ( name == res[i]->getName() )
        {
            showInfo( *res[i]->getTagListPtr(), res[i]->getTheme() );
            //showTheme( *helper_data->getRootFolder(), name );
            break;
        }
    }
}







