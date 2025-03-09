#include "warehouseStock.h"
#include "wh_shoppers.h"
#include <iostream>

void WarehouseStock::stockItem(QString itemN, double itemP, int itemS) {

    for (size_t i = 0; i < currentStock.size(); i++) {

        if (currentStock[i].itemName == itemN) {
            currentStock[i].stock += itemS;
            return;
        }
    }
    currentStock.emplace_back(itemN, itemP, itemS);
}

const Item& WarehouseStock::getItemByName(const QString& itemName) const {

    for (auto& item : currentStock) {
        if (item.itemName == itemName) {
            return item;
        }
    }
}

void WarehouseStock::newShopper(QString n, int mId, QString mType, int mEd) {

    for (const auto& shopper : memberList) {
        if (shopper->getmId() == mId) {
            return;
        }
    }
    memberList.emplace_back(new Shopper(n,mId,mType,mEd));

}

void WarehouseStock::purchaseMade(int mId, Item s) {

    for (const auto& shopper : memberList) {

        if (shopper->getmId() == mId) {
            std::cout << "Shopper Id found." << std::endl;

            Item purchaseItem = s;
            purchaseItem.stock = 1;

            shopper->addPurchase(purchaseItem);


            for (auto& item : currentStock) {

                if (item.itemName == s.itemName) {

                    item.stock -= 1;
                    break;
                }
            }
            return;
        }
    }
}

void WarehouseStock::memberfileIn(const QString& writeFile) {
    //Using test case for the moment.
}

/* ~~Accessors~~~ */

/***********************************************
 * getMemberList() may be deprecated in favor of
 * multiple functions that return individual
 * item variables. (for window/widget functions)
 ***********************************************/

const std::vector<Shopper*>& WarehouseStock::getMemberList() const {

    //console test, will be commented out.
    /* for (const auto& shopper : memberList) {
        std::cout << "Shopper Name: " << shopper->getmName().toStdString()
        << " Shopper Id: " << shopper->getmId()
        << " Shopper Account Type: ";

        if (shopper->isMember()) {
            std::cout << "Preferred ";
        } else {
            std::cout << "Basic ";
        }
        std::cout << "Shopper Expiration Date: " << shopper->getmEd()
                  << std::endl;
    } */

    return memberList;
}

/***********************************************
 * getStockList() may be deprecated in favor of
 * multiple functions that return individual
 * item variables. (for window/widget functions)
 ***********************************************/

void WarehouseStock::getStockList() const {

    //console test, will be commented out.
    for (const auto& item : currentStock) {
        std::cout << "Item Name: " << item.itemName.toStdString() << " Stock: " << item.stock
                  << " Item Price: " << item.itemPrice << std::endl;;
    }
}

int WarehouseStock::getMemberlength() const {

    return memberList.size();
}

int WarehouseStock::getStockLength() const {

    return currentStock.size();
}
