#include "choices.h"

TDT4102::AnimationWindow* choiceWindow = nullptr;


int chooseGame(player* currentPlayer) {
    std::string windowTitle = "Gambling++";
    const unsigned int windowHeight = 600;
    const unsigned int windowWidth = 600;
    choiceWindow = new TDT4102::AnimationWindow(1920/2-windowWidth/2, 1080/2-windowHeight/2, windowWidth, windowHeight, windowTitle);

    /*Tekst*/
    std::string username = currentPlayer->getUsername();
    std::string info = "Velkommen," + username + "!!";
    int infoFontSize = 30;
    int infoWidth = info.length()*infoFontSize*0.6;
    const TDT4102::Point infoPosition {static_cast<int>(windowWidth / 2) - static_cast<int>(infoWidth / 2), 50};
    choiceWindow->draw_text(infoPosition, info, TDT4102::Color::black, infoFontSize, TDT4102::Font::courier_bold_italic); 


    /*Slots*/
    const unsigned int buttonWidth = windowWidth/3;
    const unsigned int buttonHeight = windowHeight/7;

    const TDT4102::Point slotsButtonPosition {static_cast<int>((windowWidth / 2) - (buttonWidth / 2)),static_cast<int>((windowHeight / 2) + buttonHeight)};    
    const std::string slotsButtonLabel = "Slots";
    TDT4102::Button slotsButton {slotsButtonPosition, buttonWidth, buttonHeight, slotsButtonLabel};
    choiceWindow->add(slotsButton);
    slotsButton.setCallback([currentPlayer]() { playSlots(currentPlayer); });

    /*Poker*/
    const std::string PokerButtonLabel = "Poker";
    const TDT4102::Point PokerButtonPos {static_cast<int>((windowWidth / 2) - (buttonWidth / 2)), static_cast<int>(windowHeight / 2)};
    TDT4102::Button PokerButton {PokerButtonPos, buttonWidth, buttonHeight, PokerButtonLabel};
    window.add(PokerButton);
    PokerButton.setCallback([currentPlayer]() { PlayPoker(currentPlayer); } );

    /*Blackjack*/
    const TDT4102::Point BJButtonPosition {static_cast<int>((windowWidth / 2) - (buttonWidth / 2)),static_cast<int>((windowHeight / 2) - buttonHeight)};    
    const std::string BJButtonLabel = "BlackJack";
    TDT4102::Button BJButton {BJButtonPosition, buttonWidth, buttonHeight, BJButtonLabel};
    choiceWindow->add(BJButton);
    BJButton.setCallback([currentPlayer]() { playBlackJack(currentPlayer); });


    choiceWindow->wait_for_close();
    return 0;
}

void playSlots(player* currentPlayer){
    SlotsGame SlotsGame(currentPlayer);
    std::cout << "Slots" << std::endl;
    delete choiceWindow;
    SlotsGame.slots();
    std::cout << "Slots ferdig" << std::endl;
    return;
}

void playBlackJack(player* currentPlayer){
    BlackJackGame BlackJackGame(currentPlayer);
    std::cout << "BJ" << std::endl;
    delete choiceWindow;
    BlackJackGame.blackJack();
    std::cout << "BJ ferdig" << std::endl;
    return;
}

void PlayPoker(player* currentPlayer){
    PokerGame PokerGame(currentPlayer, 1);
    PokerGame.Poker();
}
