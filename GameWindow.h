#pragma once
#include "AnimationWindow.h"
#include <string>
#include <iostream>


class GameWindow : public TDT4102::AnimationWindow {
private:
    std::string username;
    int money;
    TDT4102::Font font;

public:
    GameWindow(int width, int height, const std::string& title, const std::string& username, int money, TDT4102::Font font = TDT4102::Font::pixeloid);

    void drawBackground(const TDT4102::Color& color);
    void drawUsernameAndMoney(const TDT4102::Point& namePos, const TDT4102::Point& moneyPos);
    void updateMoney(int newMoney);

    void setFullscreen(bool fullscreen);
    void getWindowSize(int& width, int& height);

    
};