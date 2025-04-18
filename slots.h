#pragma once

#include "AnimationWindow.h"
#include <string>
#include <fstream>
#include "player.h"
#include "widgets/Button.h"
#include "widgets/Slider.h"
#include "CardDeck.h"
#include <vector>
#include <filesystem>
#include <math.h>
#include "GameWindow.h"


class SlotsGame{
    private:
        player* currentPlayer;
        GameWindow* window;

        Card** slotsMatrix;
        std::string** cardFilePaths;
        TDT4102::Image** slotsImageMatrix;
        CardDeck cardDeck;
        Card card;
        int rows = 4;
        int columns = 7;
        bool spinning = true;
        bool win = false;
        int rowHeight;
    public:
        SlotsGame(player* playerPtr);
        ~SlotsGame();

        void slots();
        void spin(const TDT4102::Slider& betSlider);
        friend std::ostream& operator<<(std::ostream& os, const SlotsGame& game);

        double calculateMult();
};

