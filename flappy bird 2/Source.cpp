#include <SFML/Graphics.hpp>
#include<iostream>
#include<SFML/Audio.hpp>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <string>
#include <random>
#include <iomanip>
#include <vector>


using namespace std;
using namespace sf;

RenderWindow mainwindow(VideoMode(1700, 800), "Flappy bird");
//Menu menu(1700, 800);
Event event;
const float SCREEN_W = mainwindow.getSize().x;
const float SCREEN_H = mainwindow.getSize().y;
Font MainFont;
Font FontTheme;
Font fontrate;

void setFonts() {

FontTheme.loadFromFile("arial.TTF");
fontrate.loadFromFile("pixelmix_bold.ttf");
MainFont.loadFromFile("pixelmix.ttf");
}


float speed = 4.0f;
int cnt = 0;


struct Flash
{
    RectangleShape TheFlash;
    Clock Time;
    bool FlashOn, RestartTime;

    void Constructor()
    {
        TheFlash.setSize(Vector2f(mainwindow.getSize().x, mainwindow.getSize().y));
        FlashOn = 0, RestartTime = 0;
    }

    void Draw()
    {
        if (!RestartTime)
        {
            Time.restart();
            RestartTime = 1;
        }

        if (FlashOn && Time.getElapsedTime().asSeconds() < 0.15)
            TheFlash.setFillColor(Color(0, 0, 0, 150));

        else
            TheFlash.setFillColor(Color(0, 0, 0, 0));

        mainwindow.draw(TheFlash);
    }
} Flash;


struct Bird
{
    Texture Tx[4];
    Sprite Bird;
    Clock dt, UpAndDown;
    SoundBuffer H, J, W;
    Sound Hit, Jump, Whoop;
    int Rotation = 0, Iterator = 0;
    float Velocity, Gravity;
    bool Input = 0, Splayed, ToMove;

    void Constructor(float Scale, float PosX, float PosY)
    {
        Tx[0].loadFromFile("birddown.png");
        Tx[1].loadFromFile("birdmid.png");
        Tx[2].loadFromFile("birdup.png");
        Tx[3].loadFromFile("birdmid.png");

        Bird.setTexture(Tx[Iterator]);
        Bird.setPosition(PosX, PosY);
        Bird.setOrigin(Bird.getGlobalBounds().width / 2, Bird.getGlobalBounds().height / 2);
        Bird.setScale(Scale, Scale);
        Bird.setRotation(0);

        J.loadFromFile("Jump.wav");
        Jump.setBuffer(J);

        H.loadFromFile("Hit.wav");
        Hit.setBuffer(H);

        W.loadFromFile("Whoop.wav");
        Whoop.setBuffer(W);

        Splayed = 0, Velocity = 0, ToMove = 0, Gravity = 0.3;
    }

    void Animate()
    {
        if (dt.getElapsedTime().asSeconds() > 0.06)
        {
            if (Iterator < 3)
                Iterator++;

            else
                Iterator = 0;

            Bird.setTexture(Tx[Iterator]);
            dt.restart();
        }
    }

    void UpNDown()
    {
        if (UpAndDown.getElapsedTime().asSeconds() >= 0.1 && !ToMove)
        {
            Bird.move(0, -0.5);
            ToMove = 1;
        }
        if (UpAndDown.getElapsedTime().asSeconds() >= 0.2 && ToMove)
        {
            Bird.move(0, 0.5);
            ToMove = 0;
            UpAndDown.restart();
        }
    }

    void RotateDown()
    {
        if (Rotation <= 35)
            Rotation += 3;

        Bird.setRotation(Rotation);
    }

    void RotateUp()
    {
        if (Rotation >= -35)
            Rotation = -35;

        Bird.setRotation(Rotation);
    }

    void GUI()
    {
        if (event.type == Event::MouseButtonPressed && !Input)
        {
            if (event.mouseButton.button == Mouse::Left)
            {
                Jump.play();
                Input = 1;

                Velocity = -5;

                Bird.move(0, Velocity * 9 / 5);
            }
        }

        if (event.type == Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == Mouse::Left)
                Input = 0;
        }

    }

    void gravNvelo(Event event)
    {
        if (Bird.getPosition().y <= 0)
            Bird.setPosition(Bird.getPosition().x, 0);

        if (Velocity >= 0)
            RotateDown();

        else
            RotateUp();

        Bird.move(0, Velocity);
        Velocity += Gravity;
    }

    

    

    

    void Draw()
    {
        mainwindow.draw(Bird);
    }
} Bird;


