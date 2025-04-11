#pragma once
#include "CardDeck.h"
#include "AnimationWindow.h"
#include "player.h"
#include <vector>
#include <string>
#include "widgets/Button.h"
#include "widgets/Slider.h"
#include <thread>
#include <chrono>
#include "GameWindow.h"

enum class GameState {
    WaitingForBet,
    PlayerFirstCard,
    DealerFirstCard,
    PlayerSecondCard,
    DealerSecondCard,
    GameInProgress,
    AnimatingCard,
    ShowDealerHand,
    BlackJack,
    GameOver
};

class BlackJackGame {
    private: 
        player* currentPlayer;
        GameWindow* window;
        
        CardDeck deck;
        vector<Card> playerHand;
        vector<Card> dealerHand;
        vector<std::shared_ptr<TDT4102::Image>> playerImageHand;
        vector<std::shared_ptr<TDT4102::Image>> dealerImageHand;

        bool showDealerHand = false;

        GameState gameState;
        GameState prevGameState;

    public:
        BlackJackGame(player* playerPtr);
        ~BlackJackGame();

        int getCardValue(Card card);
        int getHandScore(vector<Card> hand);

        void drawPlayerCard();
        void drawDealerCard();

        void blackJack();

        void hit();
        void stand();
        void split();
        void doubleDown();

};

bool isAce(Card card);
bool pictureValue(Card card);

std::ostream& operator<<(std::ostream& os, const std::vector<Card>& hand);

