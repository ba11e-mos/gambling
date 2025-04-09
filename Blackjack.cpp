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
    betButton.setLabelColor(TDT4102::Color::white);
    betButton.setButtonColor(TDT4102::Color::dark_red);
    window->add(betButton);

    /*Innsats slider*/
    const unsigned int betSliderHeight = 100;
    const unsigned int betSliderWidth = betWidth;
    const unsigned int minBet = 10;
    const unsigned int maxBet = 1000;
    const unsigned int startBet = 100;
    const unsigned int step = 10;
    const unsigned int betFontSize =  30;
    const TDT4102::Point betSliderPosition {static_cast<int>(windowWidth-betSliderWidth*2), static_cast<int>(windowHeight-betHeight)};
    TDT4102::Slider betSlider {betSliderPosition, betWidth, betHeight, minBet, maxBet, startBet, step};
    int bet = 0;
    const TDT4102::Point betTextPosition {betSliderPosition.x-100, static_cast<int>(betSliderPosition.y+(betFontSize))};
    window->add(betSlider);

    /*Hit knapp*/
    const unsigned int hitWidth = static_cast<int>(windowWidth/12);
    const unsigned int hitHeight = hitWidth;
    TDT4102::Color seethroughColor(0, 0, 0, 96);
    const TDT4102::Point hitPosition {static_cast<int>(windowWidth/3), static_cast<int>(windowHeight*(3.0/4) - hitHeight/2)};
    std::string hitLabel = "Hit";
    TDT4102::Button hitButton {hitPosition, hitWidth, hitHeight, hitLabel};
    std::filesystem::path fileUCard = "assets";
    fileUCard /= "spades-unknown.png";
    TDT4102::Image unknownCard(fileUCard.string());
    hitButton.setButtonColor(seethroughColor);
    hitButton.setLabelColor(TDT4102::Color::white);
    hitButton.setButtonColor(TDT4102::Color::transparent);
    hitButton.setButtonColorHover(seethroughColor);
    window->add(hitButton);



    /*Stand knapp*/
    const unsigned int standWidth = static_cast<int>(windowWidth/12);
    const unsigned int standHeight = standWidth;
    const TDT4102::Point standPosition {static_cast<int>(windowWidth/3), static_cast<int>(windowHeight*(3.0/4)+ standHeight/2)};
    std::string standLabel = "Stand";
    TDT4102::Button standButton {standPosition, standWidth, standHeight, standLabel};
    std::filesystem::path fileCross = "assets";
    fileCross /= "cross.png";
    TDT4102::Image cross(fileCross.string());
    standButton.setButtonColor(TDT4102::Color::transparent);
    standButton.setLabelColor(TDT4102::Color::white);
    standButton.setButtonColor(TDT4102::Color::transparent);
    standButton.setButtonColorHover(seethroughColor);
    window->add(standButton);



    /*Bilder av kort*/
    const unsigned int imageWidth = static_cast<int>(windowWidth/14);
    const unsigned int imageHeight = static_cast<int>(imageWidth*1.5);

    /*Kortstokk*/
    std::filesystem::path fileCardBack = "assets";
    fileCardBack /= "card-back.png";
    TDT4102::Image cardBack(fileCardBack.string());

    /*Hånd verdi*/
    int valueFont = 30;
    TDT4102::Point playerHandValue {static_cast<int>(windowWidth*(3.0/4)+valueFont),static_cast<int>(windowHeight/2+valueFont)};
    TDT4102::Point dealerHandValue {static_cast<int>(windowWidth*(3.0/4)+valueFont),static_cast<int>(windowHeight/2-valueFont)};
    TDT4102::Font font = TDT4102::Font::pixeloid;


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

    /*Bakgrunn*/
    TDT4102::Color bgColor(0x00556b2f); 
    TDT4102::Point bgPosition {0,0};
    std::filesystem::path fileDecal = "assets";
    fileDecal /= "blackJackTable.png";
    TDT4102::Image decal(fileDecal.string());
    static unsigned int decalWidth = static_cast<int>((windowWidth/3));
    static unsigned int decalHeight = static_cast<int>((decalWidth/3.5));
    TDT4102::Point decalPos {static_cast<int>((windowWidth/2)-(decalWidth/2)), static_cast<int>(windowHeight/2-decalHeight/2)};

    /*Opretting av kortstokk*/
    deck = CardDeck();
    deck.shuffle();

    /*Kortsirkel*/
    std::filesystem::path fileCircle = "assets";
    fileCircle /= "circle.png";
    TDT4102::Image circle(fileCircle.string());
    static unsigned int circleWidth = static_cast<int>((windowWidth/6));
    static unsigned int circleHeight = static_cast<int>((circleWidth));
    TDT4102::Point cardCirclePos {static_cast<int>(windowWidth/2-circleWidth/2), static_cast<int>(windowHeight - circleHeight*1.25)};
    

    /*Animering av korttrekkning*/
    TDT4102::Point startPos{static_cast<int>(windowWidth*(7.0/8)+(imageWidth/3)),static_cast<int>(0+imageHeight/3)};
    TDT4102::Point playerEndPos{
        static_cast<int>(windowWidth / 2 - circleWidth/2 + (imageWidth * 0.5)),
        static_cast<int>(windowHeight - circleHeight*1.25)};
    TDT4102::Point dealerEndPos{
        static_cast<int>(windowWidth / 2 - imageWidth / 2),
        static_cast<int>(0 + imageHeight / 3)};
    int playerChangeX = static_cast<int>((playerEndPos.x-startPos.x)/60);
    int playerChangeY = static_cast<int>((playerEndPos.y-startPos.y)/60);
    int dealerChangeX = static_cast<int>((dealerEndPos.x-startPos.x)/60);
    int dealerChangeY = static_cast<int>((dealerEndPos.y-startPos.y)/60);

    int animatedCardX = startPos.x;
    int animatedCardY = startPos.y;

    /* Callback-funksjoner */
    betButton.setCallback([this, &betSlider, &bet]() {
        if (gameState != GameState::WaitingForBet) return;
        bet = betSlider.getValue();
        currentPlayer->subMoney(bet);

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
        bool escDown = window->is_key_down(KeyboardKey::ESCAPE);

        /*Bakgrunn*/
        window->draw_rectangle(bgPosition, windowWidth, windowHeight, bgColor);
        window->draw_image(decalPos, decal, decalWidth, decalHeight);
        window->draw_image(cardCirclePos, circle, circleWidth, circleHeight);

    
        /*Kulere knapper*/
        for (int i = 0; i < 2; i++) {
            window->draw_image({static_cast<int>((hitPosition.x + hitWidth / 2) - (imageWidth / 8) + (i * (imageWidth/8) )), static_cast<int>((hitPosition.y + hitHeight / 2) - (imageHeight / 8))}, unknownCard, imageWidth/4, imageHeight/4);
        }
        window->draw_image({static_cast<int>(standPosition.x+(standWidth/4)), static_cast<int>(standPosition.y+(standHeight/4))}, cross, standWidth/2, standHeight/2);


    
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
                    std::cout << "PFCAC" << std::endl;
                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += playerChangeX;
                    animatedCardY += playerChangeY;
                    
                    if (animatedCardX <= playerEndPos.x && animatedCardX >= playerEndPos.y) {
                        drawPlayerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::DealerFirstCard;
                        break;
                    }
                    break;
                } else if (prevGameState == GameState::DealerFirstCard) {
                    std::cout << "DFCAC" << std::endl;

                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += dealerChangeX;
                    animatedCardY += dealerChangeY;
                    
                    if (animatedCardX <= dealerEndPos.x && animatedCardY >= dealerEndPos.y) {
                        drawDealerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::PlayerSecondCard;
                        break;
                    }
                    break;

                } else if (prevGameState == GameState::PlayerSecondCard) {
                    std::cout << "PSCAC" << std::endl;

                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += playerChangeX;
                    animatedCardY += playerChangeY;
                    
                    if (animatedCardX <= playerEndPos.x && animatedCardX >= playerEndPos.y) {
                        drawPlayerCard();
                        animatedCardX = startPos.x;
                        animatedCardY = startPos.y;
                        gameState = GameState::DealerSecondCard;
                        break;
                    }
                    break;

                } else if (prevGameState == GameState::DealerSecondCard) {
                    std::cout << "DSCAC" << std::endl;

                    TDT4102::Point animationPos{animatedCardX, animatedCardY};
                    window->draw_image(animationPos, cardBack, imageWidth, imageHeight);
                    animatedCardX += dealerChangeX;
                    animatedCardY += dealerChangeY;
                    
                    if (animatedCardX <= dealerEndPos.x && animatedCardY >= dealerEndPos.y) {
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
                    
                    if (animatedCardX <= playerEndPos.x && animatedCardX >= playerEndPos.y) {
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
                    
                    if (animatedCardX <= dealerEndPos.x && animatedCardY >= dealerEndPos.y) {
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
                //denne brukes ikke rn
                if (delayCounter < 90) {
                    delayCounter += 1;
                    window->draw_rectangle(overlayPosition, windowWidth, windowHeight, overlayColor);
                    window->draw_text(winPos, "BlackJack", TDT4102::Color::black, winFontSize, font);            
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
                static_cast<int>(windowWidth / 2 - circleWidth/2 + (i * (imageWidth * 0.5))),
                static_cast<int>(windowHeight - circleHeight*1.25)};
            window->draw_image(playerImagePosition, *playerImageHand.at(i), imageWidth, imageHeight);
        }

        //Dealerkort
        if (!showDealerHand) {
            if (!dealerHand.empty()) {
                TDT4102::Point dealerImagePosition{
                    static_cast<int>(windowWidth / 2 - circleWidth / 2),
                    static_cast<int>(0 + imageHeight / 3)};
                window->draw_image(dealerImagePosition, *dealerImageHand.at(0), imageWidth, imageHeight);
                if (dealerHand.size() == 2) {
                    TDT4102::Point dealerImagePosition{
                        static_cast<int>(windowWidth / 2 - circleWidth / 2 + (1 * (imageWidth * 0.5))),
                        static_cast<int>(0 + imageHeight / 3)};
                    window->draw_image(dealerImagePosition, cardBack, imageWidth, imageHeight);

                }
            }
        } else {
            for (int i = 0; i < dealerHand.size(); i++) {
                TDT4102::Point dealerImagePosition{
                    static_cast<int>(windowWidth / 2 - circleWidth / 2 + (i * (imageWidth * 0.5))),
                    static_cast<int>(0 + imageHeight / 3)};
                window->draw_image(dealerImagePosition, *dealerImageHand.at(i), imageWidth, imageHeight);
            }
        }

        //Hånd verdi
        if (!(gameState == GameState::WaitingForBet)) {
            if (!playerHand.empty()) {
                std::string playerScore = "You: " + std::to_string(getHandScore(playerHand));
                window->draw_text(playerHandValue, playerScore, TDT4102::Color::white, valueFont, font);
            } else {
                //std::cerr << "Player hand is empty!" << std::endl;
            }

            window->draw_line({static_cast<int>(dealerHandValue.x - (valueFont)), static_cast<int>(dealerHandValue.y + (playerHandValue.y-dealerHandValue.y)/2 + valueFont*0.6)}, {static_cast<int>(dealerHandValue.x + (valueFont * 9)), static_cast<int>(dealerHandValue.y + (playerHandValue.y-dealerHandValue.y)/2 + valueFont*0.6)}, TDT4102::Color::white);

            if (!showDealerHand) {
                if (!dealerHand.empty()) {
                    std::string dealerScore = "Dealer: " + std::to_string(getCardValue(dealerHand[0])) + " + ??";
                    window->draw_text(dealerHandValue, dealerScore, TDT4102::Color::white, valueFont, font);
                } else {
                    //std::cerr << "Dealer hand is empty!" << std::endl;
                }
            } else {
                if (!dealerHand.empty()) {
                    std::string dealerScore = "Dealer: " + std::to_string(getHandScore(dealerHand));
                    window->draw_text(dealerHandValue, dealerScore, TDT4102::Color::white, valueFont, font);
                } else {
                    //std::cerr << "Dealer hand is empty!" << std::endl;
                }
            }
        }

        /*Kortstokk*/
        for (int i = 0; i < 3; i++) {
            TDT4102::Point cardDeckPosition {static_cast<int>(windowWidth*(7.0/8)+(i*imageWidth/3)),static_cast<int>(0+imageHeight/3)};
            window->draw_image(cardDeckPosition, cardBack, imageWidth, imageHeight);
        }

        /*Win overlay*/
        if (gameState == GameState::GameOver) {
            if (delayCounter > 60) {
                window->draw_rectangle(overlayPosition, windowWidth, windowHeight, overlayColor);
        
                std::string result;
                if (getHandScore(playerHand) > 21) {
                    // Bust
                    result = "YOU BUSTED!";
                } else if (getHandScore(dealerHand) > 21) {
                    // Dealer bust
                    result = "THE DEALER BUSTED!";
                } else if (getHandScore(playerHand) > getHandScore(dealerHand)) {
                    // Win
                    result = "WINNER WINNER, CHICKEN DINNER!";
                } else if (getHandScore(playerHand) < getHandScore(dealerHand)) {
                    // Loss
                    result = "THE DEALER WON";
                } else {
                    // Push
                    result = "IT'S A TIE";
                }
        
                TDT4102::Point resultPos{
                    static_cast<int>((windowWidth / 2) - (result.length() * winFontSize * 0.5)),
                    static_cast<int>(windowHeight / 2 - winFontSize)
                };
        
                window->draw_text(resultPos, result, TDT4102::Color::white, winFontSize * 2, font);
        
                if (result == "WINNER WINNER, CHICKEN DINNER!" || result == "THE DEALER BUSTED!") {
                    std::string winText = "You Won: " + std::to_string(static_cast<int>(bet*1.5));
                    TDT4102::Point winTextPos{
                        static_cast<int>((windowWidth / 2) - ((winText.length()) * winFontSize * 0.375)),
                        static_cast<int>(windowHeight / 2 + winFontSize)
                    };

                    
                    window->draw_text(winTextPos, winText, TDT4102::Color::white, static_cast<int>(winFontSize * 1.5), font);
                }
        
                if (mouseDown || spaceKeyDown) {
                    delayCounter = 0;
                    currentPlayer->addMoney(bet * 1.5);
                    gameState = GameState::WaitingForBet;
                    

                }
            } else {
                delayCounter += 1;
            }
        }

        

        //Brukernavn, penger og innsats
        pointsString = currentPlayer->formatDouble(currentPlayer->getMoney());
        window->draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, font);
        window->draw_text(pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, font);

        std::string betString = std::to_string(betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, betFontSize, font);
    
        if (deck.cards.size() < 10) {
            std::cout << "Stokker om kortstokken" << std::endl;
            deck.resetDeck();
            deck.shuffle();
        }
        
        if (escDown) {
            std::exit(EXIT_SUCCESS);
        }
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

