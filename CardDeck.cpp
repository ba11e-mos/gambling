#include "CardDeck.h"
#include <algorithm> 
#include <random>   
#include <chrono> 
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
using namespace std;


CardDeck::CardDeck() {
    for (int suit = static_cast<int>(Suit::clubs); suit <= static_cast<int>(Suit::spades); ++suit) {
        for (Rank rank : {Rank::two, Rank::three, Rank::four, Rank::five, Rank::six, Rank::seven, Rank::eight, Rank::nine, Rank::ten, Rank::jack, Rank::queen, Rank::king, Rank::ace}) {
            cards.emplace_back(static_cast<Suit>(suit), static_cast<Rank>(rank));
        }
    }
}

void CardDeck::swap(int indeks1, int indeks2) {
    Card temp = cards[indeks1];
    cards[indeks1] = cards[indeks2];
    cards[indeks2] = temp;
}

void CardDeck::print() const {
    map<Suit, vector<Card>> suitMap;
    for (const auto& card : cards) {
        suitMap[card.getSuit()].push_back(card);
    }

    // Print headers
    const int columnWidth = 15;  // Set a fixed column width
    for (const auto& suitPair : suitMap) {
        cout << setw(columnWidth) << suitToString(suitPair.first);
    }
    cout << endl;

    // Find the maximum number of cards in any suit
    size_t maxCards = 0;
    for (const auto& suitPair : suitMap) {
        if (suitPair.second.size() > maxCards) {
            maxCards = suitPair.second.size();
        }
    }

    // Print cards in columns
    for (size_t i = 0; i < maxCards; ++i) {
        for (const auto& suitPair : suitMap) {
            if (i < suitPair.second.size()) {
                cout << setw(columnWidth) << rankToString(suitPair.second[i].getRank());
            } else {
                cout << setw(columnWidth) << " ";
            }
        }
        cout << endl;
    }
}

void CardDeck::shuffle() {
    srand(time(nullptr));
    for (int i = 0; i < cards.size(); i++){
        int ran = rand()%cards.size();
        swap(i,ran);
    }
    
}

Card CardDeck::drawCard() {
    Card last = cards.back();
    cards.pop_back();
    return last;
}
