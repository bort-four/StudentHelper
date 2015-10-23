#ifndef PRINTERWIDGET_H
#define PRINTERWIDGET_H

#include <QWidget>
#include <studenthelper.h>
#include <QListWidget>

namespace Ui {
class PrinterWidget;
}

class PrinterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrinterWidget(QWidget *parent = 0, StudentHelper* = 0);
    ~PrinterWidget();

public slots:
    void queueRefresh(File *filePtr, bool isAdded);
    void showInfo(QListWidgetItem*);
    void newSize();
    void cut();
    void getBack();

private:
    Ui::PrinterWidget *ui;
    StudentHelper* helper_data;

    QPixmap* work_pix;              // рабочая картинка, её все время перерисовываем
    QPixmap* previous_pix;          // хранит предыдущее состояние картинки, загружается нажатием кнопки "ОТМЕНА"
    QListWidgetItem* currentItem;
};

#endif // PRINTERWIDGET_H
