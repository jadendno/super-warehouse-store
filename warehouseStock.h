#ifndef WAREHOUSESTOCK_H
#define WAREHOUSESTOCK_H

#include <QtCore/qobject.h>
#include <QFile>
#include <QTextStream>
#include <vector>

    /********************************************
     * Warehouse stock to help with we_shoppers
     * file. May be built upon or deprecated.
     *********************************************/

class Shopper; //Forward declaration of Shopper class.

struct Item {

    QString itemName;
    float itemPrice;
    int stock;

    Item(QString i, float p, int s) : itemName(i), itemPrice(p), stock(s) {}
};

class WarehouseStock {

public:

    void stockItem(QString itemN, double itemP, int itemS);

    void newShopper(QString n, int mId, QString mType, int mEd);

    void purchaseMade(int mId, Item s);

    void memberfileIn(const QString& writeFile);

    /* ~~Accessors~~~ */

    const std::vector<Shopper*>& getMemberList() const;

    void getStockList() const;

    int getMemberlength() const;

    int getStockLength() const;

    const Item& getItemByName(const QString& itemName) const;


private:
    std::vector<Item> currentStock;
    std::vector<Shopper*> memberList;


};

#endif // WAREHOUSESTOCK_H
