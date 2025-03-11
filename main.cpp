#include "Blackjack.h"
#include <iostream>
using namespace std;

int main() {
    cout << "Let's go gambling!" << endl;

	CardDeck deck = CardDeck();
    deck.shuffle();
    deck.print();
}

//------------------------------------------------------------------------------
