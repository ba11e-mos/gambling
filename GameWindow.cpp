#include "GameWindow.h"

GameWindow::GameWindow(int width, int height, const std::string& title, const std::string& username, int money, TDT4102::Font font)
    : TDT4102::AnimationWindow(0, 0, width, height, title), username(username), money(money), font(font) {}

void GameWindow::drawBackground(const TDT4102::Color& color) {
    this->draw_rectangle({0, 0}, this->width(), this->height(), color);
}

void GameWindow::drawUsernameAndMoney(const TDT4102::Point& namePos, const TDT4102::Point& moneyPos) {
    this->draw_text(namePos, username, TDT4102::Color::white, 20, font);
    std::string moneyText = "Money: " + std::to_string(money);
    int moneyTextLenght = moneyText.length();
    this->draw_text({static_cast<int>(moneyPos.x-(moneyTextLenght*20)/1.5), moneyPos.y}, moneyText, TDT4102::Color::white, 20, font);
}


void GameWindow::updateMoney(int newMoney) {
    money = newMoney;
}

void GameWindow::setFullscreen(bool fullscreen) {
    SDL_Window* sdlWindow = this->getWindowHandle();
    if (!sdlWindow) {
        std::cerr << "Failed to get SDL window handle." << std::endl;
        return;
    }

    if (fullscreen) {
        if (SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
            std::cerr << "Failed to set fullscreen mode: " << SDL_GetError() << std::endl;
        }
    } else {
        if (SDL_SetWindowFullscreen(sdlWindow, 0) != 0) {
            std::cerr << "Failed to exit fullscreen mode: " << SDL_GetError() << std::endl;
        }
    }
}

void GameWindow::getWindowSize(int& width, int& height) {
    SDL_Renderer* renderer = this->getRendererHandle();
    if (!renderer) {
        std::cerr << "Failed to get SDL renderer handle." << std::endl;
        return;
    }

    SDL_GetRendererOutputSize(renderer, &width, &height);
}

