#include "printerwidget.h"
#include "ui_printerwidget.h"
#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>

PrinterWidget::PrinterWidget(QWidget *parent, StudentHelper* h_data) :
    QWidget(parent),
    ui(new Ui::PrinterWidget),
    helper_data(h_data)
{
    ui->setupUi(this);

    connect(helper_data, SIGNAL(printQueueChanged(File*,bool)),  this, SLOT(queueRefresh(File*,bool)) );

    connect(ui->print_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showInfo(QListWidgetItem*)));
    connect(ui->corner_coord_x, SIGNAL(valueChanged(int)), this, SLOT(newSize()));
    connect(ui->corner_coord_y, SIGNAL(valueChanged(int)), this, SLOT(newSize()));
    connect(ui->new_size_x, SIGNAL(valueChanged(int)), this, SLOT(newSize()));
    connect(ui->new_size_y, SIGNAL(valueChanged(int)), this, SLOT(newSize()));
    connect(ui->cut_button, SIGNAL(clicked(bool)), this, SLOT(cut()));
    connect(ui->cancel_button, SIGNAL(clicked(bool)), this, SLOT(getBack()));
    connect(ui->rotate_left_button, SIGNAL(clicked(bool)), this, SLOT(rotateLeft()));
    connect(ui->rotate_right_button, SIGNAL(clicked(bool)), this, SLOT(rotateRight()));
    connect(ui->discolour_button, SIGNAL(clicked(bool)), this, SLOT(discolor()));
    connect(ui->delete_selected_button, SIGNAL(clicked(bool)), this, SLOT(deleteSelectedItems()));
    connect(ui->print_button, SIGNAL(clicked(bool)), this, SLOT(showPrintDialog()));

    edit_history = new QMap<QString,QList<QPixmap*> >;

    work_pix = NULL;
    currentItem = NULL;
    previousItem = NULL;
}

PrinterWidget::~PrinterWidget()
{
    disconnect(helper_data, SIGNAL(printQueueChanged(File*,bool)), this, SLOT(queueRefresh(File*,bool)));
    delete ui;
}

void PrinterWidget::queueRefresh(File* filePtr, bool isAdded)
{
    if (filePtr == NULL)
        return;
    const QString& name = filePtr->getName();
    QListWidget& lst = *ui->print_list;
    if (isAdded)
    {
        QListWidgetItem* item = new QListWidgetItem(name, &lst);
        item->setFlags( item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
    else
    {
        for(int i = 0; i < lst.count() ; ++i)
        {
            QListWidgetItem* item = lst.item(i);
            if (item->text() == name)
            {
                edit_history->erase(edit_history->find(name));
                if (previousItem == item)
                {
                    previousItem = NULL;
                }
                if (currentItem == item)
                {
                    currentItem = NULL;
                }
                delete item;
                break;
            }
        }
    }
}

void PrinterWidget::resetCutParameters()
{
    ui->current_width->setText(QString::number(work_pix->width()));
    ui->current_heigh->setText(QString::number(work_pix->height()));

    ui->corner_coord_x->setValue(0);
    ui->corner_coord_y->setValue(0);
    ui->new_size_x->setValue(work_pix->width());
    ui->new_size_y->setValue(work_pix->height());
}

void PrinterWidget::showInfo(QListWidgetItem* item)
{
 //   if (currentItem != NULL)
 //   {
        previousItem = currentItem;
 //   }

    currentItem = item;

    if ( currentItem == previousItem )
    {
        return;
    }

    if (previousItem != NULL)
    {
        addToHistory(previousItem->text(), *work_pix);
    }

    const QString& path = item->text();
    QLabel* lbl = new QLabel;
    if (work_pix != NULL)
    {
        delete work_pix;
    }

    QMap<QString,QList<QPixmap*> >::iterator it = edit_history->find(currentItem->text());
    if (it != edit_history->end())
    {
        work_pix = it->back();
        it->pop_back();
    }
    else
    {
        work_pix = new QPixmap(path);
    }
    lbl->setPixmap(*work_pix);
    ui->monitor_area->setWidget(lbl);

    resetCutParameters();
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

    QRect* rect = new QRect(x,y,w-1,h-1);
    QPixmap pix = work_pix->copy();
    QPainter p(&pix);
    QPen pen(Qt::red, 1, Qt::DashLine);
    p.setPen(pen);
    p.drawRect(*rect);

    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(pix);
}

void PrinterWidget::addToHistory(const QString& fname, const QPixmap& pix)
{
    QMap<QString,QList<QPixmap*> >::iterator it = edit_history->find(fname);
    QPixmap* p = new QPixmap(pix);
    if (it != edit_history->end())
    {
        it->push_back(p);
    }
    else
    {
        edit_history->insert(fname, (QList<QPixmap*>() << p) );
    }
}

void PrinterWidget::cut()
{
    if (ui->print_list->count() == 0)
        return;

    int x = ui->corner_coord_x->value();
    int y = ui->corner_coord_y->value();
    int w = ui->new_size_x->value();
    int h = ui->new_size_y->value();

    if (x == 0 && y == 0 && w == work_pix->width() && h == work_pix->height())
        return;

    addToHistory(currentItem->text(), *work_pix);

    QPixmap* p = new QPixmap(work_pix->copy(x,y,w,h));
    delete work_pix;
    work_pix = p;
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);

    resetCutParameters();
}

void PrinterWidget::getBack()
{
    if (ui->print_list->count() == 0 || edit_history == NULL)
        return;
    QMap<QString,QList<QPixmap*> >::iterator it = edit_history->find(currentItem->text());
    if (it == edit_history->end())
        return;
    if (it->empty())
        return;

    delete work_pix;
    work_pix = it->back();
    it->pop_back();
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);

    resetCutParameters();
}

