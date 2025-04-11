#include "slots.h"

SlotsGame::SlotsGame(player* playerPtr) : currentPlayer(playerPtr), window(new GameWindow(800, 600, "Slots++", playerPtr->getUsername(), playerPtr->getMoney())) {
    slotsMatrix = new Card*[rows];
    cardFilePaths = new std::string*[rows];
    slotsImageMatrix = new TDT4102::Image*[rows];

    for (int i = 0; i < rows; i++) {
        slotsMatrix[i] = new Card[columns];
        cardFilePaths[i] = new std::string[columns];
        slotsImageMatrix[i] = new TDT4102::Image[columns];
        for (int j = 0; j < columns; j++) {
            std::filesystem::path filePath = "assets";
            filePath /= "spades-unknown.png";
            cardFilePaths[i][j] = filePath.string();
            slotsImageMatrix[i][j] = TDT4102::Image(cardFilePaths[i][j]);
        }

    }
}

SlotsGame::~SlotsGame() {
    delete window;
    for (int i = 0; i < rows; i++) {
        delete[] slotsMatrix[i];
        delete[] cardFilePaths[i];
        delete[] slotsImageMatrix[i];
    }
    delete[] slotsMatrix;
    delete[] cardFilePaths;
    delete[] slotsImageMatrix;
}

void SlotsGame::slots() {
    /*Fullskjerm*/
    window->setFullscreen(true);

    int windowWidth, windowHeight;
    window->getWindowSize(windowWidth, windowHeight);

    std::cout << "Vindusstørrelse: " << windowWidth << "x" << windowHeight << std::endl;

    /*Brukernavn*/
    std::string username = currentPlayer->getUsername();
    int nameFontSize = 20;
    const TDT4102::Point namePosition {10, 10};
    

    /*Poeng*/
    int pointsFontSize = 20;
    TDT4102::Font font = TDT4102::Font::pixeloid;
    std::string pointsString = std::to_string(currentPlayer->getMoney());
    int pointsLenght = pointsString.length()*pointsFontSize*0.6;
    const TDT4102::Point pointsPosition {static_cast<int>(windowWidth)-static_cast<int>(10+pointsLenght), 10};

    /*Spillvindu*/
    const unsigned int playSquareHeight = windowHeight-200;
    const unsigned int playSquareWidth = static_cast<int>(playSquareHeight*(7.0/6));
    
    TDT4102::Color playSquareFill = TDT4102::Color::white;
    TDT4102::Color playSquareEdge = TDT4102::Color::black;
    const TDT4102::Point playSquarePosition {static_cast<int>(windowWidth/2 - playSquareWidth/2), 50};


    /*Slots matrise*/
    const unsigned int imageWidth = (playSquareWidth-100)/columns;
    const unsigned int imageHeight = static_cast<int>(imageWidth*1.5);
    int rowHeight = imageHeight+10;
    int columnWidth = imageWidth+10;
    int imageYPos = -(rowHeight*rows);
    int slidePos = (playSquareHeight+50)/60; 
    

    /*Spinneknapp*/
    const unsigned int spinHeight = static_cast<int>((windowHeight-playSquareHeight-playSquarePosition.y)/2);
    const unsigned int spinWidth = static_cast<int>(spinHeight*2);
    
    
    TDT4102::Color spinFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point spinPosition {static_cast<int>(windowWidth/2-spinWidth/2), static_cast<int>((playSquarePosition.y+playSquareHeight)+spinHeight/2)};
    std::string spinLabel = "Spinn!";
    TDT4102::Button spinButton {spinPosition, spinWidth, spinHeight, spinLabel};
    spinButton.setButtonColor(spinFill);
    spinButton.setLabelColor(TDT4102::Color::black);
    window->add(spinButton);


    /*Innsats slider*/
    const unsigned int betHeight = 100;
    const unsigned int betWidth = spinWidth;
    const unsigned int minBet = 1;
    const unsigned int maxBet = 100;
    const unsigned int startBet = 10;
    const unsigned int step = 1;
    const unsigned int betFontSize =  30;
    const TDT4102::Point betPosition {static_cast<int>(windowWidth-betWidth*2), static_cast<int>(playSquareHeight+betHeight)};
    TDT4102::Slider betSlider {betPosition, betWidth, betHeight, minBet, maxBet, startBet, step};
    
    const TDT4102::Point betTextPosition {static_cast<int>(windowWidth-betWidth*2-50), static_cast<int>(playSquareHeight+betHeight+betFontSize*0.85)};
    window->add(betSlider);

    /*Bakgrunn*/
    TDT4102::Color bgColor(0x00556b2f); 
    TDT4102::Point bgPosition {0,0};


    /*Win-overlay*/
    TDT4102::Color overlayColor(0, 0, 0, 128);
    TDT4102::Point overlayPosition {0,0};
    const unsigned int winFontSize = 50;
    double winAmount = 0;
    bool jackpot = false;
    TDT4102::Point bigWinPos {static_cast<int>((windowWidth/2)-(3.5*winFontSize)), static_cast<int>(windowHeight/2-4*winFontSize)};
    TDT4102::Point winPos {static_cast<int>((windowWidth/2)-(2*winFontSize)), static_cast<int>(windowHeight/2-2*winFontSize)};
    TDT4102::Point yippiPos {static_cast<int>((windowWidth/2)-(1.25*winFontSize)), static_cast<int>(windowHeight/2+2*winFontSize)};

    TDT4102::Point winTextPos {static_cast<int>((windowWidth/2)-200), static_cast<int>(playSquareHeight+50)};

    

    /*Callback-funksjoner*/
    spinButton.setCallback([this, &betSlider, &imageYPos, slidePos, &rowHeight, &jackpot]() {
        if (spinning) return;
        if (jackpot) return;
        spin(betSlider);
        spinning = true;
        imageYPos = -(rowHeight*rows);

    });

    while (!window->should_close()) {
        window->next_frame();

        /*Input*/
        bool mouseDown = window->is_left_mouse_button_down();
        bool spaceKeyDown = window->is_key_down(KeyboardKey::SPACE);
        bool escDown = window->is_key_down(KeyboardKey::ESCAPE);

        if (!spinning and !jackpot) {
            if (spaceKeyDown) {
                spin(betSlider);
                spinning = true;
                imageYPos = -(rowHeight*rows);
            } 
        }
        
        //Bakgrunn
        window->drawBackground(bgColor);


        //innsats oppdatering
        std::string betString = std::to_string(betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, betFontSize, font);


        //brukernavn og penger på skjerm
        window->drawUsernameAndMoney({10, 10}, {windowWidth, 10});

        
        //spillvindu
        window->draw_rectangle(playSquarePosition, playSquareWidth, playSquareHeight, playSquareFill, playSquareEdge);
        if (spinning) {
            imageYPos += slidePos;
            if (imageYPos > 50) {
                spinning = false;
            }
        }

        //Slots matrise
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                TDT4102::Point imagePosition{static_cast<int>((playSquarePosition.x+25)+(col*columnWidth)), imageYPos+(row*rowHeight)};
                window->draw_image(imagePosition, slotsImageMatrix[row][col], imageWidth, imageHeight);

            }
        }

        //win overlay
        if (win and !spinning) {
            double winTotal = calculateMult()*betSlider.getValue();
            if (winTotal >= 50) {
                jackpot = true;
                window->draw_rectangle(overlayPosition, windowWidth, windowHeight, overlayColor);
                
                window->draw_text(bigWinPos, "BIG WIN", TDT4102::Color::black, winFontSize*2, font);
                window->draw_text(winPos, "You Won:", TDT4102::Color::black, winFontSize, font);

                TDT4102::Point amountPos {static_cast<int>((windowWidth/2)-(((currentPlayer->formatDouble(winAmount).length()*0.375))*winFontSize)), windowHeight/2};

                window->draw_text(amountPos, currentPlayer->formatDouble(winAmount), TDT4102::Color::black, static_cast<int>(winFontSize*1.5), font);
                window->draw_text(yippiPos, "Yippi!", TDT4102::Color::black, winFontSize, font);

                winAmount += 1;
                
                if (winAmount >= winTotal) {
                    winAmount = winTotal;
                    if (mouseDown || spaceKeyDown) {
                        win = false;
                        jackpot = false;
                        winAmount = 0;
                    }  
                }  
                
            } else {
                std::string winText = "You won " + currentPlayer->formatDouble(winTotal) + "!!!";
                window->draw_text(winTextPos, winText, TDT4102::Color::black, 20, font);
            }
            
        }
        if (escDown) {
            std::exit(EXIT_SUCCESS);
        }
    }
}

