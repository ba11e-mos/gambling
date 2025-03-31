#include "login.h"
#include "widgets/Button.h"
#include "choices.h"
#include "slots.h"
#include <filesystem>



int main() {
  std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

      
	player currentPlayer;
   
  currentPlayer.registerUser("noa", 1000);

  SlotsGame SlotsGame(&currentPlayer);
  SlotsGame.slots();
  /*
  
  login loginWindow(&currentPlayer);

  loginWindow.loginWindow();
  */
  return 0;  
}
