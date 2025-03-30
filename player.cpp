#include "player.h"
#include "iostream"
#include "fstream"

player::player() : username(new std::string("")), money(new int(0)) {}

player::~player() {
    delete username;
    delete money;
}

void player::registerUser(const std::string& newUsername, int startingMoney) {
    *username = newUsername;
    *money = startingMoney;

    bool userExists = false;

    std::ifstream infile("users.txt");
    std::string line;

    while (std::getline(infile, line)) {
        size_t delimiterPos = line.find(",");
        std::string existingUsername = line.substr(0, delimiterPos);
        if (existingUsername == newUsername) {
            userExists = true;
            player::setMoney(std::stoi(line.substr(delimiterPos+1)));
            break;
        }
    }

    infile.close();

    if (userExists) {
        std::cout<< "Eksisterer"<< std::endl;
    } else {
        std::ofstream outFile("users.txt", std::ios::app);
        outFile << *username << ","<< startingMoney <<"\n";
        std::cout << *username << " registrert med "<< startingMoney <<" poeng" << std::endl;
        player::setMoney(startingMoney);
        outFile.close();
    }
}

std::string player::getUsername() const {
    return *username;
}

int player::getMoney() const {
    return *money;
}

void player::setMoney(int newMoney) {
    *money = newMoney;
    saveMoney();
}

void player::subMoney(int amount) {
    *money -= amount;
    saveMoney();
}

void player::addMoney(int amount) {
    *money += amount;
    saveMoney();
}

void player::saveMoney() {
    std::ifstream infile("users.txt");
    std::string line;
    std::ofstream temp("temp.txt", std::ios::app);
    int tempMoney;

    while (std::getline(infile, line)) {
        size_t delimiterPos = line.find(",");
        std::string existingUsername = line.substr(0, delimiterPos);
        if (existingUsername == *username) {
            tempMoney = *money;
        } else {
            tempMoney = std::stoi(line.substr(delimiterPos+1));
        }
        temp << existingUsername << "," << std::to_string(tempMoney) << "\n";
    }
    infile.close();
    temp.close();

    std::remove("users.txt");
    std::rename("temp.txt", "users.txt");
}