struct credits
{
    
    
    Font font;
    Text text[7]; 
    Clock wingTimer; 
    int currBird = 0;
    RectangleShape black;
    Sprite birds[7];
    void set()
    {

        font.loadFromFile("arial.ttf");
        RectangleShape _b(Vector2f(SCREEN_W, SCREEN_H));
        _b.setFillColor(Color(0, 0, 0, 150));
        black = _b;

        for (int i = 0; i < 7; i++)
        {
            text[i].setFont(font);
            text[i].setFillColor(Color::Blue);
            text[i].setOrigin(text[i].getGlobalBounds().width / 2, text[i].getGlobalBounds().height / 2);
            text[i].setScale(1.5, 1.5);
        }
        for (int i = 0; i < 7; i++)
        {
            birds[i] = Bird.Bird;
            birds[i].setTexture(Bird.Tx[0]);
            //birds[i].setColor(Color(0, 0, 0, 255));
            birds[i].setOrigin(birds[0].getGlobalBounds().width / 2, text[i].getGlobalBounds().height / 2);
            birds[i].setScale(1.3, 1.3);
        }



        text[0].setString("Youssef Fayek");
        text[1].setString("Maria Akram");
        text[2].setString("Karen Bahaa");
        text[3].setString("Maria Ramy");
        text[4].setString("Karen Ashraf");
        text[5].setString("Jennifer Hany");
        text[6].setString("Omar Zamel");

        birds[0].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width / 2 - 15, 910 - 100);
        birds[1].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width /2 - 15, 980 - 100);
        birds[2].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width /2 - 15, 1050 - 100);
        birds[3].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width /2 - 15, 1120 - 100);
        birds[4].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width /2 - 15, 1190 - 100);
        birds[5].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width /2 - 15, 1260 - 100);
        birds[6].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width /2 - 15, 1330- 100);

        text[0].setPosition(SCREEN_W / 2 - 150, 800);
        text[1].setPosition(SCREEN_W / 2 -150 , 870);
        text[2].setPosition(SCREEN_W / 2 -150 , 940);
        text[3].setPosition(SCREEN_W / 2 -150 , 1010);
        text[4].setPosition(SCREEN_W / 2 -150 , 1080);
        text[5].setPosition(SCREEN_W / 2 -150 , 1150);
        text[6].setPosition(SCREEN_W / 2 -150 , 1220);
        


    }



    void draw()
    {
        //mainwindow.draw(black);
        for (int i = 0; i < 7; i++)
        {
            mainwindow.draw(text[i]);
            mainwindow.draw(birds[i]);
        }
    }
    void move()
    {
        if (text[0].getPosition().y >= SCREEN_H /2 - 240 )
        {
            text[0].move(0, -1);
            birds[0].move(0, -1);
            text[1].move(0, -1);
            birds[1].move(0, -1);


            text[2].move(0, -1);
            birds[2].move(0, -1);




            text[3].move(0, -1);
            birds[3].move(0, -1);




            text[4].move(0, -1);
            birds[4].move(0, -1);




            text[5].move(0, -1);
            birds[5].move(0, -1);

            text[6].move(0, -1);
            birds[6].move(0, -1);
        }
        
    }
    void wingMove() {


        if (wingTimer.getElapsedTime().asMilliseconds() >= 150) 
        {
            currBird = (currBird + 1) % 3;
            //currBird1 = (currBird + 1) % 3;
            birds[0].setTexture(Bird.Tx[currBird]);
            birds[1].setTexture(Bird.Tx[currBird]);
            birds[2].setTexture(Bird.Tx[currBird]);
            birds[3].setTexture(Bird.Tx[currBird]);
            birds[4].setTexture(Bird.Tx[currBird]);
            birds[5].setTexture(Bird.Tx[currBird]);
            birds[6].setTexture(Bird.Tx[currBird]);
            wingTimer.restart();


        }


    }
}credits;

int current = 0;

struct Menu 
{
    Texture Rate;
    Sprite SpriteRate;

    Texture Bc1;
    

   // const int numBackgrounds = 2; // Number of background sprites
    Sprite MainBackGrounds[2];
    


    //   RectangleShape RectangleRate(Vector2f(95.0f, 35.0f));
    RectangleShape RectangleRate;


    Text textrate;

    Texture MainMenu;
    RectangleShape SpriteMainMenu;
    Texture mainmenubgdark;
    Sprite bgdark;
    Vector2u windowSize = mainwindow.getSize();

    Texture Title;
    Sprite SpriteTitle;
    
    Texture Play1;
    Sprite btnplay;
    Texture Theme;
    Sprite SpriteTheme;
    //flexibility
    Texture returnback;
    Sprite SpriteReturnBack;
   
