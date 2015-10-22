#include "printerwidget.h"
#include "ui_printerwidget.h"

PrinterWidget::PrinterWidget(QWidget *parent, StudentHelper* h_data) :
    QWidget(parent),
    ui(new Ui::PrinterWidget),
    helper_data(h_data)
{
    ui->setupUi(this);

    const QList<File*>& pq = *helper_data->getPrintQueuePtr();
    for(QList<File*>::const_iterator it = pq.begin(); it != pq.end(); ++it)
    {
        QListWidgetItem* item = new QListWidgetItem((*it)->getName(), ui->print_list);
        item->setFlags( item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
    //connect(ui->print_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showSelectedItem(QListWidgetItem*)));


    /*  НЕ РАБОТАЕТ!  */
}

PrinterWidget::~PrinterWidget()
{
    delete ui;
}
