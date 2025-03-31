#include "AnimationWindow.h"
#include "widgets/TextInput.h"
#include "widgets/Button.h"
#include "login.h"
#include "choices.h"
#include <fstream>
#include <string>
#include <iostream>


login::login(player* playerPtr) 
    : nameInput(TDT4102::Point{0, 0}, 0, 0, ""), window(nullptr), currentPlayer(playerPtr)
{
}

login::~login() {
    delete window;
}

int login::loginWindow() {

    std::string windowTitle = "Gambling++";

    window = new TDT4102::AnimationWindow(1920/2-windowWidth/2, 1080/2-windowHeight/2, windowWidth, windowHeight, windowTitle);

    /*Info tekst*/
    
    std::string info = "LET'S GO GAMBLING!";
    int infoFontSize = 40;
    int infoWidth = info.length()*infoFontSize*0.6;
    const TDT4102::Point infoPosition {static_cast<int>(windowWidth / 2) - static_cast<int>(infoWidth / 2), 50};


    /*Bilde*/
    TDT4102::Image loginImage("assets/hearts-unknown.png");
    TDT4102::Point loginImagePoint(static_cast<int>(windowWidth / 2) + static_cast<int>(infoWidth/2), 50);


    /*Tekst input*/
    const unsigned int textWidth = windowWidth/2;
    const unsigned int textHeight = windowHeight/7;
    const TDT4102::Point inputPosition {static_cast<int>((windowWidth / 2) - (textWidth / 2)),static_cast<int>((windowHeight / 2) - (windowHeight / 4))};    
    const std::string textPlaceholder = "Brukernavn";
    nameInput = TDT4102::TextInput{inputPosition, textWidth, textHeight, textPlaceholder};
    window->add(nameInput);
    nameInput.setCallback([this]() { checkUser(); }); 

    /*Knapp*/
    const unsigned int buttonWidth = windowWidth/3;
    const unsigned int buttonHeight = windowHeight/7;
    const TDT4102::Point buttonPosition {static_cast<int>((windowWidth / 2) - (buttonWidth / 2)),static_cast<int>((windowHeight / 2) + buttonHeight)};    
    const std::string buttonLabel = "Logg inn/Registrer";    
    TDT4102::Button button {buttonPosition, buttonWidth, buttonHeight, buttonLabel};
    window->add(button);
    button.setCallback([this]() { registerUser(); });




    while (!window->should_close() && !shouldExit) {
        window->next_frame();
        window->draw_text(infoPosition, info, TDT4102::Color::black, infoFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_image(loginImagePoint, loginImage);

        if (!usernameText.empty()) {
            int fontSize = 20;
            TDT4102::Color textColour = TDT4102::Color::red;
            int usernameTextWidth = usernameText.length() * fontSize * 0.6;
            const TDT4102::Point usernamePosition {static_cast<int>(windowWidth / 2) - static_cast<int>(usernameTextWidth / 2), static_cast<int>(windowHeight / 2)};
            window->draw_text(usernamePosition, usernameText, textColour, fontSize, TDT4102::Font::courier_bold_italic);

        }
    }

    return 0;
}


void login::checkUser() {
    std::string username = nameInput.getText();
    bool userExists = false;
    int userPoints = 0;

    std::ifstream infile("users.txt");
    std::string line;

    while (std::getline(infile, line)) {
        size_t delimiterPos = line.find(",");
        std::string existingUsername = line.substr(0, delimiterPos);
        if (existingUsername == username) {
            userExists = true;
            userPoints = std::stoi(line.substr(delimiterPos + 1)); //henter poeng fra users.txt (stringtoint)
            break;
        }
    }
    infile.close();

    if (userExists) {
        std::cout << usernameText << std::endl;       
        usernameText = "Brukernavnet " + username + " er allerede i bruk. \n Vil du bruke dette."; //må fikse newline her
    }   else {
        usernameText.clear();
    }
}
    

void login::registerUser() {
    const unsigned int startingPoints = 1000;
    std::string username = nameInput.getText();
    

    currentPlayer->registerUser(username, startingPoints);
    
    window->close();
    shouldExit = true;
    delete window;
    chooseGame(currentPlayer);
}