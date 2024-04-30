#pragma once
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class menu2
{
public:
    Text mainmenu[3];
    menu2 (float width, float height);
    void draw(RenderWindow& window);
    void MoveUp();
    void MoveDown();
    void setSelected(int n);
    int pressed()
    {
        return selected;
    }

    ~menu2 ();

private:
    int selected;
    Font font;
};