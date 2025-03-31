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

class SlotsGame{
    private:
        player* currentPlayer;
        TDT4102::AnimationWindow* window;

        Card** slotsMatrix;
        CardDeck cardDeck;
        Card card;
        int rows = 3;
        int columns = 5;
    public:
        SlotsGame(player* playerPtr);
        ~SlotsGame();

        void slots();
        void spin(const TDT4102::Slider& betSlider);
        friend std::ostream& operator<<(std::ostream& os, const SlotsGame& game);

        double calculateMult();

};

