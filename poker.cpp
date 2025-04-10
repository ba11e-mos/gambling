#include "poker.h"
#include <random>

PokerGame::PokerGame(player* playerPtr, int botCount) : CurrentPlayer(playerPtr), window(nullptr), botCount(botCount) {

    PokerDeck.shuffle();
    table.reserve(5);
    pot = new double(0.0);
    std::filesystem::path filePath = "assets";
    
    players.push_back(CurrentPlayer);

    for (int i = 0; i < botCount; i++){

        players.push_back(new player);
        players[i+1]->registerUser("Bot" + std::to_string(i+1), 1000);

    }

    for (int i = 0; i < 2; i++){
        for(auto& player : players){
            player->hand.push_back(new Card (PokerDeck.drawCard()));
        }
    }
    
    /*loop for player hand images*/
    for (Card *card: players[0]->hand)
    {
        handImages.push_back(make_shared<TDT4102::Image>(card->cardFileName(*card)));
    }
    
}


PokerGame::~PokerGame(){
    delete window;

    for (auto playerPtr : players) {
        delete playerPtr;
    }
    players.clear();

    for (auto* card : table) {
        delete card;
    }
    table.clear();
    
    for (auto* p : players) {
        for (auto* card : p->hand) {
            delete card;
        }
        p->hand.clear();
    }

}



