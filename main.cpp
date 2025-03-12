#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WarehouseStock managerL;

    //Console Testing, will be removed.

    managerL.newShopper("John", 123456, "Preffered", 01022333);
    managerL.newShopper("Bill", 223456, "basic", 01022433);
    managerL.newShopper("Jake", 234456, "Preffered", 01022303);
    managerL.newShopper("Hillary", 234556, "basic", 01122333);

    managerL.getMemberList();

    managerL.stockItem("Towels", 11.45, 2);
    managerL.stockItem("Oranges", 4.32, 5);
    managerL.stockItem("Car", 23000.45, 9);
    managerL.stockItem("Poodle", 10000000.99, 1);

    managerL.getStockList();

    managerL.purchaseMade(223456, managerL.getItemByName("Oranges"));
    managerL.purchaseMade(123456, managerL.getItemByName("Car"));
    managerL.purchaseMade(234556, managerL.getItemByName("Oranges"));
    managerL.purchaseMade(234556, managerL.getItemByName("Poodle"));
    managerL.purchaseMade(234456, managerL.getItemByName("Towels"));
    managerL.purchaseMade(123456, managerL.getItemByName("Car"));
    managerL.purchaseMade(223456, managerL.getItemByName("Towels"));

    managerL.getStockList();

    /* for (int i = 0; i < managerL.getMemberlength(); i++) {
        Shopper* shopper = managerL.getMemberList()[i];
        shopper->displayPurchase();
    } */

    MainWindow w(&managerL);
    w.show();
    return a.exec();
}
