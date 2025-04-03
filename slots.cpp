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
    Layout layout;


    std::string windowTitle = "Gambling++";
    layout.windowHeight = 1080;
    layout.windowWidth = 1280;


    window = new TDT4102::AnimationWindow(300, 10, layout.windowWidth, layout.windowHeight, windowTitle);


    /*Brukernavn*/
    std::string username = currentPlayer->getUsername();
    int nameFontSize = 20;
    const TDT4102::Point namePosition {10, 10};
    

    /*Poeng*/
    int pointsFontSize = 20;
    std::string pointsString = std::to_string(currentPlayer->getMoney());
    layout.pointsLenght = pointsString.length()*pointsFontSize*0.6;

    /*Spillvindu*/
    layout.playSquareWidth = layout.windowWidth-300;
    layout.playSquareHeight = static_cast<int>(layout.playSquareWidth*0.85);
    TDT4102::Color playSquareFill = TDT4102::Color::white;
    TDT4102::Color playSquareEdge = TDT4102::Color::black;
    layout.playSquarePosition = {150, 50};


    /*Slots matrise*/
    layout.imageWidth = (layout.playSquareWidth-100)/5;
    layout.imageHeight = static_cast<int>(layout.imageWidth*1.5);
    int rowHeight = layout.imageHeight*0.2;
    int columnWidth = layout.imageWidth*0.2;
    layout.imageYPos = -(rowHeight*rows);
    int slidePos = (layout.playSquareHeight+50)/60; 
    

    /*Spinneknapp*/
    layout.spinWidth = static_cast<int>(layout.windowWidth/6);
    layout.spinHeight = static_cast<int>(layout.spinWidth/2);
    
    TDT4102::Color spinFill = TDT4102::Color::light_sky_blue;
    layout.spinLabel = "Spinn!";
    layout.spinPosition = {static_cast<int>(layout.windowWidth/2-layout.spinWidth/2), static_cast<int>(layout.playSquareHeight+50+layout.spinHeight/2)};
    layout.spinButton = TDT4102::Button(layout.spinPosition, layout.spinWidth, layout.spinHeight, layout.spinLabel);
    layout.spinButton.setButtonColor(spinFill);
    layout.spinButton.setLabelColor(TDT4102::Color::black);
    window->add(layout.spinButton);


    /*Innsats slider*/
    const unsigned int betHeight = 100;
    const unsigned int betWidth = layout.spinWidth;
    const unsigned int minBet = 1;
    const unsigned int maxBet = 100;
    const unsigned int startBet = 10;
    const unsigned int step = 1;
    const unsigned int betFontSize =  30;
    const TDT4102::Point betPosition {static_cast<int>(layout.windowWidth-betWidth*2), static_cast<int>(layout.playSquareHeight+betHeight)};
    layout.betSlider = TDT4102::Slider{betPosition, betWidth, betHeight, minBet, maxBet, startBet, step};
    
    const TDT4102::Point betTextPosition {static_cast<int>(layout.windowWidth-betWidth*2-50), static_cast<int>(layout.playSquareHeight+betHeight+betFontSize*0.85)};
    window->add(layout.betSlider);


    /*Win-overlay*/
    TDT4102::Color overlayColor(0, 0, 0, 128);
    TDT4102::Point overlayPosition {0,0};
    const unsigned int winFontSize = 50;
    double winAmount = 0;
    jackpot = false;
    TDT4102::Point bigWinPos {static_cast<int>((layout.windowWidth/2)-(3.5*winFontSize)), static_cast<int>(layout.windowHeight/2-4*winFontSize)};
    TDT4102::Point winPos {static_cast<int>((layout.windowWidth/2)-(2*winFontSize)), static_cast<int>(layout.windowHeight/2-2*winFontSize)};
    TDT4102::Point yippiPos {static_cast<int>((layout.windowWidth/2)-(1.25*winFontSize)), static_cast<int>(layout.windowHeight/2+2*winFontSize)};

    TDT4102::Point winTextPos {static_cast<int>((layout.windowWidth/2)-200), static_cast<int>(layout.playSquareHeight+50)};

    

    /*Callback-funksjoner*/
    layout.spinButton.setCallback([this, &layout]() {
        if (spinning) return;
        if (jackpot) return;
        spinning = true;
        spinButton(layout);
    });



    updateLayout(layout);


    while (!window->should_close()) {
        window->next_frame();

        //Setter points dynamisk
        if (window->was_resized()) {
            updateLayout(layout);
        }

        /*Input*/
        bool mouseDown = window->is_left_mouse_button_down();
        bool spaceKeyDown = window->is_key_down(KeyboardKey::SPACE);
        if (!spinning and !jackpot) {
            if (spaceKeyDown) {
                spin(layout.betSlider);
                spinning = true;
                layout.imageYPos = -(rowHeight*rows);
            } 
        }
        


        //pengeverdi oppdatering
        std::string pointsString = formatDouble(currentPlayer->getMoney());

        //innsats oppdatering
        std::string betString = std::to_string(layout.betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, betFontSize, TDT4102::Font::courier_bold_italic);


        //brukernavn og penger på skjerm
        window->draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_text(layout.pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);
        
        //spillvindu
        window->draw_rectangle(layout.playSquarePosition, layout.playSquareWidth, layout.playSquareHeight, playSquareFill, playSquareEdge);
        if (spinning) {
            layout.imageYPos += slidePos;
            if (layout.imageYPos > 50) {
                spinning = false;
            }
        }

        //Slots matrise
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                TDT4102::Point imagePosition{175+(col*columnWidth), layout.imageYPos+(row*rowHeight)};
                window->draw_image(imagePosition, slotsImageMatrix[row][col], layout.imageWidth, layout.imageHeight);

            }
        }

        //win overlay
        if (win and !spinning) {
            double winTotal = calculateMult()*layout.betSlider.getValue();
            if (winTotal >= 50) {
                jackpot = true;
                window->draw_rectangle(overlayPosition, layout.windowWidth, layout.windowHeight, overlayColor);
                
                window->draw_text(bigWinPos, "BIG WIN", TDT4102::Color::black, winFontSize*2);
                window->draw_text(winPos, "You Won:", TDT4102::Color::black, winFontSize);

                TDT4102::Point amountPos {static_cast<int>((layout.windowWidth/2)-(((formatDouble(winAmount).length()*0.375))*winFontSize)), static_cast<int>(layout.windowHeight/2)};

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


/*Oppdatere spillvindu størrelser*/
void SlotsGame::updateLayout(Layout& layout) {
    layout.windowWidth = window->width();
    layout.windowHeight = window->height();

    layout.pointsPosition = {static_cast<int>(layout.windowWidth)-static_cast<int>(layout.pointsLenght*0.75), 10};


    layout.playSquareWidth = layout.windowWidth -300;
    layout.playSquareHeight = static_cast<int>(layout.windowHeight * 0.80);
    layout.playSquarePosition = {150, 50};

    layout.spinWidth = static_cast<int>(layout.windowWidth / 6);
    layout.spinHeight = static_cast<int>(layout.spinWidth / 2);
    layout.spinPosition = {static_cast<int>(layout.windowWidth / 2 - layout.spinWidth / 2), static_cast<int>(layout.playSquareHeight + 50 + layout.spinHeight / 2)};
    window->remove(layout.spinButton);
    layout.spinButton = TDT4102::Button(layout.spinPosition, layout.spinWidth, layout.spinHeight, layout.spinLabel);

    window->add(layout.spinButton);


    layout.betWidth = layout.spinWidth;
    layout.betHeight = 100;
    layout.betPosition = {static_cast<int>(layout.windowWidth - layout.betWidth * 2), static_cast<int>(layout.playSquareHeight + layout.betHeight)};
    layout.betTextPosition = {static_cast<int>(layout.windowWidth - layout.betWidth * 2 - 50), static_cast<int>(layout.playSquareHeight + layout.betHeight + 30)};

    layout.bigWinPos = {static_cast<int>((layout.windowWidth / 2) - (3.5 * 50)), static_cast<int>(layout.windowHeight / 2 - 4 * 50)};
    layout.winPos = {static_cast<int>((layout.windowWidth / 2) - (2 * 50)), static_cast<int>(layout.windowHeight / 2 - 2 * 50)};
    layout.yippiPos = {static_cast<int>((layout.windowWidth / 2) - (1.25 * 50)), static_cast<int>(layout.windowHeight / 2 + 2 * 50)};


    layout.spinButton.setCallback([this, &layout]() {
        if (spinning) return;
        if (jackpot) return;
        spinning = true;
        spinButton(layout);
    });
}



void SlotsGame::spinButton(Layout& layout) {
    if (spinning) return;
    if (jackpot) return;
    spin(layout.betSlider);
    spinning = true;
    layout.imageYPos = -(rowHeight*rows);

}