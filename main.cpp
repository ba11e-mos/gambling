#include "login.h"
#include "widgets/Button.h"
#include "choices.h"
#include "slots.h"


int main() {
    
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
