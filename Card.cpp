#include "Card.h"
#include <string>

string suitToString(Suit suit) {
    return SuitToStringMap.at(suit);
}

string rankToString(Rank rank) {
    return rankToStringMap.at(rank);
}

Card::Card(Suit suit, Rank rank) {
    s = suit;
    r = rank;
}

Suit Card::getSuit() const{
    return s;
}

Rank Card::getRank() const{
    return r;
}

string Card::toString() const{
    return rankToString(r) + " of " + suitToString(s);
}