void PokerGame::Poker(){
    
    std::string windowTitle = "Texas hold'em";
    const unsigned int windowHeight = 1080;
    const unsigned int windowWidth = 1280;
    window = new TDT4102::AnimationWindow(300, 10, windowWidth, windowHeight, windowTitle);

    
    std::string userName = CurrentPlayer->getUsername();
    const int nameFontSize = 20;
    const TDT4102::Point namePosition {10, 10};

    
    const int FundFontSize = 20;
    std::string PlayerFundsString = std::to_string(CurrentPlayer->getMoney());
    int FundsLenght = PlayerFundsString.length()*FundFontSize*0.6;
    const TDT4102::Point pointsPosition {static_cast<int>(windowWidth)-static_cast<int>(10+FundsLenght), 10};
    

    /*Table GUI*/
    const int tableWidth = windowWidth-300;
    const unsigned int imageWidth = (tableWidth-100)/5;
    const int tableHeight = static_cast<int>(imageWidth*1.5);
    const unsigned int imageHeight = static_cast<int>(imageWidth*1.5);
    int columnWidth = imageWidth+10;

    TDT4102::Color tableFill = TDT4102::Color::dark_red;
    TDT4102::Color tableEdge = TDT4102::Color::black;

    const TDT4102::Point playSquarePosition {150, (windowHeight/2)-imageHeight};

    /*betting button*/
    const int betWidth = static_cast<int>(windowWidth/6);
    const int betHeight = static_cast<int>(betWidth/2);
    
    TDT4102::Color betFill = TDT4102::Color::light_sky_blue;
    const TDT4102::Point betPosition {(windowWidth*3/4-betWidth/4), (windowHeight-3*betHeight)};
    std::string betLabel = "bet!";
    TDT4102::Button betButton {betPosition, betWidth, betHeight, betLabel};
    betButton.setButtonColor(betFill);
    betButton.setLabelColor(TDT4102::Color::black);
    window->add(betButton);

    /*fold button*/
    const TDT4102::Point foldPosition {(windowWidth/4-betWidth*3/4), (windowHeight-3*betHeight)};
    std::string foldLabel = "fold :(";
    TDT4102::Button foldButton {foldPosition, betWidth, betHeight, foldLabel};
    foldButton.setButtonColor(betFill);
    foldButton.setLabelColor(TDT4102::Color::black);
    window->add(foldButton);

    /*stand button*/
    const TDT4102::Point standPosition {foldPosition.x, static_cast<int>(foldPosition.y-betHeight*1.5)};
    std::string standLabel = "stand";
    TDT4102::Button standButton {standPosition, betWidth, betHeight, standLabel};
    standButton.setButtonColor(betFill);
    standButton.setLabelColor(TDT4102::Color::black);
    window->add(standButton);

    /*pot GUI, grass??*/
    const TDT4102::Point potPosition {playSquarePosition.x, playSquarePosition.y-(imageHeight/2+betHeight)};
    const int potWidth = betWidth;
    const int potHeight = betHeight;
    const TDT4102::Point potText {potPosition.x, potPosition.y-(potWidth/2+10)};
    window->draw_text(potText, "pot", TDT4102::Color::black, FundFontSize, TDT4102::Font::courier_bold_italic);


    /*bet slider*/
    const unsigned int sliderHeight = 100;
    const unsigned int minBet = 10;
    const unsigned int maxBet = 100;
    const unsigned int startBet = 10;
    const unsigned int step = 1;
    const unsigned int betFontSize =  30;
    const TDT4102::Point sliderPosition {(windowWidth*3/4-betWidth/4), (windowHeight-2*sliderHeight)};
    TDT4102::Slider betSlider {sliderPosition, betWidth, sliderHeight, minBet, maxBet, startBet, step};
    
    const TDT4102::Point betTextPosition {(windowWidth*3/4-betWidth/4), static_cast<int>(windowHeight-1.5*betHeight)};
    window->add(betSlider);

    /*Callback functions*/
    betButton.setCallback([this, &betSlider](){
        if (!prevGameState == GameStateP::Betting) return;
        betRound(betSlider);
        if (!flop) {
            gameState = GameStateP::Flop;
        } else  if (table.size() == 3) {
            gameState = GameStateP::Turn;
        } else if (table.size() == 4) {
            gameState = GameStateP::River;
        } else if (table.size() == 5) {
            gameState = GameStateP::Showdown;
        }
    });


    standButton.setCallback([this, &betSlider](){
        if (!prevGameState == GameStateP::Betting) return;
        if (!flop) {
            return;
        } else  if (table.size() == 3) {
            gameState = GameStateP::Turn;
        } else if (table.size() == 4) {
            gameState = GameStateP::River;
        } else if (table.size() == 5) {
            gameState = GameStateP::Showdown;
        }
        
    });

    foldButton.setCallback([this](){
        CurrentPlayer->hasFolded = true;
        std::cout << "you fold" << std::endl;
    });



    while (!window->should_close()){
        window->next_frame();

        window->setBackgroundColor(TDT4102::Color::sea_green);

        //innsats oppdatering
        std::string betString = std::to_string(betSlider.getValue());
        window->draw_text(betTextPosition, betString, TDT4102::Color::black, betFontSize, TDT4102::Font::courier_bold_italic);

        window->draw_text(namePosition, userName, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);

        /*pot GUI, grass??*/
        std::string potString = std::to_string(*pot);
        window->draw_rectangle(potPosition, potWidth, potHeight, TDT4102::Color::floral_white, TDT4102::Color::black);
        window->draw_text(potPosition, potString, TDT4102::Color::black, FundFontSize, TDT4102::Font::courier_bold_italic);
        

        window->draw_rectangle(playSquarePosition, tableWidth, tableHeight, tableFill, tableEdge);
        for(int index = 0; index < size(tableImages); index++){
            TDT4102::Point imagePos{175+(index*columnWidth), playSquarePosition.y};
            window->draw_image(imagePos, *tableImages[index], imageWidth, imageHeight);
        }
          
        for(int index = 0; index < size(handImages); index++){
            TDT4102::Point handPos{(windowWidth/2-imageWidth)+(imageWidth*3/4)*index, windowHeight-(imageHeight+75)};
            window->draw_image(handPos, *handImages[index], imageWidth, imageHeight);
        }

        std::string pointsString = std::to_string(CurrentPlayer->getMoney());

        window->draw_text(namePosition, userName, TDT4102::Color::black, nameFontSize, TDT4102::Font::courier_bold_italic);
        window->draw_text(pointsPosition, pointsString, TDT4102::Color::black, FundFontSize, TDT4102::Font::courier_bold_italic);

        /* maybe?
        if(CurrentPlayer->getMoney()<=0){
            this->~PokerGame();
        }
        */  

    switch (gameState) {
        case GameStateP::Betting:
            prevGameState = GameStateP::Betting;
            break;
        
        case GameStateP::Flop:
            prevGameState = GameStateP::Flop;


            if (!flop) dealCards();

            gameState = GameStateP::Betting;
            break;
        
        case GameStateP::Turn:
            prevGameState = GameStateP::Turn;

            if (table.size() == 3) dealCards();
            gameState = GameStateP::Betting;

            break;
        
        case GameStateP::River:
            prevGameState = GameStateP::River;

            if (table.size() == 4) dealCards();
            gameState = GameStateP::Betting;
            break;
        
        case GameStateP::Showdown:
            prevGameState = GameStateP::Showdown;

            evaluateHands();
            std::cout << "Round over! Winner has been paid out." << std::endl;
            gameState = GameStateP::Reset;
            break;
        
        case GameStateP::Reset:
            prevGameState = GameStateP::Reset;

            resetRound();
            gameState = GameStateP::Betting;
            break;
        }
    }
}

