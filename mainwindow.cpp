#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "warehouseStock.h"
#include "wh_shoppers.h"

MainWindow::MainWindow(WarehouseStock *manager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(manager)
{
    ui->setupUi(this);
    ui->startMenu->setCurrentIndex(0);
    ui->memberSearch->hide();

    /* if (!manager) {
        qDebug() << "Manager is NULL!";

    } else {
        qDebug() << "Manager is not NULL!";

    }
    const std::vector<Shopper*>& pShoppers = manager->getMemberList();
    if (pShoppers.empty()) {
        qDebug() << "Member list is empty!";
    } else {
        qDebug() << "Member list has" << pShoppers.size() << "members";
    }
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Button push to Quit application.
void MainWindow::on_pushButton_4_clicked()
{
    QCoreApplication::quit();
}


//Button push to Daily Revenue Window
void MainWindow::on_dailyRevPush_clicked()
{
    ui->startMenu->setCurrentIndex(1);
}

//Button to Item Sales Window
void MainWindow::on_itemSalePush_clicked()
{
    ui->startMenu->setCurrentIndex(2);
}

//Button to Stock Window
//Messed up names, for now quit is stock window
//and push button 4 quits application
void MainWindow::on_quitPush_clicked()
{
    ui->startMenu->setCurrentIndex(2);
}

//Returns from Daily Rev window to main stacked widget window.
void MainWindow::on_dailyRevBack_clicked()
{
    ui->startMenu->setCurrentIndex(0);
}

//Returns from Item Sales window to main stacked window widget.
void MainWindow::on_pushButton_clicked()
{
    ui->startMenu->setCurrentIndex(0);
}

//Returns from Stock window to main stacked window widget.
void MainWindow::on_pushButton_2_clicked()
{
    ui->startMenu->setCurrentIndex(0);
}


//Button to Membership Data window
void MainWindow::on_pushButton_3_clicked()
{
    ui->startMenu->setCurrentIndex(3);
}


void MainWindow::on_pushButton_5_clicked()
{

    const std::vector<Shopper*> pShoppers = manager->getMemberList();
    /*
    if (pShoppers.empty()) {
        qDebug() << "Member list is empty!";
    } else {
        qDebug() << "Member list has" << pShoppers.size() << "members";
    }
    */

    QString displayinfo;


    /* qDebug() << "Number of shoppers: " << manager->getMemberlength(); */

    for (Shopper* shopper : pShoppers) {

        if (shopper->isMember()) {
            displayinfo += "Shopper Name: " + shopper->getmName() + "\n";
            displayinfo += "Shopper Id: " + QString::number(shopper->getmId()) + "\n";
            displayinfo += "Shopper Expiration Date: " + QString::number(shopper->getmEd()) + "\n\n";
        }
    }

    ui->label->setText(displayinfo);
}


void MainWindow::on_pushButton_6_clicked()
{

    QString displayinfo2;

    const std::vector<Shopper*> pShoppers = manager->getMemberList();
    for (Shopper* shopper : pShoppers) {

        if (!shopper->isMember()) {
            displayinfo2 += "Shopper Name: " + shopper->getmName() + "\n";
            displayinfo2 += "Shopper Id: " + QString::number(shopper->getmId()) + "\n";
            displayinfo2 += "Shopper Expiration Date: " + QString::number(shopper->getmEd()) + "\n\n";
        }
    }

    ui->label->setText(displayinfo2);
}


void MainWindow::on_allMemPush_clicked()
{
    QString displayinfo;

    const std::vector<Shopper*> pShoppers = manager->getMemberList();
    for (Shopper* shopper : pShoppers) {

        displayinfo += "Shopper Name: " + shopper->getmName() + "\n";
        displayinfo += "Shopper Id: " + QString::number(shopper->getmId()) + "\n";
        displayinfo += "Shopper Expiration Date: " + QString::number(shopper->getmEd()) + "\n\n";
    }

    ui->label->setText(displayinfo);



}


 void MainWindow::on_memberSearch_textEdited(const QString &arg1)
{
    ui->memberSearch->show();
}

