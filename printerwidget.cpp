﻿#include "printerwidget.h"
#include "ui_printerwidget.h"
#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPrintPreviewDialog>

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
    connect(ui->cancel_button, SIGNAL(clicked(bool)), this, SLOT(undo()));
    connect(ui->rotate_left_button, SIGNAL(clicked(bool)), this, SLOT(rotateLeft()));
    connect(ui->rotate_right_button, SIGNAL(clicked(bool)), this, SLOT(rotateRight()));
    connect(ui->discolour_button, SIGNAL(clicked(bool)), this, SLOT(discolor()));
    connect(ui->delete_selected_button, SIGNAL(clicked(bool)), this, SLOT(deleteSelectedItems()));
    connect(ui->print_button, SIGNAL(clicked(bool)), this, SLOT(printOneByOne()));
    connect(ui->compose_button, SIGNAL(clicked(bool)), this, SLOT(composeAndPrint()));
    connect(ui->selection_button, SIGNAL(clicked(bool)), this, SLOT(selectAll()));
    connect(ui->scale_minus_button, SIGNAL(clicked(bool)), this, SLOT(scale()));
    connect(ui->scale_plus_button, SIGNAL(clicked(bool)), this, SLOT(scale()));

    edit_history = new QMap<QString,QList<QPixmap*> >;
    current_states = new QMap<QString,QPixmap*>;

    work_pix = NULL;
    currentItem = NULL;
    previousItem = NULL;
}

PrinterWidget::~PrinterWidget()
{
    disconnect(helper_data, SIGNAL(printQueueChanged(File*,bool)), this, SLOT(queueRefresh(File*,bool)));
    delete ui;
}

void PrinterWidget::addCurrentState(const QString& fname, QPixmap* pix)
{
//    QMap<QString,QPixmap*>::iterator it = current_states->find(fname);
//    if (it == current_states->end())
//    {
        current_states->insert(fname,pix);
//    }
}

void PrinterWidget::resetCurrentState(const QString& fname, QPixmap* pix)
{
    QMap<QString,QPixmap*>::iterator it = current_states->find(fname);
    if (it != current_states->end())
    {
        QPixmap* vpix = it.value();
        if (vpix != NULL)
            delete vpix;
    }
    current_states->insert(fname, pix);
}

void PrinterWidget::removeCurrentState(const QString& fname)
{
    current_states->remove(fname);
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
        addCurrentState(name, new QPixmap(name));
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
                    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
                    work_pix = new QPixmap;
                    l->setPixmap(*work_pix);
                    resetCutParameters();
                }
                removeCurrentState(name);
                delete item;
                break;
            }
        }
//        if (lst.count() == 0)
//        {
//            QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
//            qDebug() << (l==NULL);
//            work_pix = new QPixmap(1,1);
//            l->setPixmap(*work_pix);qDebug() << "AAAAAAAAAAAAA";
//            resetCutParameters();qDebug() << "AAAAAAAAAAAAA";
//        }
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
    previousItem = currentItem;
    currentItem = item;

    if ( currentItem == previousItem )
    {
        return;
    }

    work_pix = current_states->find(item->text()).value();

    QLabel* lbl = new QLabel;
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

    QPixmap* pix = setRedBox(QRect(x,y,w-1,h-1));
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*pix);
}

QPixmap* PrinterWidget::setRedBox(const QRect& rect)
{
    QPixmap* pix = new QPixmap(*work_pix);
    if (work_pix->size().width() == 0 || work_pix->size().height() == 0)
        return work_pix;
    QPainter p(pix);
    QPen pen(Qt::red, 1, Qt::DashLine);
    p.setPen(pen);
    p.drawRect(rect);
    return pix;
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

    const QString& name = currentItem->text();
    QPixmap* p = new QPixmap(work_pix->copy(x,y,w,h));

    addToHistory(name, *work_pix);
    resetCurrentState(name, p);
    work_pix = p;
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);

    resetCutParameters();
}

void PrinterWidget::undo()
{
    if (ui->print_list->count() == 0 || edit_history == NULL)
        return;
    const QString& name = currentItem->text();
    QMap<QString,QList<QPixmap*> >::iterator it = edit_history->find(name);
    if (it == edit_history->end())
        return;
    if (it->empty())
        return;

    QPixmap* pix = it->back();
    bool nsize = false;
    if (work_pix->width() == pix->width())
        if (work_pix->height() == pix->height())
            nsize = false;

    resetCurrentState(name,pix);
    work_pix = pix;
    it->pop_back();

    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    pix = nsize ? work_pix : setRedBox(QRect(0,0,work_pix->width()-1,work_pix->height()-1));
    l->setPixmap(*pix);

    resetCutParameters();
}

//void PrinterWidget::redo()
//{
//    if (ui->print_list->count() == 0 || edit_history == NULL)
//        return;
//    const QString& name = currentItem->text();
//    QMap<QString,QList<QPixmap*> >::iterator it = edit_history->find(name);
//    if (it == edit_history->end())
//        return;
//    if (it->empty())
//        return;

//    QPixmap* pix = it->back();
//    bool nsize = false;
//    if (work_pix->width() == pix->width())
//        if (work_pix->height() == pix->height())
//            nsize = false;

