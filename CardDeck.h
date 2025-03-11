#pragma once
#include "Card.h"
#include <vector>
using namespace std;

// BEGIN: 3a
class CardDeck {
    public:
        vector<Card> cards;
        CardDeck(); 
        void swap(int indeks1, int indeks2);
        void print() const;
        void shuffle();
        Card drawCard();
};
// END: 3a