    //rectangle
   // RectangleShape rectangle(Vector2f(70.0f, 25.0f));
    RectangleShape rectangle;
    //triangle
    CircleShape triangle;



    

    void set() {

        Bc1.loadFromFile("land.png");
        for (int i = 0; i < 2; ++i)
        {
            MainBackGrounds[i].setTexture(Bc1);
            MainBackGrounds[i].setScale(float(SCREEN_W) / Bc1.getSize().x, float(SCREEN_H) / Bc1.getSize().y);
            MainBackGrounds[i].setPosition(Vector2f(i * SCREEN_W, 670));
        }
         RectangleShape RectangleRate111(Vector2f(95.0f, 35.0f));
         RectangleShape rectangle111(Vector2f(70.0f, 25.0f));
         rectangle = rectangle111;
         RectangleRate = RectangleRate111;
        textrate.setFont(fontrate);
        textrate.setCharacterSize(25);
        textrate.setString("Credits");
        textrate.setFillColor(Color::Yellow);
        textrate.setPosition(713, 575);
        Rate.loadFromFile("btn-rate.png");

        SpriteRate.setTexture(Rate);
        SpriteRate.setPosition(Vector2f(700, 550));
        SpriteRate.setScale(2.5f, 2.5f);
        RectangleRate.setPosition(Vector2f(725, 570));
        RectangleRate.setOutlineColor(Color::White);
        RectangleRate.setFillColor(Color::White);


        triangle.setPosition(Vector2f(33.f, 48.f));
        triangle.setRadius(17.f);
        triangle.setPointCount(3);
        triangle.setFillColor(Color::Green);
        triangle.setOutlineThickness(5.f);
        triangle.setRotation(270.f);

        rectangle.setPosition(Vector2f(20.f, 14.f));
        rectangle.setOutlineColor(Color::White);
        rectangle.setFillColor(Color::White);


        Play1.loadFromFile("btn-play.png");
        btnplay.setTexture(Play1);
        btnplay.setPosition(Vector2f(500, 550));
        btnplay.setScale(1.5f, 1.5f);
        returnback.loadFromFile("btn-rate.png");
        SpriteReturnBack.setTexture(returnback);
        SpriteReturnBack.setPosition(Vector2f(5, 5));
        SpriteReturnBack.setScale(1.5f, 1.5f);

    Theme.loadFromFile("Theme.png");
    SpriteTheme.setTexture(Theme);
    SpriteTheme.setPosition(Vector2f(900, 550));
    
    SpriteTheme.setScale(1.5f, 1.5f);


        Title.loadFromFile("game_title.png");
        SpriteTitle.setTexture(Title);
        SpriteTitle.setPosition(Vector2f(650, 170));
        SpriteTitle.setScale(2.0f, 2.0f);

         mainmenubgdark.loadFromFile("Dark bg.jpg");
        MainMenu.loadFromFile("Classic.png");
            bgdark.setTexture(mainmenubgdark);
        RectangleShape SpriteMainMenu1(Vector2f(SCREEN_W,SCREEN_H));
        SpriteMainMenu = SpriteMainMenu1;
        SpriteMainMenu.setTexture(&MainMenu);

    }
    void moveLands() {
        // Update positions and reset if needed
        for (int i = 0; i < 2; ++i)
        {
            MainBackGrounds[i].move(-speed, 0);
            if (MainBackGrounds[i].getPosition().x + MainBackGrounds[i].getGlobalBounds().width < 0)
            {
                float nextX = MainBackGrounds[(i + 1) % 2].getPosition().x + MainBackGrounds[(i + 1) % 2].getGlobalBounds().width;
                MainBackGrounds[i].setPosition(Vector2f(nextX - 5, 670));
            }
        }
    }

}menu;
struct levels 
{
    RectangleShape Rectangle;
    Texture play;
    Sprite btnplay1;
    RectangleShape rectangle2;
    Texture play2;
    Sprite btnplay2;

    RectangleShape rectangle3;
    Texture play3;
    Sprite btnplay3;

    Text text;
    Text text2;
    Text text3;