//    resetCurrentState(name,pix);
//    work_pix = pix;
//    it->pop_back();

//    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
//    pix = nsize ? work_pix : setRedBox(QRect(0,0,work_pix->width()-1,work_pix->height()-1));
//    l->setPixmap(*pix);

//    resetCutParameters();
//}

void PrinterWidget::rotateLeft()
{
    if (ui->print_list->count() == 0)
        return;

    const QString& name = currentItem->text();

    addToHistory(name, *work_pix);

    QPixmap* pix = new QPixmap(work_pix->height(), work_pix->width());
    QPainter p(pix);
    p.translate(0,work_pix->width());
    p.rotate(-90);
    p.drawPixmap(work_pix->rect(), *work_pix);

    resetCurrentState(name,pix);
    work_pix = pix;
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);
    resetCutParameters();
}

void PrinterWidget::rotateRight()
{
    if (ui->print_list->count() == 0)
        return;

    const QString& name = currentItem->text();

    addToHistory(currentItem->text(), *work_pix);

    QPixmap* pix = new QPixmap(work_pix->height(), work_pix->width());
    QPainter p(pix);
    p.translate(work_pix->height(),0);
    p.rotate(90);
    p.drawPixmap(work_pix->rect(), *work_pix);

    resetCurrentState(name,pix);
    work_pix = pix;
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);
    resetCutParameters();
}

void PrinterWidget::scale()
{
    if (ui->print_list->count() == 0)
        return;

    double sx, sy;
    sx = sender() == ui->scale_plus_button ? sy = 1.1 : sy = 0.9;

    const QString& name = currentItem->text();

    addToHistory(currentItem->text(), *work_pix);

    QSize size(sx*work_pix->width(),sy*work_pix->height());
    QPixmap* pix = new QPixmap( work_pix->scaled(size,Qt::KeepAspectRatio) );

    resetCurrentState(name,pix);
    work_pix = pix;
    QLabel* l = dynamic_cast<QLabel*>(ui->monitor_area->widget());
    l->setPixmap(*work_pix);
    resetCutParameters();
}

void PrinterWidget::discolor()
{
    if (ui->print_list->count() == 0)
        return;

    const QString& name = currentItem->text();

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
    QPixmap* pix = new QPixmap( QPixmap::fromImage(img) );
    resetCurrentState(name,pix);
    work_pix = pix;
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

void PrinterWidget::selectAll()
{
    if (ui->print_list->count() == 0)
        return;

    Qt::CheckState status1, status2;
    QPushButton* pb = ui->selection_button;
    if (pb->text() == "Выделить всё")
    {
        pb->setText("Снять отметки");
        status1 = Qt::Unchecked;
        status2 = Qt::Checked;
    }
    else
    {
        pb->setText("Выделить всё");
        status1 = Qt::Checked;
        status2 = Qt::Unchecked;
    }

    QListWidget& lst = *ui->print_list;
    for(int i = 0; i < lst.count(); ++i)
    {
        QListWidgetItem* item = lst.item(i);
        if (item->checkState() == status1)
        {
            item->setCheckState(status2);
        }
    }
}

QList<QPixmap*>& PrinterWidget::getSelectedPixes()
{
    QList<QPixmap*>& pixes_to_print = *(new QList<QPixmap*>);
    const QListWidget& lst = *ui->print_list;
    for(int i = 0; i < lst.count(); ++i)
    {
        QListWidgetItem* item = lst.item(i);
        if (item->checkState() == Qt::Checked)
        {
            QMap<QString,QPixmap*>::iterator it = current_states->find(item->text());
            if (it == current_states->end())
                continue;
            QPixmap* pix = it.value();
            if (pix == NULL)
                continue;
            pixes_to_print.append(pix);
        }
    }
    return pixes_to_print;
}

void PrinterWidget::composeAndPrint()
{
    if (ui->print_list->count() == 0)
        return;

    QPrinter printer;
    QPrintPreviewDialog preview( &printer, this );
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_composed(QPrinter*)) );
    preview.exec();
}

void PrinterWidget::print_composed(QPrinter* printer)
{
//    QList<QPixmap*> pixes_to_print = getSelectedPixes();
    QPixmap* composition = new QPixmap;
    /*
     * Composition...
    */
    QPainter painter;
    painter.begin(printer);
    painter.drawPixmap( composition->rect(), *composition );
    painter.end();
    delete composition;
}

void PrinterWidget::printOneByOne()
{
    if (ui->print_list->count() == 0)
        return;

    QPrinter printer;
    QPrintPreviewDialog preview( &printer, this );
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_selected(QPrinter*)) );
    preview.exec();
}

void PrinterWidget::print_selected(QPrinter* printer)
{
    QList<QPixmap*> pixes_to_print = getSelectedPixes();
    QPainter painter;
    painter.begin(printer);
    for (QList<QPixmap*>::iterator it = pixes_to_print.begin(); it != pixes_to_print.end(); ++it)
    {
        if (it != pixes_to_print.begin())
        {
            printer->newPage();
        }
        painter.drawPixmap( (*it)->rect(), *(*it) );
    }
    painter.end();
}
