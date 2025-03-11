#pragma once
#include "CardDeck.h"



class BlackJack {
    private: 
        CardDeck deck;
        vector<Card> playerHand;
        vector<Card> dealerHand;

        int playerHandSum;
        int dealerHandSum;
    public:
        int getCardValue(Card card);
        int getHandScore(vector<Card> hand);

        bool askPlayerDrawCard();
        void drawPlayerCard();
        void drawDealerCard();

        void playGame();
};

bool isAce(Card card);