    void set()
    {
        RectangleShape _Rectangle(Vector2f(200, 100));
        RectangleShape _rectangle2(Vector2f(200.f, 100.f));
        RectangleShape _rectangle3(Vector2f(200.f, 100.f));
        Rectangle = _Rectangle;
        rectangle3 = _rectangle3;
        rectangle2 = _rectangle2;


    Rectangle.setPosition(Vector2f(790, 180));
    Rectangle.setFillColor(Color::White);
    Rectangle.setScale(0.7f, 0.7f);

    play.loadFromFile("btn-play.png");
    btnplay1.setTexture(play);
    btnplay1.setPosition(Vector2f(735, 150));
    btnplay1.setScale(2.0f, 2.0f);

    rectangle2.setPosition(Vector2f(790, 330));
    rectangle2.setFillColor(Color::White);
    rectangle2.setScale(0.7f, 0.7f);

    play2.loadFromFile("btn-play.png");
    btnplay2.setTexture(play2);
    btnplay2.setPosition(Vector2f(735, 320));
    btnplay2.setScale(2.0f, 2.0f);

    rectangle3.setPosition(Vector2f(790, 520));
    rectangle3.setFillColor(Color::White);
    rectangle3.setScale(0.7f, 0.7f);

    play3.loadFromFile("btn-play.png");
    btnplay3.setTexture(play3);
    btnplay3.setPosition(Vector2f(735, 490));
    btnplay3.setScale(2.0f, 2.0f);

    text.setFont(MainFont);
    text.setString("Easy");
    text.setCharacterSize(40);
    text.setFillColor(Color::Green);
    text.setPosition(780, 185);

    text2.setFont(MainFont);
    text2.setString("Medium");
    text2.setCharacterSize(40);
    text2.setFillColor(Color::Yellow);
    text2.setPosition(760, 355);

    text3.setFont(MainFont);
    text3.setString("Hard");
    text3.setCharacterSize(40);
    text3.setFillColor(Color::Red);
    text3.setPosition(780, 525);
   
    
    
    }

}level;

