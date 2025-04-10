#include "login.h"
#include "widgets/Button.h"
#include "choices.h"
#include "slots.h"
#include "poker.h"
#include <filesystem>



int main() {

 
	player currentPlayer;
 
  currentPlayer.registerUser("noa", 1000);

  PokerGame PokerGame(&currentPlayer);
  PokerGame.Poker();
/* 
  SlotsGame SlotsGame(&currentPlayer);
  SlotsGame.slots();


  login loginWindow(&currentPlayer);

  loginWindow.loginWindow();
 */ 
  return 0;  
}
