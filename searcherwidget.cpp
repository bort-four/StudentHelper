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

void SearcherWidget::searchStart()
{
    if (searching_type == 0)
    {
        QVector<File*> result;
        QList<File*>& flist = *helper_data->getFileListPtr();
        for(QList<File*>::iterator it = flist.begin(); it != flist.end(); ++it)
        {
            const QString& name = (*it)->getName();
            if (name.contains(query_string))
            {
                result.push_back(*it);
            }
        }
        if( result.empty() )    // очистить старую модель!
        {
            //ui->FoundObjectsListView->setModel(new QStringListModel);
            //return;
        }
        temp_searching_results = &result;
        QStringListModel* model = new QStringListModel;
        QStringList str_list;
        for(int i = 0; i < result.size(); ++i)
        {
            str_list.push_back(result[i]->getName());
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

    const QVector<File*>& tres = *temp_searching_results;
}
