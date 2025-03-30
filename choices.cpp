#include "choices.h"


int chooseGame(player* currentPlayer) {
    std::string windowTitle = "Gambling++";
    const unsigned int windowHeight = 600;
    const unsigned int windowWidth = 600;
    TDT4102::AnimationWindow window(1920/2-windowWidth/2, 1080/2-windowHeight/2, windowWidth, windowHeight, windowTitle);

    /*Tekst*/
    std::string username = currentPlayer->getUsername();
    std::string info = "Velkommen," + username + "!!";
    int infoFontSize = 30;
    int infoWidth = info.length()*infoFontSize*0.6;
    const TDT4102::Point infoPosition {static_cast<int>(windowWidth / 2) - static_cast<int>(infoWidth / 2), 50};
    window.draw_text(infoPosition, info, TDT4102::Color::black, infoFontSize, TDT4102::Font::courier_bold_italic); 


    /*Slots*/
    const unsigned int buttonWidth = windowWidth/3;
    const unsigned int buttonHeight = windowHeight/7;
    const TDT4102::Point buttonPosition {static_cast<int>((windowWidth / 2) - (buttonWidth / 2)),static_cast<int>((windowHeight / 2) + buttonHeight)};    
    const std::string buttonLabel = "Slots";
    TDT4102::Button button {buttonPosition, buttonWidth, buttonHeight, buttonLabel};
    window.add(button);
    button.setCallback([currentPlayer]() { playSlots(currentPlayer); });

    /*Poker*/

    /*Blackjack*/

    window.wait_for_close();
    return 0;
}

void playSlots(player* currentPlayer){
    SlotsGame SlotsGame(currentPlayer);
    SlotsGame.slots();
    return;
}