int main()
{
    menu.set();
    Bird.Constructor(2, 500, 350);
    mainwindow.setFramerateLimit(60);
    level.set();

    setFonts();
    credits.set();

    Texture theme1;
    theme1.loadFromFile("Classic.png");
    Sprite SpriteTheme1;
    SpriteTheme1.setTexture(theme1);
    SpriteTheme1.setPosition(Vector2f(400, 40));
    SpriteTheme1.setScale(0.82f, 0.6f);

    Texture theme2;
    theme2.loadFromFile("London.png");
    Sprite SpriteTheme2;
    SpriteTheme2.setTexture(theme2);
    SpriteTheme2.setPosition(Vector2f(800, 40));
    SpriteTheme2.setScale(0.82f, 0.6f);

    Texture theme3;
    theme3.loadFromFile("Shanghai.png");
    Sprite SpriteTheme3;
    SpriteTheme3.setTexture(theme3);
    SpriteTheme3.setPosition(Vector2f(400, 350));
    SpriteTheme3.setScale(0.82f, 0.6f);


    Texture theme4;
    theme4.loadFromFile("Tokyo.png");
    Sprite SpriteTheme4;
    SpriteTheme4.setTexture(theme4);
    SpriteTheme4.setPosition(Vector2f(800, 350));
    SpriteTheme4.setScale(0.82f, 0.6f);

    RectangleShape titleBar(Vector2f(822, 40));
    titleBar.setFillColor(Color::Black);
    titleBar.setPosition(400, 0);
    Text titleText("Choose a Theme", FontTheme, 20);
    titleText.setFillColor(Color::White);
    titleText.setPosition(100, 10); // Adjust as needed

    // Calculate the centered position for the text
    float textWidth = titleText.getGlobalBounds().width;
    // Set the centered position for the text
    titleText.setPosition(750, 10); // Adjusted for vertical centering
    


   
    //themes for bird

    Texture birdthemes;
    birdthemes.loadFromFile("btn-play.png");
    Sprite SpriteBIRDthemes;
    SpriteBIRDthemes.setTexture(birdthemes);
    SpriteBIRDthemes.setPosition(Vector2f(1100, 550));
    SpriteBIRDthemes.setScale(1.5f, 1.5f);

    RectangleShape rectanglebirdthemes(Vector2f(200.f, 100.f));
    rectanglebirdthemes.setPosition(Vector2f(1120, 560));
    rectanglebirdthemes.setFillColor(Color::White);
    rectanglebirdthemes.setScale(0.5f, 0.65f);

    Texture birdtheme;
    birdtheme.loadFromFile("bird-02.png");
    Sprite Birdtheme;
    Birdtheme.setTexture(birdtheme);
    Birdtheme.setPosition(Vector2f(1145, 568));
    Birdtheme.setScale(0.9f, 0.9f);
    

    while (mainwindow.isOpen())
    {
        Event event;
        while (mainwindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                mainwindow.close();
            }

            //button press
            if (Mouse::isButtonPressed(Mouse::Left) && cnt == 0)
            {
                Vector2i mousePos = Mouse::getPosition(mainwindow);
                if (mousePos.x > 500 && mousePos.x < 740 && mousePos.y>550 && mousePos.y < 790)
                {
                    cnt = 1;
                }

            }
            //themes
            if (Mouse::isButtonPressed(Mouse::Left) && cnt ==0)
            {
                Vector2i mousePos = Mouse::getPosition(mainwindow);
                if (mousePos.x > 900 && mousePos.x < 1150 && mousePos.y>550 && mousePos.y < 790)
                {
                    cnt = 2;
                }

            }
            //credits
            if (Mouse::isButtonPressed(Mouse::Left) && cnt == 0 )
            {
                Vector2i mousePos = Mouse::getPosition(mainwindow);
                if (mousePos.x > 700 && mousePos.x < 900 && mousePos.y>550 && mousePos.y < 790)
                {
                    cnt = 3;
                }

            }

            // Check for mouse click events
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(mainwindow);

                // Check if the return back sprite is clicked
                if (cnt == 1 && mousePos.x > 5 && mousePos.x < 25 + menu.SpriteReturnBack.getGlobalBounds().width &&
                    mousePos.y > 5 && mousePos.y < 25 + menu.SpriteReturnBack.getGlobalBounds().height)
                {
                    // Reset cnt to return to the first screen
                    cnt = 0;
                }
                if (cnt == 2 && mousePos.x > 5 && mousePos.x < 25 + menu.SpriteReturnBack.getGlobalBounds().width &&
                    mousePos.y > 5 && mousePos.y < 25 + menu.SpriteReturnBack.getGlobalBounds().height)
                {
                    // Reset cnt to return to the first screen
                    cnt = 0;
                }
            }

            
                
            
           
        }
        menu.moveLands();
        

        
     


        if (cnt == 3) {
            credits.move();
            credits.wingMove();
        }
        //MainBirds.animate();
        Bird.Animate();
        mainwindow.clear();
        mainwindow.draw(menu.SpriteMainMenu);
        if (cnt == 0) {
            mainwindow.draw(Bird.Bird);
           // MainBirds.draw(mainwindow);
            mainwindow.draw(menu.SpriteTitle);
            mainwindow.draw(menu.btnplay);
            mainwindow.draw(menu.SpriteTheme);
            mainwindow.draw(menu.SpriteRate);
            mainwindow.draw(menu.RectangleRate);
            mainwindow.draw(menu.textrate);
            mainwindow.draw(SpriteBIRDthemes);
            mainwindow.draw(rectanglebirdthemes);
            mainwindow.draw(Birdtheme);
            //cout << menu.RectangleRate.getPosition().x<<"      ";
        }
        if (cnt == 1) {
          //  mainwindow.draw(menu.SpriteMainMenu);
            mainwindow.draw(menu.SpriteReturnBack);
            mainwindow.draw(menu.rectangle);
            mainwindow.draw(menu.triangle);
            mainwindow.draw(level.btnplay1);
            mainwindow.draw(level.btnplay2);
            mainwindow.draw(level.btnplay3);
            mainwindow.draw(level.Rectangle);
            mainwindow.draw(level.rectangle2);
            mainwindow.draw(level.rectangle3);
            mainwindow.draw(level.text);
            //cout << level.text.getPosition().y << "      ";

            mainwindow.draw(level.text2);
            mainwindow.draw(level.text3);

            for (int i = 0; i < 2; ++i)
            {
                mainwindow.draw(menu.MainBackGrounds[i]);
            }

        }

        else  if (cnt == 2) {
            mainwindow.draw(menu.bgdark);
            mainwindow.draw(menu.SpriteReturnBack);
            mainwindow.draw(menu.rectangle);
            mainwindow.draw(menu.triangle);
            mainwindow.draw(SpriteTheme1);
            mainwindow.draw(SpriteTheme2);
            mainwindow.draw(SpriteTheme3);
            mainwindow.draw(SpriteTheme4);
            mainwindow.draw(titleBar);
            mainwindow.draw(titleText);

            for (int i = 0; i < 2; ++i)
            {

                //mainwindow.draw(darkbackgrounds[i]);
            }
        }
        if (cnt == 0) 
        {
            for (int i = 0; i < 2; ++i)
            {
                mainwindow.draw(menu.MainBackGrounds[i]);
            }
        }

       // mainwindow.draw(credits.text[6]);

        //menu.draw(mainwindow);
        if (cnt == 3) {
            credits.draw();
            
        }
        mainwindow.display();

    }
    return 0;
}

