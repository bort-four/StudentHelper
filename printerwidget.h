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

private:
    void addToHistory(const QString&fname, const QPixmap &pix);
    void resetCutParameters();

public slots:
    void queueRefresh(File *filePtr, bool isAdded);
    void showInfo(QListWidgetItem*);
    void newSize();
    void cut();
    void getBack();
    void rotateLeft();
    void rotateRight();
    void discolor();
    void deleteSelectedItems();

private:
    Ui::PrinterWidget *ui;
    StudentHelper* helper_data;

    QPixmap* work_pix;
    QListWidgetItem* currentItem;
    QListWidgetItem* previousItem;

    QMap<QString,QList<QPixmap*> >* edit_history;
};

#endif // PRINTERWIDGET_H
