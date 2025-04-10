#pragma once;

#include "AnimationWindow.h"
#include "widgets/TextInput.h"
#include "player.h"

class login {
    private:
        TDT4102::TextInput nameInput;
        TDT4102::AnimationWindow* window;
        player* currentPlayer;
        std::string usernameText;
        std::string user;
        bool shouldExit = false;
        const unsigned int windowHeight = 600;
        const unsigned int windowWidth = 600;
    public: 
        login(player* playerPtr);
        ~login();
        int loginWindow();
        void checkUser();
        void registerUser();

    
};