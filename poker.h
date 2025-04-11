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
#include "GameWindow.h"

enum class HandRank {
    HighCard = 1,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush
};

enum GameStateP {
    Betting = 0,
    Flop,
    Turn,
    River,
    Showdown,
    Results,
    Reset
};

struct EvaluatedHand {
    HandRank rank;
    std::vector<int> rankedValues; 
    player* playerPtr;
};


class PokerGame {
private:

    bool flop = false;
    player* CurrentPlayer;     
    std::vector<player*> players;
    int botCount;
    
    CardDeck PokerDeck;      

    // Game state variables
    std::vector<Card*> playerHand;
    std::vector<Card*> table;
    double* pot;
    bool roundInProgress;
    GameStateP gameState;
    GameStateP prevGameState = GameStateP::Betting;

    
    // GUI
    GameWindow* window;
    std::vector<std::shared_ptr<TDT4102::Image>> tableImages;
    std::vector<std::shared_ptr<TDT4102::Image>> handImages;

    // game flow
    void betRound(const TDT4102::Slider& betSlider);
    void dealCards();
    void evaluateHands();
    void resetRound();

    //bot things
    double botMakeBet(player* bot);
    bool botMakeMove();

    /*hand evaluator*/
    std::vector<Card*> getAllCards(player* p, const std::vector<Card*>& table);
    EvaluatedHand evaluatePlayerHand(player* p);
    std::string handRankToString(HandRank rank);
    std::string winnerNameAndHand;
    player* winner; 
    


public:
    PokerGame(player* human, int botCount = 1);
    ~PokerGame();

    void Poker();
};
