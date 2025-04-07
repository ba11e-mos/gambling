#include "Blackjack.h"

BlackJackGame::BlackJackGame(player* playerPtr) : currentPlayer(playerPtr), window(nullptr), gameState(GameState::WaitingForBet) {}

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
    playerImageHand.emplace_back(std::make_shared<TDT4102::Image>(newCard.cardFileName(newCard)));
    return;
}

void BlackJackGame::drawDealerCard() {
    Card newCard = deck.drawCard();
    dealerHand.emplace_back(newCard);
    dealerImageHand.emplace_back(std::make_shared<TDT4102::Image>(newCard.cardFileName(newCard)));
    return;
}


void BlackJackGame::blackJack() {

    /*Spillvindu*/
    std::string windowTitle = "Gambling++";
    window = new TDT4102::AnimationWindow(0, 0, 800, 600, windowTitle);

    const int logicalWidth = 1920;
    const int logicalHeight = 1080;

    SDL_Rect displayBounds;
    float ddpi, hdpi, vdpi;
    if (SDL_GetDisplayBounds(0, &displayBounds) != 0) {
        std::cerr << "Failed to get display bounds: " << SDL_GetError() << std::endl;
        return;
    }

    if (SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi) != 0) {
        std::cerr << "Failed to get display DPI: " << SDL_GetError() << std::endl;
        return;
    }

    const float standardDPI = 96.0f;

    float scaleFactor = (hdpi / standardDPI);

    std::cout << displayBounds.w << "x" << displayBounds.h << std::endl;
    std::cout << scaleFactor << std::endl;


    int windowWidth = static_cast<int>(displayBounds.w*(1.6));
    int windowHeight = static_cast<int>(displayBounds.h*(1.6));

    SDL_Window* sdlWindow = window->getWindowHandle();
    if (!sdlWindow) {
        std::cerr << "Failed to get SDL window handle." << std::endl;
        return;
    }

    if (SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
        std::cerr << "Failed to set fullscreen mode: " << SDL_GetError() << std::endl;
        return;
    }
    std::cout << windowWidth << "x" << windowHeight << std::endl;

    /*Brukernavn*/
    std::string username = currentPlayer->getUsername();
    int nameFontSize = 20;
    const TDT4102::Point namePosition {10, 10};
    

    /*Poeng*/
    int pointsFontSize = 20;
    std::string pointsString = std::to_string(currentPlayer->getMoney());
    int pointsLenght = pointsString.length()*pointsFontSize*0.6;
    const TDT4102::Point pointsPosition {static_cast<int>(windowWidth)-static_cast<int>(10+pointsLenght), 10};

    /*Start spill/bet knapp*/
    const unsigned int betWidth = static_cast<int>(windowWidth/12);
    const unsigned int betHeight = static_cast<int>(betWidth/2);

    TDT4102::Color spinFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point betPosition {windowWidth-betWidth*2, static_cast<int>(windowHeight-betHeight*2)};
    std::string spinLabel = "Bet";
    TDT4102::Button spinButton {betPosition, betWidth, betHeight, spinLabel};
    spinButton.setButtonColor(spinFill);
    spinButton.setLabelColor(TDT4102::Color::black);
    window->add(spinButton);

    /*Innsats slider*/
    const unsigned int betSliderHeight = 100;
    const unsigned int betSliderWidth = betWidth;
    const unsigned int minBet = 1;
    const unsigned int maxBet = 100;
    const unsigned int startBet = 10;
    const unsigned int step = 1;
    const unsigned int betFontSize =  30;
    const TDT4102::Point betSliderPosition {windowWidth-betSliderWidth*2, static_cast<int>(windowHeight-betHeight)};
    TDT4102::Slider betSlider {betSliderPosition, betWidth, betHeight, minBet, maxBet, startBet, step};
    
    const TDT4102::Point betTextPosition {betSliderPosition.x-100, betSliderPosition.y+(betFontSize)};
    window->add(betSlider);

    /*Hit knapp*/
    const unsigned int hitWidth = static_cast<int>(windowWidth/12);
    const unsigned int hitHeight = static_cast<int>(hitWidth/2);

    TDT4102::Color hitFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point hitPosition {windowWidth/2-hitWidth*3, static_cast<int>(windowHeight-hitHeight*2)};
    std::string hitLabel = "Hit";
    TDT4102::Button hitButton {hitPosition, hitWidth, hitHeight, hitLabel};
    spinButton.setButtonColor(hitFill);
    spinButton.setLabelColor(TDT4102::Color::black);
    window->add(hitButton);

    /*Stand knapp*/
    const unsigned int standWidth = static_cast<int>(windowWidth/12);
    const unsigned int standHeight = static_cast<int>(standWidth/2);

    TDT4102::Color standFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point standPosition {windowWidth/2-standWidth*4, static_cast<int>(windowHeight-standHeight*2)};
    std::string standLabel = "Stand";
    TDT4102::Button standButton {standPosition, standWidth, standHeight, standLabel};
    spinButton.setButtonColor(standFill);
    spinButton.setLabelColor(TDT4102::Color::black);
    window->add(standButton);


    /*Bilder av kort*/
    const unsigned int imageWidth = windowWidth/12;
    const unsigned int imageHeight = static_cast<int>(imageWidth*1.5);



   /* Spilltilstand */
   gameState = GameState::WaitingForBet;
   auto lastUpdateTime = std::chrono::steady_clock::now();

   /* Callback-funksjoner */
   spinButton.setCallback([this, &betSlider]() {
       if (gameState != GameState::WaitingForBet) return;
       currentPlayer->subMoney(betSlider.getValue());
       deck = CardDeck();
       deck.shuffle();
       playerHand.clear();
       dealerHand.clear();
       playerImageHand.clear();
       dealerImageHand.clear();   
       gameState = GameState::PlayerFirstCard;
   });

   hitButton.setCallback([this]() { this->hit(); });
   standButton.setCallback([this]() { this->stand(); });



    while (!window->should_close()) {
        window->next_frame();

        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime).count() > 1000) {
            lastUpdateTime = currentTime;

            switch (gameState) {
                case GameState::PlayerFirstCard:
                    drawPlayerCard();
                    std::cout << playerHand << std::endl;
                    std::cout << playerHand.size() << std::endl;
                    gameState = GameState::DealerFirstCard;
                    break;
                case GameState::DealerFirstCard:
                    drawDealerCard();
                    std::cout << dealerHand << std::endl;

                    gameState = GameState::PlayerSecondCard;
                    break;
                case GameState::PlayerSecondCard:
                    drawPlayerCard();
                    std::cout << playerHand << std::endl;
                    std::cout << playerHand.size() << std::endl;
                    gameState = GameState::DealerSecondCard;
                    break;
                case GameState::DealerSecondCard:
                    drawDealerCard();
                    std::cout << dealerHand << std::endl;
                    gameState = GameState::GameInProgress;
                    break;
                case GameState::GameInProgress:
                    if (getHandScore(playerHand) >= 21) {
                        gameState = GameState::ShowDealerHand;
                    }
                    break;
                case GameState::ShowDealerHand:
                    showDealerHand = true;
                    if (getHandScore(dealerHand) < 17) {
                        drawDealerCard();
                        if (getHandScore(dealerHand) > 21) {
                            break;
                        }
                    }
                    gameState = GameState::GameOver;
                    break;
                case GameState::GameOver:

                    //win skjerm
                    gameState = GameState::WaitingForBet;
                    break;
                default:
                    break;
            }
        }

        //Spillerkort
        for (int i = 0; i < playerHand.size(); i++) {
            TDT4102::Point playerImagePosition{
                static_cast<int>(windowWidth / 2 - imageWidth / 2 + (i * (imageWidth * 0.9))),
                static_cast<int>(windowHeight - imageHeight * 1.5)};
            window->draw_image(playerImagePosition, *playerImageHand.at(i), imageWidth, imageHeight);
        }

        //Dealer kort
        if (!showDealerHand) {
            if (!dealerHand.empty()) {
                TDT4102::Point dealerImagePosition{
                    static_cast<int>(windowWidth / 2 - imageWidth / 2),
                    static_cast<int>(0 + imageHeight * 1.5)};
                window->draw_image(dealerImagePosition, *dealerImageHand.at(0), imageWidth, imageHeight);
            }
        } else {
            for (int i = 0; i < dealerHand.size(); i++) {
                TDT4102::Point dealerImagePosition{
                    static_cast<int>(windowWidth / 2 - imageWidth / 2 + (i * (imageWidth * 0.9))),
                    static_cast<int>(0 + imageHeight * 1.5)};

                window->draw_image(dealerImagePosition, *dealerImageHand.at(i), imageWidth, imageHeight);
            }
        }

        //Brukernavn, penger og innsats
        std::string pointsString = currentPlayer->formatDouble(currentPlayer->getMoney());
        window->draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_text(pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);

        std::string betString = std::to_string(betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, betFontSize, TDT4102::Font::courier_bold_italic);
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

std::ostream& operator<<(std::ostream& os, const std::vector<Card>& hand) {
    for (const auto& card : hand) {
        os << card << "\t";
    }
    return os;
}


void BlackJackGame::hit() {
    drawPlayerCard();
}

void BlackJackGame::stand() {
    gameState = GameState::ShowDealerHand;
}

