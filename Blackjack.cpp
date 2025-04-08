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
    std::cout << "Player drew: " << newCard.toString() << std::endl;
    return;
}

void BlackJackGame::drawDealerCard() {
    Card newCard = deck.drawCard();
    dealerHand.emplace_back(newCard);
    dealerImageHand.emplace_back(std::make_shared<TDT4102::Image>(newCard.cardFileName(newCard)));
    std::cout << "Dealer drew: " << newCard.toString() << std::endl;

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

    float scaleFactor = 1;//(hdpi / standardDPI);

    std::cout << displayBounds.w << "x" << displayBounds.h << std::endl;
    std::cout << scaleFactor << std::endl;


    int windowWidth = static_cast<int>(displayBounds.w/(scaleFactor));
    int windowHeight = static_cast<int>(displayBounds.h/(scaleFactor));

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
    const TDT4102::Point betPosition {static_cast<int>(windowWidth-betWidth*2), static_cast<int>(windowHeight-betHeight*2)};
    std::string spinLabel = "Bet";
    TDT4102::Button betButton {betPosition, betWidth, betHeight, spinLabel};
    betButton.setButtonColor(spinFill);
    betButton.setLabelColor(TDT4102::Color::black);
    window->add(betButton);

    /*Innsats slider*/
    const unsigned int betSliderHeight = 100;
    const unsigned int betSliderWidth = betWidth;
    const unsigned int minBet = 1;
    const unsigned int maxBet = 100;
    const unsigned int startBet = 10;
    const unsigned int step = 1;
    const unsigned int betFontSize =  30;
    const TDT4102::Point betSliderPosition {static_cast<int>(windowWidth-betSliderWidth*2), static_cast<int>(windowHeight-betHeight)};
    TDT4102::Slider betSlider {betSliderPosition, betWidth, betHeight, minBet, maxBet, startBet, step};
    
    const TDT4102::Point betTextPosition {betSliderPosition.x-100, static_cast<int>(betSliderPosition.y+(betFontSize))};
    window->add(betSlider);

    /*Hit knapp*/
    const unsigned int hitWidth = static_cast<int>(windowWidth/12);
    const unsigned int hitHeight = static_cast<int>(hitWidth/2);

    TDT4102::Color hitFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point hitPosition {static_cast<int>(windowWidth/2-hitWidth*3), static_cast<int>(windowHeight-hitHeight*2)};
    std::string hitLabel = "Hit";
    TDT4102::Button hitButton {hitPosition, hitWidth, hitHeight, hitLabel};
    hitButton.setButtonColor(hitFill);
    hitButton.setLabelColor(TDT4102::Color::black);
    window->add(hitButton);

    /*Stand knapp*/
    const unsigned int standWidth = static_cast<int>(windowWidth/12);
    const unsigned int standHeight = static_cast<int>(standWidth/2);

    TDT4102::Color standFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point standPosition {static_cast<int>(windowWidth/2-standWidth*4), static_cast<int>(windowHeight-standHeight*2)};
    std::string standLabel = "Stand";
    TDT4102::Button standButton {standPosition, standWidth, standHeight, standLabel};
    standButton.setButtonColor(standFill);
    standButton.setLabelColor(TDT4102::Color::black);
    window->add(standButton);


    /*Bilder av kort*/
    const unsigned int imageWidth = windowWidth/12;
    const unsigned int imageHeight = static_cast<int>(imageWidth*1.5);

    /*Kortstokk*/
    std::filesystem::path filePath = "assets";
    filePath /= "card-back.png";
    TDT4102::Image cardBack(filePath.string());

    /*Hånd verdi*/
    int valueFont = 30;
    TDT4102::Point playerHandValue {static_cast<int>(windowWidth*(3.0/4)+valueFont),static_cast<int>(windowHeight/2+valueFont)};
    TDT4102::Point dealerHandValue {static_cast<int>(windowWidth*(3.0/4)+valueFont),static_cast<int>(windowHeight/2-valueFont)};

    /* Spilltilstand */
    gameState = GameState::WaitingForBet;
    auto lastUpdateTime = std::chrono::steady_clock::now();

    /*Win-overlay*/
    TDT4102::Color overlayColor(0, 0, 0, 128); 
    TDT4102::Point overlayPosition {0,0};
    const unsigned int winFontSize = 50;
    double winAmount = 0;
    TDT4102::Point bigWinPos {static_cast<int>((windowWidth/2)-(3.5*winFontSize)), static_cast<int>(windowHeight/2-4*winFontSize)};
    TDT4102::Point winPos {static_cast<int>((windowWidth/2)-(2*winFontSize)), static_cast<int>(windowHeight/2-2*winFontSize)};
    TDT4102::Point yippiPos {static_cast<int>((windowWidth/2)-(1.25*winFontSize)), static_cast<int>(windowHeight/2+2*winFontSize)};
    int delayCounter = 0;

    /*Opretting av kortstokk*/
    deck = CardDeck();
    deck.shuffle();

    /*Animering av korttrekkning*/
    TDT4102::Point startPos{static_cast<int>(windowWidth*(1.0/4)-(imageWidth/3)),static_cast<int>(0+imageHeight/3)};
    TDT4102::Point playerEndPos{
        static_cast<int>(windowWidth / 2 - imageWidth / 2),
        static_cast<int>(windowHeight - imageHeight)};
    TDT4102::Point dealerEndPos{
        static_cast<int>(windowWidth / 2 - imageWidth / 2),
        static_cast<int>(0 + imageHeight / 3)};
    int playerChangeX = static_cast<int>((playerEndPos.x-startPos.x)/90);
    int playerChangeY = static_cast<int>((playerEndPos.y-startPos.y)/90);
    int dealerChangeX = static_cast<int>((dealerEndPos.x-startPos.x)/90);
    int dealerChangeY = static_cast<int>((dealerEndPos.y-startPos.y)/90);

    int animatedCardX = startPos.x;
    int animatedCardY = startPos.y;

    /* Callback-funksjoner */
    betButton.setCallback([this, &betSlider]() {
        if (gameState != GameState::WaitingForBet) return;
        currentPlayer->subMoney(betSlider.getValue());

        playerHand.clear();
        dealerHand.clear();
        playerImageHand.clear();
        dealerImageHand.clear();
        showDealerHand = false;
        gameState = GameState::PlayerFirstCard;
    });

    hitButton.setCallback([this]() {
        if (gameState != GameState::GameInProgress) {return;} 
        this->hit(); 
    });
    standButton.setCallback([this]() { 
        if (gameState != GameState::GameInProgress) {return;}
        this->stand(); 
    });


    while (!window->should_close()) {
        window->next_frame();

        /*Input*/
        bool mouseDown = window->is_left_mouse_button_down();
        bool spaceKeyDown = window->is_key_down(KeyboardKey::SPACE);

    
        switch (gameState) {
            case GameState::PlayerFirstCard:
                std::cout << "PFC" << std::endl;
                prevGameState = GameState::PlayerFirstCard;
                gameState = GameState::AnimatingCard;
                break;

            case GameState::DealerFirstCard:
                std::cout << "DFC" << std::endl;

                prevGameState = GameState::DealerFirstCard;
                gameState = GameState::AnimatingCard;
                break;

            case GameState::PlayerSecondCard:
                std::cout << "PSC" << std::endl;
                prevGameState = GameState::PlayerSecondCard;
                gameState = GameState::AnimatingCard;
                break;

            case GameState::DealerSecondCard:
            std::cout << "DSC" << std::endl;
                prevGameState = GameState::DealerSecondCard;
                gameState = GameState::AnimatingCard;
                break;

            case GameState::GameInProgress:
            std::cout << "GIP" << std::endl;
                if (getHandScore(playerHand) > 21) {
                    gameState = GameState::GameOver;
                }
                break;
            case GameState::AnimatingCard:
                std::cout << "AC" << std::endl;
                if (prevGameState == GameState::PlayerFirstCard) {
                    std::cout << "playerfirstcard" << std::endl;
                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += playerChangeX;
                    animatedCardY += playerChangeY;
                    
                    if (animatedCardX > playerEndPos.x && animatedCardX > playerEndPos.y) {
                        drawPlayerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::DealerFirstCard;
                        break;
                    }
                    break;
                } else if (prevGameState == GameState::DealerFirstCard) {
                    std::cout << "dealerfirstcard" << std::endl;
                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += dealerChangeX;
                    animatedCardY += dealerChangeY;
                    
                    if (animatedCardX > dealerEndPos.x && animatedCardX > dealerEndPos.y) {
                        drawDealerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::PlayerSecondCard;
                        break;
                    }
                    break;

                } else if (prevGameState == GameState::PlayerSecondCard) {
                    std::cout << "playersecondcard" << std::endl;
                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += playerChangeX;
                    animatedCardY += playerChangeY;
                    
                    if (animatedCardX > playerEndPos.x && animatedCardX > playerEndPos.y) {
                        drawPlayerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::DealerSecondCard;
                        break;
                    }
                    break;

                } else if (prevGameState == GameState::DealerSecondCard) {
                    std::cout << "dealersecondcard" << std::endl;
                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += dealerChangeX;
                    animatedCardY += dealerChangeY;
                    
                    if (animatedCardX > dealerEndPos.x && animatedCardX > dealerEndPos.y) {
                        drawDealerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::GameInProgress;
                        break;
                    }
                    break;

                } else if (prevGameState == GameState::GameInProgress) {
                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += playerChangeX;
                    animatedCardY += playerChangeY;
                    
                    if (animatedCardX > playerEndPos.x && animatedCardX > playerEndPos.y) {
                        drawPlayerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::GameInProgress;
                        break;
                    }
                    break;

                } else if (prevGameState == GameState::ShowDealerHand) {
                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += dealerChangeX;
                    animatedCardY += dealerChangeY;
                    
                    if (animatedCardX > dealerEndPos.x && animatedCardX > dealerEndPos.y) {
                        drawDealerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::ShowDealerHand;
                        break;
                    }
                    break;

                }

            case GameState::ShowDealerHand:
                std::cout << "SDH" << std::endl;
                if (delayCounter < 60) {
                    delayCounter += 1;
                    showDealerHand = true;
                    std::cout << showDealerHand << std::endl;
                    prevGameState = GameState::ShowDealerHand;
                    if (getHandScore(dealerHand) < 17) {
                        gameState = GameState::AnimatingCard;
        
                    } else {
                        gameState = GameState::GameOver;
                        delayCounter = 0;
                    }
                }
                    
                break;
            case GameState::BlackJack:
                if (delayCounter < 90) {
                    delayCounter += 1;
                    window->draw_rectangle(overlayPosition, windowWidth, windowHeight, overlayColor);
                    window->draw_text(winPos, "BlackJack", TDT4102::Color::black, winFontSize);            
                    break;
                } else {
                    delayCounter = 0;
                    gameState = GameState::ShowDealerHand; 
                }

            case GameState::GameOver:
                break;

            case GameState::WaitingForBet:
                std::cout << "WFB" << std::endl;

            default:
                break;
        }
        

        //Spillerkort
        for (int i = 0; i < playerHand.size(); i++) {
            TDT4102::Point playerImagePosition{
                static_cast<int>(windowWidth / 2 - imageWidth / 2 + (i * (imageWidth * 0.9))),
                static_cast<int>(windowHeight - imageHeight)};
            window->draw_image(playerImagePosition, *playerImageHand.at(i), imageWidth, imageHeight);
        }

        //Dealerkort
        if (!showDealerHand) {
            if (!dealerHand.empty()) {
                TDT4102::Point dealerImagePosition{
                    static_cast<int>(windowWidth / 2 - imageWidth / 2),
                    static_cast<int>(0 + imageHeight / 3)};
                window->draw_image(dealerImagePosition, *dealerImageHand.at(0), imageWidth, imageHeight);
                if (dealerHand.size() == 2) {
                    TDT4102::Point dealerImagePosition{
                        static_cast<int>(windowWidth / 2 - imageWidth / 2 + (1 * (imageWidth * 0.9))),
                        static_cast<int>(0 + imageHeight / 3)};
                    window->draw_image(dealerImagePosition, cardBack, imageWidth, imageHeight);

                }
            }
        } else {
            for (int i = 0; i < dealerHand.size(); i++) {
                TDT4102::Point dealerImagePosition{
                    static_cast<int>(windowWidth / 2 - imageWidth / 2 + (i * (imageWidth * 0.9))),
                    static_cast<int>(0 + imageHeight / 3)};
                window->draw_image(dealerImagePosition, *dealerImageHand.at(i), imageWidth, imageHeight);
            }
        }

        //Hånd verdi
        if (!(gameState == GameState::WaitingForBet)) {
            if (!playerHand.empty()) {
                std::string playerScore = "You: " + std::to_string(getHandScore(playerHand));
                window->draw_text(playerHandValue, playerScore, TDT4102::Color::black, valueFont);
            } else {
                //std::cerr << "Player hand is empty!" << std::endl;
            }

            if (!showDealerHand) {
                if (!dealerHand.empty()) {
                    std::string dealerScore = "Dealer: " + std::to_string(getCardValue(dealerHand[0])) + "+ ??";
                    window->draw_text(dealerHandValue, dealerScore, TDT4102::Color::black, valueFont);
                } else {
                    //std::cerr << "Dealer hand is empty!" << std::endl;
                }
            } else {
                if (!dealerHand.empty()) {
                    std::string dealerScore = "Dealer: " + std::to_string(getHandScore(dealerHand));
                    window->draw_text(dealerHandValue, dealerScore, TDT4102::Color::black, valueFont);
                } else {
                    //std::cerr << "Dealer hand is empty!" << std::endl;
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            TDT4102::Point cardDeckPosition {static_cast<int>(windowWidth*(1.0/4)-(i*imageWidth/3)),static_cast<int>(0+imageHeight/3)};
            window->draw_image(cardDeckPosition, cardBack, imageWidth, imageHeight);
        }

        if (gameState == GameState::GameOver) {
            if (delayCounter > 60) {
                window->draw_rectangle(overlayPosition, windowWidth, windowHeight, overlayColor);
                std::string result;
                if (getHandScore(playerHand) > 21) {
                    //bust
                    result = "YOU BUSTED!";
                } else if (getHandScore(dealerHand) > 21) {
                    //dealer bust
                    result = "THE DEALER BUSTED!";
                    window->draw_text(winPos, "You Won:", TDT4102::Color::black, winFontSize);            
                    TDT4102::Point amountPos{static_cast<int>((windowWidth / 2) - (((currentPlayer->formatDouble(betSlider.getValue() * 1.5).length() * 0.375)) * winFontSize)), windowHeight / 2};
                    window->draw_text(amountPos, currentPlayer->formatDouble(betSlider.getValue() * 1.5), TDT4102::Color::black, static_cast<int>(winFontSize * 1.5));
                } else if (getHandScore(playerHand) > getHandScore(dealerHand)) {
                    //win
                    result = "YOU WON!";
                    window->draw_text(winPos, "You Won:", TDT4102::Color::black, winFontSize);            
                    TDT4102::Point amountPos{static_cast<int>((windowWidth / 2) - (((currentPlayer->formatDouble(betSlider.getValue() * 1.5).length() * 0.375)) * winFontSize)), windowHeight / 2};
                    window->draw_text(amountPos, currentPlayer->formatDouble(betSlider.getValue() * 1.5), TDT4102::Color::black, static_cast<int>(winFontSize * 1.5));
                } else if (getHandScore(playerHand) < getHandScore(dealerHand)) {
                    //tap
                    result = "THE DEALER WON";
                } else {
                    //push
                    result = "IT'S A TIE";
                    window->draw_text(winPos, "You've won your bet back!", TDT4102::Color::black, winFontSize);            
                }
                window->draw_text(bigWinPos, result, TDT4102::Color::black, winFontSize * 2);
                
                if (mouseDown || spaceKeyDown) {
                    delayCounter = 0;
                    gameState = GameState::WaitingForBet;
                }
            } else {
                delayCounter += 1;
            }
        }

        //Brukernavn, penger og innsats
        pointsString = currentPlayer->formatDouble(currentPlayer->getMoney());
        window->draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_text(pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);

        std::string betString = std::to_string(betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, betFontSize, TDT4102::Font::courier_bold_italic);
    }

}

std::ostream& operator<<(std::ostream& os, const std::vector<Card>& hand) {
    for (const auto& card : hand) {
        os << card << "\t";
    }
    return os;
}


void BlackJackGame::hit() {
    prevGameState = GameState::GameInProgress;
    gameState = GameState::AnimatingCard;
}

void BlackJackGame::stand() {
    gameState = GameState::ShowDealerHand;
}

