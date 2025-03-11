#include "Blackjack.h"

bool isAce(Card card) {
    return rankToString(card.getRank()) == "ace";
}

bool pictureValue(Card card) {
    string rank = rankToString(card.getRank());
    return rank == "jack" || rank == "queen" || rank == "king";
}

int BlackJack::getCardValue(Card card) {
    if (isAce(card)) {
        return 11;
    } else if (pictureValue(card)) {
        return 10;
    }
    return static_cast<int>(card.getRank());
}

int BlackJack::getHandScore(vector<Card> hand) {
    int tot_value = 0;
    int ace_count = 0;
    for (const auto& card : hand) {
        tot_value += getCardValue(card);
        if (isAce(card)) {
            ace_count++;
        }
    }
    while (tot_value > 21 && ace_count > 0) {
        tot_value -= 10;
        ace_count--;
    }
    return tot_value;
}

bool BlackJack::askPlayerDrawCard() {
    char ans;
    cout << "Draw (Y/N): ";
    cin >> ans;
    if (ans == 'y' || ans == 'Y') {
        return true;
    } else {
        return false;
    }
}

void BlackJack::drawPlayerCard() {
    Card newCard = deck.drawCard();
    playerHand.emplace_back(newCard);
    cout << "Player drew: " << newCard.toString() << endl;
    return;
}

void BlackJack::drawDealerCard() {
    Card newCard = deck.drawCard();
    dealerHand.emplace_back(newCard);
    return;
}

void BlackJack::playGame() {
    char playAgain;
    do {
        deck = CardDeck();
        deck.shuffle();
        deck.print();
        playerHand.clear();
        dealerHand.clear();

        drawPlayerCard();
        drawDealerCard();
        cout << "The dealer's first card is: " << dealerHand[0].toString() << endl;

        drawPlayerCard();
        drawDealerCard();

        cout << "The sum of your cards is: " << getHandScore(playerHand) << endl;

        while (askPlayerDrawCard()) {
            drawPlayerCard();
            cout << "The sum of your cards is: " << getHandScore(playerHand) << endl;
                
            if (getHandScore(playerHand) > 21) {
                cout << "BUST! Your score is over 21. " << endl;
                break;
            }
        }
    
        cout << "The dealers cards are: ";
        for (const auto& card : dealerHand) {
            cout << card.toString() << " ";
        }
        cout << endl;

        cout << "The dealers total hand: " << getHandScore(dealerHand) << endl;
    
        while (getHandScore(dealerHand) < 17) {
            drawDealerCard();
            cout << "The dealer drew: " << dealerHand.back().toString() << endl;
            cout << "The dealers total hand: " << getHandScore(dealerHand) << endl;
            if (getHandScore(dealerHand) > 21) {
                cout << "The dealer has busted." << endl;
                break;
            }
        } 

        if (getHandScore(playerHand) > 21) {
            cout << "You lost!! Better luck next time!" << endl;
        } else if (getHandScore(dealerHand) > 21 || getHandScore(playerHand) > getHandScore(dealerHand)) {
            cout << "You win!! Congratulations!" << endl;
        } else if (getHandScore(playerHand) < getHandScore(dealerHand)) {
            cout << "You lost!! Better luck next time!" << endl;
        } else {
            cout << "It's a tie!" << endl;
        }

        cout << "Do you wanna play again? (Y/N): ";
        cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');
}