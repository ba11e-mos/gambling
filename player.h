#pragma once
#include <string>

class player {
    private:
        std::string* username;
        int* money;
    public:
        player();
        ~player();

        std::string getUsername()const;
        int getMoney() const;
        void registerUser(const std::string& newUsername, int startingMoney);
        void setMoney(int newMoney);
        void subMoney(int amount);
        void addMoney(int amount);
};