void PokerGame::betRound(const TDT4102::Slider& betSlider){
    const double minBet = 10.0;
    double amount = betSlider.getValue();

    for(auto& player : players){
        double betAmount = 0.0;

        if(player == players[0]&& !CurrentPlayer->hasFolded){
            
            betAmount = amount;
            std::cout << "you bet: " << betAmount << std::endl;
        }
        else{
            bool willPlay = botMakeMove();
            if(willPlay){
                betAmount = botMakeBet(player);
            }
            else{
                std::cout << player->getUsername() << " folds" << std::endl;
                player->hasFolded = true;
                continue;
            }
        }

        *pot += betAmount;
        player->subMoney(betAmount);
    }
    
}

std::vector<Card*> PokerGame::getAllCards(player* player, const std::vector<Card*>& table) {
    std::vector<Card*> cards = player->hand;
    cards.insert(cards.end(), table.begin(), table.end());
    return cards;
}


bool PokerGame::botMakeMove(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1); 
    return dis(gen);
}

double PokerGame::botMakeBet(player* bot){
    const double chipValues[4] = {10.0, 20.0, 50.0, 100.0};

    srand(time(nullptr));

    int randomChip = rand()% size(chipValues);
    double bet = chipValues[randomChip];
    if (bot->getMoney() < bet){ return bot->getMoney();}
    return bet;
    
}



void PokerGame::dealCards(){
    if(!flop){
        for(int i = 0; i<3; ++i){
            Card drawnCard = PokerDeck.drawCard();
            tableImages.push_back(std::make_shared<TDT4102::Image>(drawnCard.cardFileName(drawnCard)));
            table.push_back(new Card (drawnCard));
        }
        flop = true;
    }
    else{
        PokerDeck.drawCard();
        Card drawnCard = PokerDeck.drawCard();
        table.push_back(new Card (drawnCard));
        tableImages.push_back(std::make_shared<TDT4102::Image>(drawnCard.cardFileName(drawnCard)));

    }
    
    this->gameState = GameStateP::Betting;
    
}

void PokerGame::resetRound(){

    for (Card* c : table) delete c;
    table.clear();
    tableImages.clear();
    flop = false;

    PokerDeck.resetDeck();

    *pot = 0;

    for (player* p : players) {
        for (Card* c : p->hand) delete c;
        p->hand.clear();
        p->hasFolded = false;

        for (int i = 0; i < 2; ++i) {
            Card newCard = PokerDeck.drawCard();
            p->hand.push_back(new Card(newCard));
        }
    }

    handImages.clear();
    for (Card *card: players[0]->hand) {
        handImages.push_back(make_shared<TDT4102::Image>(card->cardFileName(*card)));
    }
}

