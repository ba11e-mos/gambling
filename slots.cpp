#include "slots.h"

SlotsGame::SlotsGame(player* playerPtr) : currentPlayer(playerPtr), window(nullptr) {}

SlotsGame::~SlotsGame() {
    delete window;
}

void SlotsGame::slots() {
    std::string windowTitle = "Gambling++";
    const unsigned int windowHeight = 720;
    const unsigned int windowWidth = 1280;
    window = new TDT4102::AnimationWindow(300, 300, windowWidth, windowHeight, windowTitle);


    /*Brukernavn*/
    std::string username = currentPlayer->getUsername();
    int nameFontSize = 20;
    const TDT4102::Point namePosition {10, 10};
    

    /*Poeng*/
    int pointsFontSize = 20;
    std::string pointsString = std::to_string(currentPlayer->getMoney());
    int pointsLenght = pointsString.length()*pointsFontSize*0.6;
    const TDT4102::Point pointsPosition {static_cast<int>(windowWidth)-static_cast<int>(10+pointsLenght), 10};

    /*Spillvindu*/
    const unsigned int playSquareHeight = static_cast<int>(3*(windowHeight/5)-50);
    const unsigned int playSquareWidth = windowWidth-300;
    TDT4102::Color playSquareFill = TDT4102::Color::white;
    TDT4102::Color playSquareEdge = TDT4102::Color::black;
    const TDT4102::Point playSquarePosition {150, 50};


    /*Spinneknapp*/
    const unsigned int spinHeight = static_cast<int>((((windowHeight/5)*2)/4)*3);
    const unsigned int spinWidth = spinHeight;
    TDT4102::Color spinFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point spinPosition {windowWidth/2-spinWidth/2, static_cast<int>(((windowHeight/5)*3)+((((windowHeight/5)*2)-(spinHeight))/2))};
    std::string spinLabel = "Spinn!";
    TDT4102::Button spinButton {spinPosition, spinWidth, spinHeight, spinLabel};
    spinButton.setButtonColor(spinFill);
    window->add(spinButton);


    /*Innsats slider*/
    const unsigned int betHeight = 100;
    const unsigned int betWidth = spinWidth;
    const unsigned int minBet = 1;
    const unsigned int maxBet = 100;
    const unsigned int startBet = 10;
    const unsigned int step = 1;
    const TDT4102::Point betPosition {windowWidth-betWidth-50, static_cast<int>(((windowHeight/5)*3)+((((windowHeight/5)*2)-(betHeight))/2))};
    TDT4102::Slider betSlider {betPosition, betWidth, betHeight, minBet, maxBet, startBet, step};
    
    const TDT4102::Point betTextPosition {windowWidth-betWidth-50, static_cast<int>(((windowHeight/5)*3)+((((windowHeight/5)*2)-(betHeight))/2))+betHeight};
    window->add(betSlider);

    /*Callback-funksjoner*/
    spinButton.setCallback([this, &betSlider]() {spin(betSlider);});

    while (!window->should_close()) {
        window->next_frame();

        //pengeverdi oppdatering
        std::string pointsString = std::to_string(currentPlayer->getMoney());
        
        //innsats oppdatering
        std::string betString = std::to_string(betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);


        //brukernavn og penger på skjerm
        window->draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_text(pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);
        
        //spillvindu
        window->draw_rectangle(playSquarePosition, playSquareWidth, playSquareHeight, playSquareFill, playSquareEdge);
    }
}

void SlotsGame::spin(const TDT4102::Slider& betSlider) {
    int amount = betSlider.getValue();
    currentPlayer->subMoney(amount);
}  