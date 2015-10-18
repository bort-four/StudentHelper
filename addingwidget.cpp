#include "addingwidget.h"
#include "ui_addingwidget.h"

AddingWidget::AddingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddingWidget)
{
    ui->setupUi(this);
}

AddingWidget::~AddingWidget()
{
    delete ui;
}
