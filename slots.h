#pragma once

#include "AnimationWindow.h"
#include <string>
#include <fstream>
#include "player.h"
#include "widgets/Button.h"
#include "widgets/Slider.h"
#include "CardDeck.h"
#include <vector>
#include <algorithm>
#include <filesystem>

struct Layout {

    int pointsLenght;
    TDT4102::Point pointsPosition;

    unsigned int playSquareWidth;
    unsigned int playSquareHeight;
    TDT4102::Point playSquarePosition;

    unsigned int imageWidth;
    unsigned int imageHeight;
    int imageYPos;

    unsigned int spinWidth;
    unsigned int spinHeight;
    TDT4102::Point spinPosition;
    std::string spinLabel;
    TDT4102::Button spinButton;


    TDT4102::Slider betSlider;


    unsigned int betWidth;
    unsigned int betHeight;
    TDT4102::Point betPosition;
    TDT4102::Point betTextPosition;

    TDT4102::Point overlayPosition;
    TDT4102::Point bigWinPos;
    TDT4102::Point winPos;
    TDT4102::Point yippiPos;

    unsigned int windowWidth;
    unsigned int windowHeight;

    Layout() : betSlider({0, 0}, 0, 0, 0, 0, 0, 0), spinButton({0, 0}, 0, 0, spinLabel) {}

};

class SlotsGame{
    private:
        player* currentPlayer;
        TDT4102::AnimationWindow* window;

        Card** slotsMatrix;
        std::string** cardFilePaths;
        TDT4102::Image** slotsImageMatrix;
        CardDeck cardDeck;
        Card card;
        int rows = 3;
        int columns = 5;
        bool spinning = true;
        bool win = false;
        bool jackpot;
        int rowHeight;
    public:
        SlotsGame(player* playerPtr);
        ~SlotsGame();

        void slots();
        void spin(const TDT4102::Slider& betSlider);
        friend std::ostream& operator<<(std::ostream& os, const SlotsGame& game);

        double calculateMult();
        std::string formatDouble(double value);
        void updateLayout(Layout& layout);
        void spinButton(Layout& layout);
    
};


