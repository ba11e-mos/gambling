#include "slots.h"

SlotsGame::SlotsGame(player* playerPtr) : currentPlayer(playerPtr), window(nullptr) {
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
    std::string windowTitle = "Gambling++";
    const unsigned int windowHeight = 1080;
    const unsigned int windowWidth = 1280;
    window = new TDT4102::AnimationWindow(300, 10, windowWidth, windowHeight, windowTitle);


    /*Brukernavn*/
    std::string username = currentPlayer->getUsername();
    int nameFontSize = 20;
    const TDT4102::Point namePosition {10, 10};
    

    /*Poeng*/
    int pointsFontSize = 20;
    std::string pointsString = std::to_string(currentPlayer->getMoney());
    int pointsLenght = pointsString.length()*pointsFontSize*0.6;
    const TDT4102::Point pointsPosition {static_cast<int>(windowWidth)-static_cast<int>(10+pointsLenght), 10};

    /*Spillvindu*/
    const unsigned int playSquareWidth = windowWidth-300;
    const unsigned int playSquareHeight = static_cast<int>(playSquareWidth*0.85);
    TDT4102::Color playSquareFill = TDT4102::Color::white;
    TDT4102::Color playSquareEdge = TDT4102::Color::black;
    const TDT4102::Point playSquarePosition {150, 50};


    /*Slots matrise*/
    const unsigned int imageWidth = (playSquareWidth-100)/5;
    const unsigned int imageHeight = static_cast<int>(imageWidth*1.5);
    int rowHeight = imageHeight+10;
    int columnWidth = imageWidth+10;
    int imageYPos = -(rowHeight*rows);
    int slidePos = (playSquareHeight+50)/60; 
    

    /*Spinneknapp*/
    const unsigned int spinWidth = static_cast<int>(windowWidth/6);
    const unsigned int spinHeight = static_cast<int>(spinWidth/2);
    
    TDT4102::Color spinFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point spinPosition {windowWidth/2-spinWidth/2, static_cast<int>(playSquareHeight+50+spinHeight/2)};
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
    const TDT4102::Point betPosition {windowWidth-betWidth*2, static_cast<int>(playSquareHeight+betHeight)};
    TDT4102::Slider betSlider {betPosition, betWidth, betHeight, minBet, maxBet, startBet, step};
    
    const TDT4102::Point betTextPosition {windowWidth-betWidth*2-50, static_cast<int>(playSquareHeight+betHeight+betFontSize*0.85)};
    window->add(betSlider);


    /*Win-overlay*/
    TDT4102::Color overlayColor(0, 0, 0, 128);
    TDT4102::Point overlayPosition {0,0};
    const unsigned int winFontSize = 50;
    double winAmount = 0;
    bool jackpot = false;
    TDT4102::Point bigWinPos {static_cast<int>((windowWidth/2)-(3.5*winFontSize)), windowHeight/2-4*winFontSize};
    TDT4102::Point winPos {static_cast<int>((windowWidth/2)-(2*winFontSize)), windowHeight/2-2*winFontSize};
    TDT4102::Point yippiPos {static_cast<int>((windowWidth/2)-(1.25*winFontSize)), windowHeight/2+2*winFontSize};

    TDT4102::Point winTextPos {(windowWidth/2)-200, playSquareHeight+50};

    

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
        if (!spinning and !jackpot) {
            if (spaceKeyDown) {
                spin(betSlider);
                spinning = true;
                imageYPos = -(rowHeight*rows);
            } 
        }
        


        //pengeverdi oppdatering
        std::string pointsString = formatDouble(currentPlayer->getMoney());

        //innsats oppdatering
        std::string betString = std::to_string(betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, betFontSize, TDT4102::Font::courier_bold_italic);


        //brukernavn og penger på skjerm
        window->draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_text(pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);
        
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
                TDT4102::Point imagePosition{175+(col*columnWidth), imageYPos+(row*rowHeight)};
                window->draw_image(imagePosition, slotsImageMatrix[row][col], imageWidth, imageHeight);

            }
        }

        //win overlay
        if (win and !spinning) {
            double winTotal = calculateMult()*betSlider.getValue();
            if (winTotal >= 50) {
                jackpot = true;
                window->draw_rectangle(overlayPosition, windowWidth, windowHeight, overlayColor);
                
                window->draw_text(bigWinPos, "BIG WIN", TDT4102::Color::black, winFontSize*2);
                window->draw_text(winPos, "You Won:", TDT4102::Color::black, winFontSize);

                TDT4102::Point amountPos {static_cast<int>((windowWidth/2)-(((formatDouble(winAmount).length()*0.375))*winFontSize)), windowHeight/2};

                window->draw_text(amountPos, formatDouble(winAmount), TDT4102::Color::black, static_cast<int>(winFontSize*1.5));
                window->draw_text(yippiPos, "Yippi!", TDT4102::Color::black, winFontSize);

                winAmount += 0.1;
                
                if (winAmount >= winTotal) {
                    winAmount = winTotal;
                    if (mouseDown || spaceKeyDown) {
                        win = false;
                        jackpot = false;
                        winAmount = 0;
                    }  
                }  
                
            } else {
                std::string winText = "You won " + formatDouble(winTotal) + "!!!";
                window->draw_text(winTextPos, winText);
            }
            
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


    std::vector<int> suitCount = {hearts, spades, clubs, diamonds};

    for (int count : suitCount) {
        if (count >= 6) {
            mult += 1 + (count - 6) * 0.5;
        }
    }

    return mult;
}

/*Formatering av double til string med to desimaler*/
std::string SlotsGame::formatDouble(double value) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}