#include "Card.h"
#include <string>





Card::Card() : s(Suit::clubs), r(Rank::two) {}

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

std::string suitToString(Suit suit) {
    return SuitToStringMap.at(suit);
}

std::string rankToString(Rank rank) {
    return rankToStringMap.at(rank);
}

std::string Card::cardFileName(Card card) {
    std::string suit = suitToString(card.getSuit());
    std::string rank = rankToString(card.getRank());
    std::string filename = "assets/" + suit + "-" + rank + ".png";
    return filename;
}