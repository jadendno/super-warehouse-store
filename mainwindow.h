#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "warehouseStock.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WarehouseStock *manager, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    /*

        BUTTONS!!!

    */
    void on_pushButton_4_clicked();

    void on_dailyRevPush_clicked();

    void on_itemSalePush_clicked();

    void on_quitPush_clicked();

    void on_dailyRevBack_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_allMemPush_clicked();

    void on_memberSearch_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;
    WarehouseStock *manager;
};
#endif // MAINWINDOW_H