/*hand evaluation...ew*/
EvaluatedHand PokerGame::evaluatePlayerHand(player* player){
    std::vector<Card*> cards = getAllCards(player, table);

    std::map<int, int> valueCount;
    std::map<Suit, int> suitCount;
    std::vector<int> values;

    for(Card* card : cards){
        int val = static_cast<int>(card->getRank());
        valueCount[val]++;
        suitCount[card->getSuit()]++;
        values.push_back(val);
    }

    /*sort high to low*/
    std::sort(values.begin(), values.end(), std::greater<int>());

    /*remove duplicates (had help with this one)*/
    std::vector<int> uniqueValues = values;
    uniqueValues.erase(std::unique(uniqueValues.begin(), uniqueValues.end()), uniqueValues.end());

    /*flush check*/
    bool isFlush = false;
    for(auto& [suit, count] : suitCount){
        if (count >= 5){
            isFlush = true;
            break;
        }
    }

    /*straight check, gay?*/
    bool isStraight = false;
    for(size_t i = 0; i <= uniqueValues.size() - 5 && !isStraight; i++){
        bool straight = true;
        for(int j = 0; j < 4; j++){
            if(uniqueValues[i+j]-1 != uniqueValues[i+j]+1)
            straight = false;
            break;
        }

        if(straight){isStraight = true; break;}

    }

    /*A2345 straight*/
    if(!isStraight && std::find(uniqueValues.begin(), uniqueValues.end(), 14) != uniqueValues.end()){
        std::vector<int> lowStraight = {5, 4, 3, 2, 1};
        if(std::includes(uniqueValues.begin(), uniqueValues.end(), lowStraight.begin(), lowStraight.end())){
            isStraight = true;
        }
    }    

    /*groups*/
    int pairs = 0;
    bool three, four = false;
    std::vector<int> groupedValues;

    for(auto& [val, count] : valueCount){
        if(count == 4){
            four = true;
            groupedValues.insert(groupedValues.begin(), 4, val);
        }
        else if(count == 3){
            three = true;
            groupedValues.insert(groupedValues.begin(), 3, val);
        }
        else if(count == 2){
            pairs++;
            groupedValues.insert(groupedValues.begin(), 2, val);
        }
        else{
            groupedValues.push_back(val);
        }
    }

    HandRank rank = HandRank::HighCard;

    if(isFlush && isStraight){
        rank = HandRank::StraightFlush;
    }
    else if(four){
        rank = HandRank::FourOfAKind;
    }
    else if(three && pairs >= 1) {
        rank = HandRank::FullHouse;
    }
    else if(isFlush){
        rank  = HandRank::Flush;
    }
    else if(isStraight){
        rank = HandRank::Straight;
    }
    else if(three){
        rank = HandRank::ThreeOfAKind;
    }
    else if(pairs >= 2){
        rank = HandRank::TwoPair;
    }
    else if(pairs == 1){
        rank = HandRank::OnePair;
    }

    std::sort(groupedValues.begin(), groupedValues.end(), std::greater<int>());

    return EvaluatedHand{rank, groupedValues, player};
}

void PokerGame::evaluateHands(){
    std::vector<EvaluatedHand> results;
    for(player* player : players){
        if(!player->hasFolded){
            results.push_back(evaluatePlayerHand(player));
        }
    }

    std::sort(results.begin(), results.end(), [](const EvaluatedHand& a, const EvaluatedHand& b){
        if(a.rank != b.rank) {return a.rank > b.rank;}
        return a.rankedValues > b.rankedValues;

    });

    /*winner values*/
    player* winner = results.front().playerPtr;
    std::string winnerName = winner->getUsername();
    winner->addMoney(*pot);  
}