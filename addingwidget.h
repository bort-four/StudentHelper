#ifndef ADDINGWIDGET_H
#define ADDINGWIDGET_H

#include <QWidget>

namespace Ui {
class AddingWidget;
}

class AddingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddingWidget(QWidget *parent = 0);
    ~AddingWidget();

private:
    Ui::AddingWidget *ui;
};

#endif // ADDINGWIDGET_H
