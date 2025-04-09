#include "login.h"
#include "widgets/Button.h"
#include "choices.h"
#include "slots.h"
#include <filesystem>



int main() {

      
	player currentPlayer;
   
  currentPlayer.registerUser("noa", 1000);

  SlotsGame SlotsGame(&currentPlayer);
  SlotsGame.slots();

  /*
  BlackJackGame BlackJackGame(&currentPlayer);
  BlackJackGame.blackJack();
  
  
  login loginWindow(&currentPlayer);

  loginWindow.loginWindow();
  */
  return 0;  
}
