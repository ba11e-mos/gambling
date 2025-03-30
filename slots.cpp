#include "slots.h"

void slots(player* currentPlayer) {
    std::string windowTitle = "Gambling++";
    const unsigned int windowHeight = 720;
    const unsigned int windowWidth = 1280;
    TDT4102::AnimationWindow window(300, 300, windowWidth, windowHeight, windowTitle);


    /*Brukernavn*/
    std::string username = currentPlayer->getUsername();
    int nameFontSize = 20;
    const TDT4102::Point namePosition {10, 10};
    

    /*Poeng*/
    int money = currentPlayer->getMoney();

    int pointsFontSize = 20;
    std::string pointsString = std::to_string(money);
    int pointsLenght = pointsString.length()*pointsFontSize*0.6;
    const TDT4102::Point pointsPosition {static_cast<int>(windowWidth)-static_cast<int>(10+pointsLenght), 10};

    /*Spillvindu*/
    const unsigned int playSquareHeight = windowHeight-300;
    const unsigned int playSquareWidth = windowWidth-300;
    TDT4102::Color playSquareFill = TDT4102::Color::white;
    TDT4102::Color playSquareEdge = TDT4102::Color::black;
    const TDT4102::Point playSquarePosition {150, 150};


    /*Spinneknapp*/

    while (!window.should_close()) {
        window.next_frame();
        window.draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window.draw_text(pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);
        window.draw_rectangle(playSquarePosition, playSquareWidth, playSquareHeight, playSquareFill, playSquareEdge);
        //window.wait_for_close();
    }
}