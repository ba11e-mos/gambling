#include "player.h"
#include "iostream"
#include "fstream"

player::player() : username(new std::string("")), money(new double(0.0)) {}

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
        std::cout<< "Login with user "<< *username <<std::endl;
    } else {
        std::ofstream outFile("users.txt", std::ios::app);
        if (!outFile) {
            std::cerr << "Error: Could not open users.txt." << std::endl;
            return;
        }
        outFile << *username << ","<< startingMoney <<"\n";
        outFile.flush();
        std::cout << *username << " registrert med "<< startingMoney <<" poeng" << std::endl;
        player::setMoney(startingMoney);
        outFile.close();
        
    }
    return;
}

std::string player::getUsername() const {
    return *username;
}

double player::getMoney() const {
    return *money;
}

void player::setMoney(double newMoney) {
    *money = newMoney;
    saveMoney();
}

void player::subMoney(double amount) {
    *money -= amount;
    saveMoney();
}

void player::addMoney(double amount) {
    *money += amount;
    saveMoney();
}

void player::saveMoney() {
    std::ifstream infile("users.txt");
    std::string line;
    std::ofstream temp("temp.txt", std::ios::trunc);
    

    while (std::getline(infile, line)) {
        size_t delimiterPos = line.find(",");
        std::string existingUsername = line.substr(0, delimiterPos);
        double tempMoney;
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
