#ifndef SEARCHERWIDGET_H
#define SEARCHERWIDGET_H

#include <QWidget>
#include "studenthelper.h"
#include <QListWidget>
#include "filebrowserwidget.h"

namespace Ui {
class SearcherWidget;
}

class SearcherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearcherWidget(QWidget *parent = 0, StudentHelper *hlpr = 0);
    ~SearcherWidget();

private:
    void searchStart(const QList<File*>&);
//    void showInfo(const QStringList&);
    void clearResultList();
    QString prepareQueryString();

public slots:
    void localSearching();
    void baseSearching();
    void searchTypeSelected();
//    void showSelectedItem(QListWidgetItem *item);
    void addToPrintQueue();
    void selectAll();

private:
    Ui::SearcherWidget* ui;
    StudentHelper*      helper_data;
    char                searching_type;
    QList<File*>*       temp_searching_results;
    FileBrowserWidget*  browser;
};

#endif // SEARCHERWIDGET_H
