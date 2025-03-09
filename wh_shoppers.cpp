#include "wh_shoppers.h"
#include <iostream>

float Shopper::totalSpent() {

    float total = 0.0f;

    for (const auto& item : purchaseHistory) {

        total += item.itemPrice;
    }

    if (this->isMember()) {
        total *= 0.95f;
    }

    return total;
}

//I believe a date an item was purchased should be implemented in the future.
void Shopper::addPurchase(Item s) {

    purchaseHistory.push_back(s);
}

void Shopper::displayPurchase() {

    if (int i = 0; i < purchaseHistory.size()) {
        std::cout << "Item Purchased: "
                  << purchaseHistory[i].itemName.toStdString()
                  << std::endl;
    }
}

int Shopper::purchaseListSize() {

    return purchaseHistory.size();
}

bool Shopper::isMember() const {

    if (type == "Preffered") {
        return true;
    } else {
        return false;
    }
}

int Shopper::getmId() const {

    return memberId;
}

QString Shopper::getmName() const {

    return name;
}

int Shopper::getmEd() const {

    return expirationDate;
}
