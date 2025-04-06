#pragma once
#include "CardDeck.h"
#include "AnimationWindow.h"
#include "player.h"
#include <vector>
#include <string>


class BlackJackGame {
    private: 
        player* currentPlayer;
        TDT4102::AnimationWindow* window;

        CardDeck deck;
        vector<Card> playerHand;
        vector<Card> dealerHand;
        vector<TDT4102::Image> playerImageHand;
        vector<TDT4102::Image> dealerImageHand;

        int playerHandSum;
        int dealerHandSum;


    public:
        BlackJackGame(player* playerPtr);
        ~BlackJackGame();

        int getCardValue(Card card);
        int getHandScore(vector<Card> hand);

        void drawPlayerCard();
        void drawDealerCard();

        void blackJack();
};

bool isAce(Card card);
bool pictureValue(Card card);