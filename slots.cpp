#include "slots.h"

SlotsGame::SlotsGame(player* playerPtr) : currentPlayer(playerPtr), window(nullptr) {
    slotsMatrix = new Card*[rows];
    for (int i = 0; i < rows; i++) {
        slotsMatrix[i] = new Card[columns];
    }
}

SlotsGame::~SlotsGame() {
    delete window;
    for (int i = 0; i < rows; i++) {
        delete[] slotsMatrix[i];
    }
    delete[] slotsMatrix;
}

void SlotsGame::slots() {
    std::string windowTitle = "Gambling++";
    const unsigned int windowHeight = 720;
    const unsigned int windowWidth = 1280;
    window = new TDT4102::AnimationWindow(300, 300, windowWidth, windowHeight, windowTitle);


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
    const unsigned int playSquareHeight = static_cast<int>(3*(windowHeight/5)-50);
    const unsigned int playSquareWidth = windowWidth-300;
    TDT4102::Color playSquareFill = TDT4102::Color::white;
    TDT4102::Color playSquareEdge = TDT4102::Color::black;
    const TDT4102::Point playSquarePosition {150, 50};


    /*Spinneknapp*/
    const unsigned int spinHeight = static_cast<int>((((windowHeight/5)*2)/4)*3);
    const unsigned int spinWidth = spinHeight;
    TDT4102::Color spinFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point spinPosition {windowWidth/2-spinWidth/2, static_cast<int>(((windowHeight/5)*3)+((((windowHeight/5)*2)-(spinHeight))/2))};
    std::string spinLabel = "Spinn!";
    TDT4102::Button spinButton {spinPosition, spinWidth, spinHeight, spinLabel};
    spinButton.setButtonColor(spinFill);
    window->add(spinButton);


    /*Innsats slider*/
    const unsigned int betHeight = 100;
    const unsigned int betWidth = spinWidth;
    const unsigned int minBet = 1;
    const unsigned int maxBet = 100;
    const unsigned int startBet = 10;
    const unsigned int step = 1;
    const TDT4102::Point betPosition {windowWidth-betWidth-50, static_cast<int>(((windowHeight/5)*3)+((((windowHeight/5)*2)-(betHeight))/2))};
    TDT4102::Slider betSlider {betPosition, betWidth, betHeight, minBet, maxBet, startBet, step};
    
    const TDT4102::Point betTextPosition {windowWidth-betWidth-50, static_cast<int>(((windowHeight/5)*3)+((((windowHeight/5)*2)-(betHeight))/2))+betHeight};
    window->add(betSlider);

    /*Callback-funksjoner*/
    spinButton.setCallback([this, &betSlider]() {spin(betSlider);});

    while (!window->should_close()) {
        window->next_frame();

        //pengeverdi oppdatering
        std::string pointsString = std::to_string(currentPlayer->getMoney());

        //innsats oppdatering
        std::string betString = std::to_string(betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);


        //brukernavn og penger på skjerm
        window->draw_text(namePosition, username, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_text(pointsPosition, pointsString, TDT4102::Color::black, pointsFontSize, TDT4102::Font::courier_bold_italic);
        
        //spillvindu
        window->draw_rectangle(playSquarePosition, playSquareWidth, playSquareHeight, playSquareFill, playSquareEdge);
    }
}

void SlotsGame::spin(const TDT4102::Slider& betSlider) {
    int amount = betSlider.getValue();
    currentPlayer->subMoney(amount);

    cardDeck.resetDeck();
    cardDeck.shuffle();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            slotsMatrix[row][col] = cardDeck.drawCard();
        }
    }

    std::cout << *this << std::endl;

    std::cout << "Du vant! "<< amount*calculateMult() << std::endl;
    currentPlayer->addMoney(amount*calculateMult());
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

int SlotsGame::calculateMult(){
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


    std::vector<int> suit = {hearts, spades, clubs, diamonds};

    for (int max = 13; max > 5; --max) {
        double tempMult = 1.0 + (max - 5)*0.5;

        if (std::any_of(suit.begin(), suit.end(), [max](int v) { return v > max;})) {
            mult = tempMult;
            break;
        }
    }

    return mult;
}