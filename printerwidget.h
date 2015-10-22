#ifndef PRINTERWIDGET_H
#define PRINTERWIDGET_H

#include <QWidget>
#include <studenthelper.h>

namespace Ui {
class PrinterWidget;
}

class PrinterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrinterWidget(QWidget *parent = 0, StudentHelper* = 0);
    ~PrinterWidget();

private:
    Ui::PrinterWidget *ui;
    StudentHelper* helper_data;
};

#endif // PRINTERWIDGET_H
