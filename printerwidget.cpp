#include "printerwidget.h"
#include "ui_printerwidget.h"
#include <QPainter>

PrinterWidget::PrinterWidget(QWidget *parent, StudentHelper* h_data) :
    QWidget(parent),
    ui(new Ui::PrinterWidget),
    helper_data(h_data)
{
    ui->setupUi(this);

    connect(helper_data, SIGNAL(printQueueChanged(File*,bool)),  this, SLOT(queueRefresh(File*,bool)) );
    connect(ui->corner_coord_x, SIGNAL(valueChanged(int)), this, SLOT(newSize()));
    connect(ui->corner_coord_y, SIGNAL(valueChanged(int)), this, SLOT(newSize()));
    connect(ui->new_size_x, SIGNAL(valueChanged(int)), this, SLOT(newSize()));
    connect(ui->new_size_y, SIGNAL(valueChanged(int)), this, SLOT(newSize()));
    connect(ui->cut_button, SIGNAL(clicked(bool)), this, SLOT(cut()));
    connect(ui->cancel_button, SIGNAL(clicked(bool)), this, SLOT(getBack()));

}

PrinterWidget::~PrinterWidget()
{
    disconnect(helper_data, SIGNAL(printQueueChanged(File*,bool)), this, SLOT(queueRefresh(File*,bool)));
    delete ui;
}

void PrinterWidget::queueRefresh(File* filePtr, bool isAdded)
{
    const QString& name = filePtr->getName();
    QList<File*>& pq = *helper_data->getPrintQueuePtr();
    QList<File*>::iterator it = pq.begin();
    for(; it != pq.end(); ++it)
    {
        if ((*it)->getName() == name)
            break;
    }
    if (it == pq.end())
        return;
    if (isAdded)
    {
        QListWidgetItem* item = new QListWidgetItem(name, ui->print_list);
        item->setFlags( item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        connect(ui->print_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showInfo(QListWidgetItem*)));
    }
    else
    {
        pq.erase(it);
    }
}

void PrinterWidget::showInfo(QListWidgetItem* item)
{
    if ( currentItem == item )
        return;
    currentItem = item;
    const QString& path = item->text();
    QLabel* lbl = new QLabel;
    if (work_pix != NULL)
    {
        delete work_pix;
    }
    work_pix = new QPixmap(path);
    lbl->setPixmap(*work_pix);
    ui->monitor_area->setWidget(lbl);
    ui->current_width->setText(QString::number(work_pix->width()));
    ui->current_heigh->setText(QString::number(work_pix->height()));

    ui->corner_coord_x->setValue(0);
    ui->corner_coord_y->setValue(0);
    ui->new_size_x->setValue(work_pix->width());
    ui->new_size_y->setValue(work_pix->height());

}

void PrinterWidget::newSize()
{
    if (ui->print_list->count() == 0)
        return;

    int new_x = ui->corner_coord_x->value();
    int new_y = ui->corner_coord_y->value();
    int new_w = ui->new_size_x->value();
    int new_h = ui->new_size_y->value();

    int pix_w = work_pix->width();
    int pix_h = work_pix->height();
    int x = pix_w - new_x <= 0 ? pix_w : new_x;
    int y = pix_h - new_y <= 0 ? pix_h : new_y;
    int w = new_w - (pix_w-x) >= 0 ? pix_w-x : new_w;
    int h = new_h - (pix_h-y) >= 0 ? pix_h-y : new_h;

    QRect* rect = new QRect(x,y,w,h);
    QPixmap pix = work_pix->copy();
    QPainter p(&pix);
    QPen pen(Qt::red, 2, Qt::DashLine);
    p.setPen(pen);
    p.drawRect(*rect);

    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(pix);
}

void PrinterWidget::cut()
{
    if (ui->print_list->count() == 0)
        return;

    int x = ui->corner_coord_x->value();
    int y = ui->corner_coord_y->value();
    int w = ui->new_size_x->value();
    int h = ui->new_size_y->value();

    previous_pix = work_pix;
    work_pix = new QPixmap(work_pix->copy(x,y,w,h));
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);

    ui->current_width->setText(QString::number(work_pix->width()));
    ui->current_heigh->setText(QString::number(work_pix->height()));

    ui->corner_coord_x->setValue(0);
    ui->corner_coord_y->setValue(0);
    ui->new_size_x->setValue(work_pix->width());
    ui->new_size_y->setValue(work_pix->height());
}

void PrinterWidget::getBack()
{
    if ((ui->print_list->count() == 0) || (previous_pix == NULL))
        return;

    work_pix = previous_pix;
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);
}
