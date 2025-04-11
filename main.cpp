#include "login.h"
#include "widgets/Button.h"
#include "choices.h"
#include "slots.h"
#include "poker.h"
#include <filesystem>



int main() {

 
	player currentPlayer;
/*    
  currentPlayer.registerUser("noa", 1000); 
  
 BlackJackGame BlackJackGame(&currentPlayer);
  BlackJackGame.blackJack();
  

  SlotsGame SlotsGame(&currentPlayer);
  SlotsGame.slots();
PokerGame PokerGame(&currentPlayer);
  PokerGame.Poker();


  */
  login loginWindow(&currentPlayer);

  loginWindow.loginWindow();

  return 0;  
}
