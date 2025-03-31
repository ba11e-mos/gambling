#include "Card.h"
#include <string>
#include <filesystem>

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
    auto it = SuitToStringMap.find(suit);
    if (it == SuitToStringMap.end()) {
        std::cerr << "Error: Invalid suit value!" << std::endl;
        return "";
    }
    return it->second;
}

std::string rankToString(Rank rank) {
    auto it = rankToStringMap.find(rank);
    if (it == rankToStringMap.end()) {
        std::cerr << "Error: Invalid rank value!" << std::endl;
        return "";
    }
    return it->second;
}

std::string Card::cardFileName(Card card) {
    std::string suit = suitToString(card.getSuit());
    std::string rank = rankToString(card.getRank());

    if (suit.empty() || rank.empty()) {
        throw std::runtime_error("Invalid card: suit or rank is empty!");
    }


    std::filesystem::path filePath = "assets";
    filePath /= suit + "-" + rank + ".png";

    std::cout << "Generated file path: " << filePath.string() << std::endl;

    return filePath.string();
}