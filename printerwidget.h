#ifndef PRINTERWIDGET_H
#define PRINTERWIDGET_H

#include <QWidget>
#include <studenthelper.h>
#include <QListWidget>
#include <QPrinter>

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
    void resetCutParameters(int width, int height);
    void setRedBox(QPixmap *pixmap, const QRect &rect);
    void addCurrentState(const QString&fname, const QPixmap &pix);
    void resetCurrentState(const QString&fname, const QPixmap &pix);
    void removeCurrentState(const QString&fname);
    QList<QPixmap *> &getSelectedPixes();
    void clearHistory(QMap<QString,QList<QPixmap*> >::iterator it);

public slots:
    void queueRefresh(File *filePtr, bool isAdded);
    void showInfo(QListWidgetItem*);
    void newSize();
    void cut();
    void undo();
    void rotateLeft();
    void rotateRight();
    void discolor();
    void deleteSelectedItems();
    void selectAll();
    void printOneByOne();
    void composeAndPrint();
    void print_selected(QPrinter* printer);
    void print_composed(QPrinter* printer);
    void scale();

private:
    Ui::PrinterWidget *ui;
    StudentHelper* helper_data;

    QPixmap* work_pix;
    QPixmap* cut_rect_show_pix;
    QListWidgetItem* currentItem;
    QListWidgetItem* previousItem;

    QMap<QString,QList<QPixmap*> >* edit_history;
    QMap<QString,QPixmap*>* current_states;
};

#endif // PRINTERWIDGET_H