void SlotsGame::spin(const TDT4102::Slider& betSlider) {
    int amount = betSlider.getValue();
    currentPlayer->subMoney(amount);
    double mult = 1.0;
    win = false;
 
    cardDeck.resetDeck();
    cardDeck.shuffle();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {

            slotsMatrix[row][col] = cardDeck.drawCard();
            cardFilePaths[row][col] = slotsMatrix[row][col].cardFileName(slotsMatrix[row][col]);
            slotsImageMatrix[row][col] = TDT4102::Image(cardFilePaths[row][col]);
        }
    }

    std::cout << *this << std::endl;    



    mult = calculateMult();
    if (mult > 0) {
        std::cout << "Du vant! "<< amount*mult << " " << mult << std::endl;
        win = true;
        currentPlayer->addMoney(amount*mult);

    } else {
        std::cout << "Ingen gevinst!" << std::endl;
    }

    
    
}  


std::ostream& operator<<(std::ostream& os, const SlotsGame& game) {
    for (int row = 0; row < game.rows; row++) {
        for (int col = 0; col < game.columns; col++) {
            os << game.slotsMatrix[row][col].toString() << "\t";
        }
        os << std::endl;
    }
    return os;
}



double SlotsGame::calculateMult(){
    int hearts = 0;
    int spades = 0;
    int clubs = 0;
    int diamonds = 0;
    double mult = 0.0;


    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            std::string suitString = suitToString(slotsMatrix[row][col].getSuit());
            if (suitString == "hearts") {
                hearts++;
            } else if (suitString == "spades") {
                spades++;
            } else if (suitString == "clubs") {
                clubs++;
            } else if (suitString == "diamonds") {
                diamonds++;
            }
        }
    }

    //std::cout << hearts << " " << spades << " " << clubs << " " << diamonds << std::endl;
    std::vector<int> suitCount = {hearts, spades, clubs, diamonds};

    for (int count : suitCount) {
        if (count >= 10) {
            mult += std::pow(2, count - 10);
        }
    }

    return mult;
}

