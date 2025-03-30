#pragma once

#include "AnimationWindow.h"
#include <string>
#include <fstream>
#include "player.h"
#include "widgets/Button.h"
#include "widgets/Slider.h"

class SlotsGame{
    private:
        player* currentPlayer;
        TDT4102::AnimationWindow* window;
    public:
        SlotsGame(player* playerPtr);
        ~SlotsGame();

        void slots();
        void spin(const TDT4102::Slider& betSlider);

};

