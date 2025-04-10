#pragma once

#include <string>
#include "AnimationWindow.h"
#include "slots.h"
#include "widgets/Button.h"
#include "player.h"
#include "Blackjack.h"

int chooseGame(player* currentPlayer);
void playSlots(player* currentPlayer);
void playBlackJack(player* currentPlayer);
 extern TDT4102::AnimationWindow* choiceWindow;