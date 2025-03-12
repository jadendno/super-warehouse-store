#ifndef WH_SHOPPERS_H
#define WH_SHOPPERS_H

#include "warehouseStock.h"
#include <QtCore/qobject.h>
#include <QFile>
#include <QTextStream>
#include <vector>

    /********************************************
     * Creates shopper object that holds the
     * details of warehouse patrons including:
     * name, member id, vector of Item object
     * from warehouseStock class member type,
     * rebate amount,and expiration date.
     *
     * ~NEED TO DO~
     *  -Eventually add value that holds total
     *   amount spent on Warehouse.
     *  -Add list of items that customer has
     *   bought and their quantities.
     *  -Calculate savings from rebate if patron
     *   is a preferred member.
     *********************************************/


class Shopper {

public:

    //As of right now, this constructor is all that will be necessary.
    //If function to create new member were implemented, a different constructor
    //would likely be need to create a new member id.

    Shopper(QString n, int mId, QString mTyp, int mEd) :
        name(n), memberId(mId), type(mTyp), expirationDate(mEd) {}

    /********************************************
     * Total spent function will calculate the
     * amount the member has spent and return
     * the value. Will take into account member
     * type.
     *********************************************/

    float totalSpent();

    /********************************************
     * Add Purchase will add an item to the
     * member's purchase history. Will likely be
     * called from warehouseStock member function,
     * as the list of available items will be
     * decreased there.
     *********************************************/

    void addPurchase(Item s);

    /********************************************
     * Display function will take in index
     * argument and display purchase details.
     * Subject to change given ui functionality.
     *********************************************/

    void displayPurchase();

    /* ~~Accessors~~ */

    int purchaseListSize(); //Returns size of purchase history list

    bool isMember() const; //Checks member type.

    int getmId() const; //Returns member id

    QString getmName() const; //Returns member name

    int getmEd() const; //Returns member expiration date

    /********************************************
     * fileIn function will read from file that
     * details WareHouse patrons. Will initialize
     * Shopper object with appropriate values.
     *********************************************/


private:

    QString name;
    int memberId;
    QString type;
    int expirationDate;
    std::vector<Item> purchaseHistory;

};

#endif // WH_SHOPPERS_H
