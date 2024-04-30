
#include "menu2.h"
#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;
menu2::menu2(float width, float height)
{
    font.loadFromFile("JANDAMANATEEBUBBLE.TTF");

}

menu2 ::~menu2() {}

void menu2::setSelected(int n)
{
    selected = n;
}

void menu2::draw(RenderWindow& window)
{
    for (int i = 0; i < 3; i++)
    {
        window.draw(mainmenu[i]);
    }
}

void menu2::MoveDown()
{
    if (selected + 1 <= 3) //not exit
    {
        mainmenu[selected].setFillColor(Color::White);
        selected++;
        if (selected == 3)
        {
            selected = 0;
        }
        mainmenu[selected].setFillColor(Color{ 255,204,0 });
    }
}

void menu2::MoveUp()
{
    if (selected - 1 >= -1) //not play
    {
        mainmenu[selected].setFillColor(Color::White);
        selected--;
        if (selected == -1)
        {
            selected = 2;
        }
        mainmenu[selected].setFillColor(Color{ 255,204,0 });
    }
}
