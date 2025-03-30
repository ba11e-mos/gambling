#include "login.h"
#include "widgets/Button.h"
#include "choices.h"
#include "slots.h"


int main() {
	player currentPlayer;

    login loginWindow(&currentPlayer);

    loginWindow.loginWindow();
    
    return 0;
}
