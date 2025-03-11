#include "CardDeck.h"
#include <algorithm> 
#include <random>   
#include <chrono>    

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
    for (const auto& card : cards) {
        string suit = suitToString(const_cast<Card&>(card).getSuit());
        string rank = rankToString(const_cast<Card&>(card).getRank());
        cout << rank << " of " << suit << endl;
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
