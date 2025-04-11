#include "login.h"
#include "widgets/Button.h"
#include "choices.h"
#include "slots.h"
#include "poker.h"
#include <filesystem>



int main() {

 
	player currentPlayer;

    login loginWindow(&currentPlayer);

    loginWindow.loginWindow();

  return 0;  
}
