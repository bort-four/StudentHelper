#ifndef SEARCHERWIDGET_H
#define SEARCHERWIDGET_H

#include <QWidget>
#include "studenthelper.h"

namespace Ui {
class SearcherWidget;
}

class SearcherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearcherWidget(QWidget *parent = 0, StudentHelper *hlpr = 0);
    ~SearcherWidget();

public slots:
    void searchStart();
    void searchTypeSelected();
    void querySpecification(QString);
    void showSelectedItem(QModelIndex);

private:
    Ui::SearcherWidget *ui;
    StudentHelper* helper_data;
    char searching_type;
    QString query_string;
    QVector<File*>* temp_searching_results;
};

#endif // SEARCHERWIDGET_H
