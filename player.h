#pragma once
#include <string>
#include <vector>
#include "CardDeck.h"

class player {
    private:
        std::string* username;
        double* money;
    public:
        player();
        ~player();

        std::string getUsername()const;
        double getMoney() const;
        void registerUser(const std::string& newUsername, int startingMoney);
        void setMoney(double newMoney);
        void subMoney(double amount);
        void addMoney(double amount);
        void saveMoney();
        std::vector<Card*> hand;
        bool hasFolded = false;
};