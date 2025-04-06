#include "Blackjack.h"

BlackJackGame::BlackJackGame(player* playerPtr) : currentPlayer(playerPtr), window(nullptr) {}

BlackJackGame::~BlackJackGame() {
    delete window;
}


bool isAce(Card card) {
    return rankToString(card.getRank()) == "ace";
}

bool pictureValue(Card card) {
    std::string rank = rankToString(card.getRank());
    return rank == "jack" || rank == "queen" || rank == "king";
}

int BlackJackGame::getCardValue(Card card) {
    if (isAce(card)) {
        return 11;
    } else if (pictureValue(card)) {
        return 10;
    }
    return static_cast<int>(card.getRank());
}

int BlackJackGame::getHandScore(vector<Card> hand) {
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


void BlackJackGame::drawPlayerCard() {
    Card newCard = deck.drawCard();
    playerHand.emplace_back(newCard);
    playerImageHand.emplace_back(TDT4102::Image(newCard.cardFileName(newCard)));
    return;
}

void BlackJackGame::drawDealerCard() {
    Card newCard = deck.drawCard();
    dealerHand.emplace_back(newCard);
    dealerImageHand.emplace_back(TDT4102::Image(newCard.cardFileName(newCard)));
    return;
}

void BlackJackGame::blackJack() {
    

    /*dette er en scuffed metode å hente skjermstørrelse*/
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    }
    SDL_Rect displayBounds;
    if (SDL_GetDisplayBounds(0, &displayBounds) != 0) {
        std::cerr << "Failed to get display bounds: " << SDL_GetError() << std::endl;
        SDL_Quit(); 
    }
    int screenWidth = displayBounds.w;
    int screenHeight = displayBounds.h;


    /*Spillvindu*/
    std::string windowTitle = "Gambling++";
    const unsigned int windowHeight = static_cast<unsigned int>((screenHeight*3)/4);
    const unsigned int windowWidth = static_cast<unsigned int>((screenWidth*2)/3);
    window = new TDT4102::AnimationWindow(static_cast<int>(screenWidth/2-windowWidth/2), static_cast<int>(screenHeight/2-windowHeight/2), windowWidth, windowHeight, windowTitle);

    /*Brukernavn*/
    std::string username = currentPlayer->getUsername();
    int nameFontSize = 20;
    const TDT4102::Point namePosition {10, 10};
    

    /*Poeng*/
    int pointsFontSize = 20;
    std::string pointsString = std::to_string(currentPlayer->getMoney());
    int pointsLenght = pointsString.length()*pointsFontSize*0.6;
    const TDT4102::Point pointsPosition {static_cast<int>(windowWidth)-static_cast<int>(10+pointsLenght), 10};

    /*Bilder av kort*/
    const unsigned int imageWidth = windowWidth/12;
    const unsigned int imageHeight = static_cast<int>(imageWidth*1.5);


    /*for testing*/
    deck = CardDeck();
    deck.shuffle();
    deck.print();
    playerHand.clear();
    dealerHand.clear();
    drawPlayerCard();
    drawDealerCard();
    drawPlayerCard();
    drawDealerCard();

    while (!window->should_close()) {
        window->next_frame();

        //pengeverdi oppdatering
        std::string pointsString = currentPlayer->formatDouble(currentPlayer->getMoney());

        //brukernavn og penger på skjerm
        window->draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_text(pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);
        
        for (int i = 0; i < playerHand.size(); i++) {
            TDT4102::Point playerImagePosition{static_cast<int>(windowWidth/2-imageWidth/2+(i*(imageWidth*0.8))), static_cast<int>(windowHeight-imageHeight*1.5-(i*(imageHeight*0.8)))};
            window->draw_image(playerImagePosition, playerImageHand[i], imageWidth, imageHeight);
        }
    }

    /*
    char playAgain;
    do {


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

        std::cout << "The dealers cards are: ";
        for (const auto& card : dealerHand) {
            std::cout << card.toString() << " ";
        }
        std::cout << std::endl;

        std::cout << "The dealers total hand: " << getHandScore(dealerHand) << std::endl;
    
        while (getHandScore(dealerHand) < 17) {
            drawDealerCard();
            std::cout << "The dealer drew: " << dealerHand.back().toString() << std::endl;
            std::cout << "The dealers total hand: " << getHandScore(dealerHand) << std::endl;
            if (getHandScore(dealerHand) > 21) {
                std::cout << "The dealer has busted." << std::endl;
                break;
            }
        } 

        if (getHandScore(playerHand) > 21) {
            std::cout << "You lost!! Better luck next time!" << std::endl;
        } else if (getHandScore(dealerHand) > 21 || getHandScore(playerHand) > getHandScore(dealerHand)) {
            std::cout << "You win!! Congratulations!" << std::endl;
        } else if (getHandScore(playerHand) < getHandScore(dealerHand)) {
            std::cout << "You lost!! Better luck next time!" << std::endl;
        } else {
            std::cout << "It's a tie!" << std::endl;
        }

        std::cout << "Do you wanna play again? (Y/N): ";
        std::cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');
    */
}