void PrinterWidget::rotateLeft()
{
    if (ui->print_list->count() == 0)
        return;

    addToHistory(currentItem->text(), *work_pix);

    QPixmap* pix = new QPixmap(work_pix->height(), work_pix->width());
    QPainter p(pix);
    p.translate(0,work_pix->width());
    p.rotate(-90);
    p.drawPixmap(work_pix->rect(), *work_pix);

    delete work_pix;
    work_pix = pix;
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);
    resetCutParameters();
}

void PrinterWidget::rotateRight()
{
    if (ui->print_list->count() == 0)
        return;

    addToHistory(currentItem->text(), *work_pix);

    QPixmap* pix = new QPixmap(work_pix->height(), work_pix->width());
    QPainter p(pix);
    p.translate(work_pix->height(),0);
    p.rotate(90);
    p.drawPixmap(work_pix->rect(), *work_pix);

    delete work_pix;
    work_pix = pix;
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);
    resetCutParameters();
}

void PrinterWidget::discolor()
{
    if (ui->print_list->count() == 0)
        return;

    addToHistory(currentItem->text(), *work_pix);

    QImage img = work_pix->toImage();
    for( int w = 0; w < img.rect().right(); ++w )
    {
        for( int h = 0; h < img.rect().bottom(); ++h )
        {
            QColor col( img.pixel(w,h) );
            col.setHsv(col.hue(), 0, col.value(), col.alpha());
            img.setPixel(w,h,col.rgb());
        }
    }
    delete work_pix;
    work_pix = new QPixmap( QPixmap::fromImage(img) );
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);
    resetCutParameters();
    emit ui->corner_coord_x->valueChanged(0);
}

void PrinterWidget::deleteSelectedItems()
{
    QListWidget& lst = *ui->print_list;
    if (lst.count() == 0)
        return;

    for(int i = lst.count()-1; i != -1 ; --i)
    {
        QListWidgetItem* item = lst.item(i);
        if (item->checkState() == Qt::Checked)
        {
            File* file = helper_data->findFileByName(item->text());
            if ( file != NULL )
            {
                file->setSelectedToPrint(false);
            }
        }
    }
}

void PrinterWidget::showPrintDialog()
{
    QPrinter printer;
    QPrintDialog* dial = new QPrintDialog(&printer);
    if (dial->exec() == QDialog::Accepted)
    {
        qDebug() << "Print...";
    }
    delete dial;
}
