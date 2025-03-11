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

    const int columnWidth = 15; 
    for (const auto& suitPair : suitMap) {
        cout << setw(columnWidth) << suitToString(suitPair.first);
    }
    cout << endl;

    size_t maxCards = 0;
    for (const auto& suitPair : suitMap) {
        if (suitPair.second.size() > maxCards) {
            maxCards = suitPair.second.size();
        }
    }

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
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::shuffle(cards.begin(), cards.end(), rng);
}

Card CardDeck::drawCard() {
    Card last = cards.back();
    cards.pop_back();
    return last;
}
