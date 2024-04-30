#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream> 

using namespace std;
using namespace sf;

#define SPEED -5 // Speed of pipes and grounds
#define Sover -10 // Transition for game over
#define GAP 165 // gap between pipes
#define DIS 1330 // distances between pipes

RenderWindow window(VideoMode(1700, 800), "Flappy Bird", Style::Default);
Event event;

short int modes = 0, diff = 1, BirdTheme = 2;
bool ttp = 0;
float Velocity, Gravity, PipeSpeedUpNDown = 1.5;
float speed = 4.0f;
int cnt = 0;
int cc = 0;
//int current = 0


const float SCREEN_W = window.getSize().x;
const float SCREEN_H = window.getSize().y;
Font MainFont;
Font FontTheme;
Font fontrate;

void setFonts()
{
    FontTheme.loadFromFile("arial.TTF");
    fontrate.loadFromFile("pixelmix_bold.ttf");
    MainFont.loadFromFile("pixelmix.ttf");
}

void setAssets();
void transition();
void draw();
void animation();
void drawW();


struct Pipes
{
    Sprite PiUp[5];
    Sprite PiDown[5];
    Texture Tex[2];

    int Rand;
    bool x, y, a, b;
    void GenRan()
    {
        Rand = 240 + rand() % 296;
    }

    void Constructor()
    {
        Tex[0].loadFromFile("PipeUp2.png");
        Tex[1].loadFromFile("PipeDown2.png");

        for (int i = 0; i < 5; i++)
        {
            GenRan();
            PiUp[i].setTexture(Tex[0]);
            PiUp[i].setPosition(1700, Rand);

            PiDown[i].setTexture(Tex[1]);
            PiDown[i].setPosition(1700, PiUp[i].getPosition().y - GAP * 4 );
        }
        x = 0, y = 0, a = 0, b = 0;
    }


    void MovePipesOne()
    {
        GenRan();
        PiUp[0].move(SPEED, 0);
        PiDown[0].move(SPEED, 0);

        if (PiUp[0].getPosition().x <= -112)
        {
            PiUp[0].setPosition(1700, Rand);
            PiDown[0].setPosition(1700, PiUp[0].getPosition().y - GAP * 4);
        }
    }

    void MovePipesTwo()
    {
        GenRan();

        if (PiUp[0].getPosition().x <= DIS)
            x = 1;

        if (x == 1)
        {
            PiUp[1].move(SPEED, 0);
            PiDown[1].move(SPEED, 0);
        }

        if (PiUp[1].getPosition().x <= -112)
        {
            PiUp[1].setPosition(1700, Rand);
            PiDown[1].setPosition(1700, PiUp[1].getPosition().y - GAP * 4);
        }
    }

    void MovePipesThree()
    {
        GenRan();

        if (PiUp[1].getPosition().x <= DIS)
            y = 1;

        if (y == 1)
        {
            PiUp[2].move(SPEED, 0);
            PiDown[2].move(SPEED, 0);
        }

        if (PiUp[2].getPosition().x <= -112)
        {
            PiUp[2].setPosition(1700, Rand);
            PiDown[2].setPosition(1700, PiUp[2].getPosition().y - (GAP * 4) );
        }
    }

    void MovePipesFour()
    {
        GenRan();

        if (PiUp[2].getPosition().x <= DIS)
            a = 1;

        if (a == 1)
        {
            PiUp[3].move(SPEED, 0);
            PiDown[3].move(SPEED, 0);
        }

        if (PiUp[3].getPosition().x <= -112)
        {
            PiUp[3].setPosition(1700, Rand);
            PiDown[3].setPosition(1700, PiUp[3].getPosition().y - (GAP * 4));
        }
    }

    void MovePipesFive()
    {
        GenRan();

        if (PiUp[3].getPosition().x <= DIS)
            b = 1;

        if (b == 1)
        {
            PiUp[4].move(SPEED, 0);
            PiDown[4].move(SPEED, 0);
        }

        if (PiUp[4].getPosition().x <= -112)
        {
            PiUp[4].setPosition(1700, Rand);
            PiDown[4].setPosition(1700, PiUp[4].getPosition().y - (GAP * 4) );
        }
    }

    void MovePipes()
    {
        MovePipesOne();
        MovePipesTwo();
        MovePipesThree();
        MovePipesFour();
        MovePipesFive();
    }

    void Draw()
    {
        for (int i = 0; i < 5; i++)
        {
            window.draw(PiUp[i]);
            window.draw(PiDown[i]);
        }
    }
} Pipes;

struct Flash
{
    RectangleShape TheFlash;
    Clock Time;
    bool FlashOn, RestartTime;

    void Constructor()
    {
        TheFlash.setSize(Vector2f(window.getSize().x, window.getSize().y));
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

        window.draw(TheFlash);
    }
} Flash;
struct Ground
{
    Sprite TwoGrounds;
    Texture Grounds;

    void Constructor(int PosX, int PosY)
    {
        Grounds.loadFromFile("Land.PNG");
        TwoGrounds.setTexture(Grounds);
        TwoGrounds.setPosition(PosX, PosY);
        TwoGrounds.setScale(2.2, 1);
    }

    void MoveGrounds()
    {
        TwoGrounds.move(SPEED, 0);

        if (TwoGrounds.getPosition().x <= -1700)
            TwoGrounds.setPosition(1700, 650);
    }

    void Draw()
    {
        window.draw(TwoGrounds);
    }

} Lone, Ltwo;

struct Bird
{
    Texture TxOne[4], TxTwo[4], TxThree[4];
    Sprite Bird;
    Clock dt, UpAndDown, PipeTimer;
    SoundBuffer H, J, W;
    Sound Hit, Jump, Whoop;
    int Rotation = 0, Iterator = 0;
    float  Gravity;
    bool Input = 0, Splayed, ToMove, ForPipeTimer;

    void Constructor(float Scale, float PosX, float PosY, int Birdtheme)
    {
        TxOne[0].loadFromFile("birddown.png");
        TxOne[1].loadFromFile("birdmid.png");
        TxOne[2].loadFromFile("birdup.png");
        TxOne[3].loadFromFile("birdmid.png");

        TxTwo[0].loadFromFile("birddown2.png");
        TxTwo[1].loadFromFile("birdmid2.png");
        TxTwo[2].loadFromFile("birdup2.png");
        TxTwo[3].loadFromFile("birdmid2.png");

        TxThree[0].loadFromFile("birddown3.png");
        TxThree[1].loadFromFile("birdmid3.png");
        TxThree[2].loadFromFile("birdup3.png");
        TxThree[3].loadFromFile("birdmid3.png");


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

        Splayed = 0, Velocity = 0, ToMove = 0, Gravity = 0.35, ForPipeTimer = 0, BirdTheme = Birdtheme;
    }

    void Animate()
    {
        if (dt.getElapsedTime().asSeconds() > 0.06)
        {
            if (Iterator < 3)
                Iterator++;

            else
                Iterator = 0;

            if (BirdTheme == 0)
                Bird.setTexture(TxOne[Iterator]);

            if (BirdTheme == 1)
                Bird.setTexture(TxTwo[Iterator]);

            if (BirdTheme == 2)
                Bird.setTexture(TxThree[Iterator]);

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

                if (diff == 1)
                    Velocity = 5;

                else
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

    void Difficulties()
    {

        if (modes == 1 && !ForPipeTimer)
        {
            PipeTimer.restart();
            ForPipeTimer = 1;
        }


        if (diff == 0)
        {
            Velocity += Gravity;

            if (Velocity >= 0)
                RotateDown();

            else
                RotateUp();
        }

        if (diff == 1)
        {
            Velocity -= Gravity;
            if (Velocity <= 0)
                RotateDown();

            else
                RotateUp();
        }

        if (diff == 2)
        {
            cout << Pipes.PiUp[1].getPosition().y << endl;
            Velocity += Gravity;

            if (Velocity >= 0)
                RotateDown();

            else
                RotateUp();

            for (int i = 0; i < 5; i++)
            {
                if (i % 2 == 0)
                {
                    Pipes.PiUp[i].move(0, -PipeSpeedUpNDown);
                    Pipes.PiDown[i].move(0, -PipeSpeedUpNDown);
                }
                else
                {
                    Pipes.PiUp[i].move(0, PipeSpeedUpNDown);
                    Pipes.PiDown[i].move(0, PipeSpeedUpNDown);
                }


                if ((int)PipeTimer.getElapsedTime().asSeconds() == 2)
                {
                    PipeSpeedUpNDown *= -1;
                    PipeTimer.restart();
                }
            }
        }
    }

    void gravNvelo()
    {
        if (Bird.getPosition().y <= 0)
            Bird.setPosition(Bird.getPosition().x, 0);

        Difficulties();
        Bird.move(0, Velocity);
    }

    void CollisionWPipes(Sprite pipes[])
    {
        if (Bird.getGlobalBounds().intersects(pipes[0].getGlobalBounds()) || Bird.getGlobalBounds().intersects(pipes[1].getGlobalBounds()) || Bird.getGlobalBounds().intersects(pipes[2].getGlobalBounds()) || Bird.getGlobalBounds().intersects(pipes[3].getGlobalBounds()) || Bird.getGlobalBounds().intersects(pipes[4].getGlobalBounds()))
        {
            if (!Splayed)
            {
                Whoop.play();
                Hit.play();
                Splayed = 1;
            }
            Bird.setRotation(70);
            Flash.FlashOn = 1;
            modes = 2;
        }
    }

    void CollisionWGround()
    {
        if (Bird.getGlobalBounds().intersects(Lone.TwoGrounds.getGlobalBounds()) || Bird.getGlobalBounds().intersects(Ltwo.TwoGrounds.getGlobalBounds()))
        {
            if (!Splayed)
            {
                Hit.play();
                Splayed = 1;
            }
            modes = 2;
        }
    }

    void MoveDTCol()
    {
        if (!(Bird.getGlobalBounds().intersects(Lone.TwoGrounds.getGlobalBounds()) || Bird.getGlobalBounds().intersects(Ltwo.TwoGrounds.getGlobalBounds())))
        {
            Bird.setRotation(75);
            Bird.move(0, 7);
        }
    }

    void Draw()
    {
        window.draw(Bird);
    }
}Bird;

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
            birds[i].setTexture(Bird.TxOne[0]);
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
        birds[1].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width / 2 - 15, 980 - 100);
        birds[2].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width / 2 - 15, 1050 - 100);
        birds[3].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width / 2 - 15, 1120 - 100);
        birds[4].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width / 2 - 15, 1190 - 100);
        birds[5].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width / 2 - 15, 1260 - 100);
        birds[6].setPosition(SCREEN_W / 2 - text[0].getGlobalBounds().width / 2 - 15, 1330 - 100);

        text[0].setPosition(SCREEN_W / 2 - 150, 800);
        text[1].setPosition(SCREEN_W / 2 - 150, 870);
        text[2].setPosition(SCREEN_W / 2 - 150, 940);
        text[3].setPosition(SCREEN_W / 2 - 150, 1010);
        text[4].setPosition(SCREEN_W / 2 - 150, 1080);
        text[5].setPosition(SCREEN_W / 2 - 150, 1150);
        text[6].setPosition(SCREEN_W / 2 - 150, 1220);
    }

    void draw()
    {
        if (cnt == 3) {
            //mainwindow.draw(black);
            for (int i = 0; i < 7; i++)
            {
                window.draw(text[i]);
                window.draw(birds[i]);
            }
        }
    }

    void move()
    {
        if (text[0].getPosition().y >= SCREEN_H / 2 - 240)
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
    void wingMove()
    {

        if (wingTimer.getElapsedTime().asMilliseconds() >= 150)
        {
            currBird = (currBird + 1) % 3;
            //currBird1 = (currBird + 1) % 3;
            birds[0].setTexture(Bird.TxOne[currBird]);
            birds[1].setTexture(Bird.TxOne[currBird]);
            birds[2].setTexture(Bird.TxOne[currBird]);
            birds[3].setTexture(Bird.TxOne[currBird]);
            birds[4].setTexture(Bird.TxOne[currBird]);
            birds[5].setTexture(Bird.TxOne[currBird]);
            birds[6].setTexture(Bird.TxOne[currBird]);
            wingTimer.restart();
        }
    }
}credits;

struct Menu
{
    RectangleShape darkWindow;
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
    Vector2u windowSize = window.getSize();

    Texture Title;
    Sprite SpriteTitle;

    Texture Play1;
    Sprite btnplay;
    Texture Theme;
    Sprite SpriteTheme;
    //flexibility
    Texture returnback;
    Sprite SpriteReturnBack;


    RectangleShape rectangle;
    //triangle
    CircleShape triangle;


    //themes for bird

    Texture birdthemes;
    Sprite SpriteBIRDthemes;


    RectangleShape rectanglebirdthemes;

    Texture birdtheme;
    Sprite Birdtheme;

    void set()
    {
        RectangleShape dar(Vector2f(SCREEN_W, SCREEN_H));
        darkWindow = dar;
        darkWindow.setFillColor(Color(0, 0, 0, 200));

        RectangleShape rectanglebirdthemes1(Vector2f(200.f, 100.f));
        rectanglebirdthemes = rectanglebirdthemes1;
        birdthemes.loadFromFile("btn-play.png");
        SpriteBIRDthemes.setTexture(birdthemes);
        SpriteBIRDthemes.setPosition(Vector2f(1100, 550));
        SpriteBIRDthemes.setScale(1.5f, 1.5f);
        rectanglebirdthemes.setPosition(Vector2f(1120, 560));
        rectanglebirdthemes.setFillColor(Color::White);
        rectanglebirdthemes.setScale(0.5f, 0.65f);

        birdtheme.loadFromFile("bird-02.png");
        Birdtheme.setTexture(birdtheme);
        Birdtheme.setPosition(Vector2f(1145, 568));
        Birdtheme.setScale(0.9f, 0.9f);
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
        RectangleShape SpriteMainMenu1(Vector2f(SCREEN_W, SCREEN_H));
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
    void draw() {
        window.draw(menu.SpriteMainMenu);
        if (cnt == 0) {
            window.draw(Bird.Bird);
            window.draw(menu.SpriteTitle);
            window.draw(menu.btnplay);
            window.draw(menu.SpriteTheme);
            window.draw(menu.SpriteRate);
            window.draw(menu.RectangleRate);
            window.draw(menu.textrate);
            window.draw(menu.SpriteBIRDthemes);
            window.draw(menu.rectanglebirdthemes);
            window.draw(menu.Birdtheme);

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

    void draw() {
        if (cnt == 1)
        {
            window.draw(menu.SpriteReturnBack);
            window.draw(menu.rectangle);
            window.draw(menu.triangle);
            window.draw(level.btnplay1);
            window.draw(level.btnplay2);
            window.draw(level.btnplay3);
            window.draw(level.Rectangle);
            window.draw(level.rectangle2);
            window.draw(level.rectangle3);
            window.draw(level.text);
            window.draw(level.text2);
            window.draw(level.text3);
            for (int i = 0; i < 2; ++i)
            {
                window.draw(menu.MainBackGrounds[i]);
            }
        }
    }

}level;

struct themes
{
    Texture theme1;

    Sprite SpriteTheme1;

    Texture theme2;

    Sprite SpriteTheme2;


    Texture theme3;

    Sprite SpriteTheme3;


    RectangleShape titleBar;
    Texture theme4;
    Text titleText;
    Sprite SpriteTheme4;
    void set()
    {
        Text titleText1("Choose a Theme", FontTheme, 20);
        titleText = titleText1;
        theme1.loadFromFile("Classic.png");
        SpriteTheme1.setTexture(theme1);
        SpriteTheme1.setPosition(Vector2f(400, 40));
        SpriteTheme1.setScale(0.82f, 0.6f);

        theme2.loadFromFile("London.png");
        SpriteTheme2.setTexture(theme2);
        SpriteTheme2.setPosition(Vector2f(800, 40));
        SpriteTheme2.setScale(0.82f, 0.6f);
        theme3.loadFromFile("Shanghai.png");
        SpriteTheme3.setTexture(theme3);
        SpriteTheme3.setPosition(Vector2f(400, 350));
        SpriteTheme3.setScale(0.82f, 0.6f);
        theme4.loadFromFile("Tokyo.png");
        SpriteTheme4.setTexture(theme4);
        SpriteTheme4.setPosition(Vector2f(800, 350));
        SpriteTheme4.setScale(0.82f, 0.6f);

        RectangleShape titleBar1(Vector2f(822, 40));
        titleBar = titleBar1;
        titleBar.setFillColor(Color::Black);
        titleBar.setPosition(400, 0);

        titleText.setFillColor(Color::White);
        titleText.setPosition(100, 10); // Adjust as needed

        // Calculate the centered position for the text
        float textWidth = titleText.getGlobalBounds().width;
        // Set the centered position for the text
        titleText.setPosition(750, 10); // Adjusted for vertical centering
    }
    void draw() {
        if (cnt == 2)
        {

            window.draw(menu.SpriteReturnBack);
            window.draw(menu.rectangle);
            window.draw(menu.triangle);
            window.draw(menu.darkWindow);
            window.draw(theme.SpriteTheme1);
            window.draw(theme.SpriteTheme2);
            window.draw(theme.SpriteTheme3);
            window.draw(theme.SpriteTheme4);
            window.draw(theme.titleBar);
            window.draw(theme.titleText);
        }
    }

}theme;






struct Themes
{
    Sprite themes;
    Texture Textures[4];

    void Constructor()
    {
        Textures[0].loadFromFile("Classic.png");
        Textures[1].loadFromFile("Shanghai.png");
        Textures[2].loadFromFile("Tokyo.png");
        Textures[3].loadFromFile("London.png");

        themes.setTexture(Textures[0]);
        themes.setScale(2.85, 1.5);
        themes.setPosition(0, -100);
    }   

    void Draw(int x)
    {
        themes.setTexture(Textures[x]);
        window.draw(themes);
    }
} Themes;

struct Scoring
{
    Font ScoreFont;
    Text Score, HighScore;
    SoundBuffer ScoInc;
    Sound ScoreSound;
    int Sinc, EasyHigh = 0, MediumHigh = 0, HardHigh = 0;
    bool Inc, MoveScoreUp;

    void Constructor(int x, int y, int size)
    {
        ScoreFont.loadFromFile("JANDAMANATEESOLID.ttf");
        Score.setFont(ScoreFont);
        Score.setCharacterSize(size);
        Score.setPosition(x, y); // 280, 100
        Score.setString(to_string(Sinc));

        HighScore.setFont(ScoreFont);

        HighScore.setPosition(880, 1175);
        HighScore.setCharacterSize(20);

        ScoInc.loadFromFile("Score.wav");
        ScoreSound.setBuffer(ScoInc);

        Inc = 0, MoveScoreUp = 0, Sinc = 0;
    }

    void hsSetup()
    {
        ifstream EasyHighScore("EasyHighScore.txt");
        ifstream MediumHighScore("MediumHighScore.txt");
        ifstream HardHighScore("HardHighScore.txt");

        if (diff == 0)
        {
            HighScore.setString(to_string(EasyHigh));

            if (EasyHighScore.is_open())
            {
                EasyHighScore >> EasyHigh;
                EasyHighScore.close();
            }

            if (Sinc > EasyHigh)
                EasyHigh = Sinc;

            ofstream highScoreFile("EasyHighScore.txt");
            if (highScoreFile.is_open())
            {
                highScoreFile << EasyHigh;
                highScoreFile.close();
            }
        }

        if (diff == 1)
        {
            HighScore.setString(to_string(MediumHigh));

            if (MediumHighScore.is_open())
            {
                MediumHighScore >> MediumHigh;
                MediumHighScore.close();
            }

            if (Sinc > MediumHigh)
                MediumHigh = Sinc;

            ofstream highScoreFile("MediumHighScore.txt");
            if (highScoreFile.is_open())
            {
                highScoreFile << MediumHigh;
                highScoreFile.close();
            }
        }

        if (diff == 2)
        {
            HighScore.setString(to_string(HardHigh));

            if (HardHighScore.is_open())
            {
                HardHighScore >> HardHigh;
                HardHighScore.close();
            }

            if (Sinc > HardHigh)
                HardHigh = Sinc;

            ofstream highScoreFile("HardHighScore.txt");
            if (highScoreFile.is_open())
            {
                highScoreFile << HardHigh;
                highScoreFile.close();
            }
        }


    }

    void IncScore()
    {
        if ((Bird.Bird.getPosition().x == Pipes.PiUp[0].getPosition().x || Bird.Bird.getPosition().x == Pipes.PiUp[1].getPosition().x || Bird.Bird.getPosition().x == Pipes.PiUp[2].getPosition().x) || Bird.Bird.getPosition().x == Pipes.PiUp[3].getPosition().x || Bird.Bird.getPosition().x == Pipes.PiUp[4].getPosition().x)
        {
            ScoreSound.play();
            Sinc++;
            Score.setString(to_string(Sinc));
        }
    }

    void ForGover()
    {
        if (!MoveScoreUp)
        {
            Constructor(880, 1115, 20);
            MoveScoreUp = 1;
        }

        if (HighScore.getPosition().y > 345)
            HighScore.move(0, Sover);

        if (Score.getPosition().y > 290)
            Score.move(0, Sover);
    }

    void DrawScore()
    {
        window.draw(Score);
    }

    void DrawHighScore()
    {
        window.draw(HighScore);
    }
} Score;

struct TapToPlayMenu
{
    Texture Textures[2]; // Textures[0] = ttp, Textures[1] = get ready
    Sprite Stuff[2]; // Stuff[0] = ttp, Stuff[1] = get ready
    void Constructor()
    {
        Textures[0].loadFromFile("ttplay.png");
        Stuff[0].setTexture(Textures[0]);
        Stuff[0].setScale(0.8, 0.8);
        Stuff[0].setPosition(800, 200);

        Textures[1].loadFromFile("getready.png");
        Stuff[1].setTexture(Textures[1]);
        Stuff[1].setScale(1.1, 1.1);
        Stuff[1].setPosition(735, 100);
    }
    void Draw()
    {
        window.draw(Stuff[0]);
        window.draw(Stuff[1]);
    }
} Tmenu;

struct GameOverMenu
{
    Texture gameOver[4];
    Sprite over[4];

    void Constuctor()
    {
        gameOver[0].loadFromFile("gameover.png");
        gameOver[1].loadFromFile("score.png");
        gameOver[2].loadFromFile("Play.png");
        gameOver[3].loadFromFile("Themes.png");

        for (int i = 0; i < 4; i++)
        {
            over[i].setTexture(gameOver[i]);  // over 0 = gameover, over 1 = score

            if (i < 2)
                over[i].setScale(1.2, 1.2);
        }

        over[0].setPosition(675, 1000);
        over[1].setPosition(650, 1075);

        over[2].setPosition(675, 1225);
        over[3].setPosition(800, 1225);
    }


    void Transition()
    {
        if (over[0].getPosition().y > 175)
            over[0].move(0, Sover);

        if (over[1].getPosition().y > 250)
            over[1].move(0, Sover);

        if (over[2].getPosition().y > 400)
            over[2].move(0, Sover);

        if (over[3].getPosition().y > 400)
            over[3].move(0, Sover);
    }

    void Draw()
    {
        window.draw(over[0]);
        window.draw(over[1]);
        window.draw(over[2]);
        window.draw(over[3]);
    }
} Gover;

struct ForModeControl
{
    void ControlModeOne()
    {
       if (Mouse::isButtonPressed(Mouse::Left) && modes == 0 && !ttp && cc ==1 )
           modes = 1;
    }

    void Reset()
    {
       
       
            ttp = 1;
            modes = 0;

            Lone.Constructor(0, 650);
            Ltwo.Constructor(780, 650);

            Pipes.Constructor();

            Bird.Constructor(2, 300, 250, 1);

            Themes.Constructor();

            Score.Constructor(280, 100, 50);

            Gover.Constuctor();

            Flash.Constructor();
        

        if (event.type == Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == Mouse::Left)
                ttp = 0;
        }
    }
} Control;

struct Gamemodes
{
    void TTPlay()
    {
        Bird.Animate();
        Bird.UpNDown();

        Lone.MoveGrounds();
        Ltwo.MoveGrounds();

        window.clear();

        Themes.Draw(0);

        Tmenu.Draw();

        Bird.Draw();

        Lone.Draw();
        Ltwo.Draw();

        window.display();
    }
    void Playing()
    {
        
        Bird.CollisionWGround();
        Bird.CollisionWGround();
        Bird.CollisionWPipes(Pipes.PiUp);
        Bird.CollisionWPipes(Pipes.PiDown);
        Bird.gravNvelo();
        Bird.GUI();
        Bird.Animate();

        Pipes.MovePipes();

        Lone.MoveGrounds();
        Ltwo.MoveGrounds();

        Score.IncScore();

        window.clear();

        Themes.Draw(0);

        Bird.Draw();

        Pipes.Draw();

        Lone.Draw();
        Ltwo.Draw();

        Score.DrawScore();

        window.display();
    }

    void Collided()
    {
        Bird.MoveDTCol();

        Gover.Transition();

        Score.ForGover();

        window.clear();

        Themes.Draw(0);

        Pipes.Draw();

        Bird.Draw();

        Lone.Draw();
        Ltwo.Draw();

        Gover.Draw();

        Score.DrawScore();
        Score.DrawHighScore();

        Flash.Draw();

        window.display();
    }
} Mode;

struct Game
{
    void Flappy_Bird() // Call Me
    {
        if (modes == 0) {
            Mode.TTPlay();
        }
        if (modes == 1)
            Mode.Playing();

        if (modes == 2)
            Mode.Collided();
    }
} Game;



int main()
{
    window.setPosition(Vector2i(100, 50));
    window.setFramerateLimit(60);
    setAssets();


    //Bird.Constructor(1.7, 300, 250, 1);

    Pipes.Constructor();

    Lone.Constructor(0, 650);
    Ltwo.Constructor(1700, 650);

    Themes.Constructor();

    Tmenu.Constructor();

    Gover.Constuctor();

    Score.Constructor(800, 100, 50);

    Flash.Constructor();

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
                transition();
        }

        Control.ControlModeOne();
        Control.Reset();
        Score.hsSetup();

        animation();
        if (cc == 0 )
            draw();
        // Full Game
        else 
            Game.Flappy_Bird();
      //  drawW();
    }
    return 0;
}



void setAssets()
{

    menu.set();
    Bird.Constructor(2, 500, 350, 0);
    window.setFramerateLimit(60);
    level.set();
    setFonts();
    credits.set();
    theme.set();
}
void draw()
{

    window.clear();
    window.draw(menu.SpriteMainMenu);

    if (cnt != 0)
        window.draw(menu.SpriteReturnBack);
    menu.draw();
    for (int i = 0; i < 2; ++i)
    {
        window.draw(menu.MainBackGrounds[i]);
    }
    level.draw();
    theme.draw();
    credits.draw();
    window.display();
}

void transition()
{
    if (event.type == Event::Closed)
    {
        window.close();
    }

    //button press
    if (Mouse::isButtonPressed(Mouse::Left) && cnt == 0)
    {
        Vector2i mousePos = Mouse::getPosition(window);
        if (mousePos.x > 500 && mousePos.x < 740 && mousePos.y>550 && mousePos.y < 790)
        {
            cnt = 1;
        }

    }
    //themes
    if (Mouse::isButtonPressed(Mouse::Left) && cnt == 0)
    {
        Vector2i mousePos = Mouse::getPosition(window);
        if (mousePos.x > 900 && mousePos.x < 1150 && mousePos.y>550 && mousePos.y < 790)
        {
            cnt = 2;
        }

    }
    //credits
    if (Mouse::isButtonPressed(Mouse::Left) && cnt == 0)
    {
        Vector2i mousePos = Mouse::getPosition(window);
        if (mousePos.x > 700 && mousePos.x < 900 && mousePos.y>550 && mousePos.y < 790)
        {
            cnt = 3;
        }

    }

    // Check for mouse click events
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        Vector2i mousePos = Mouse::getPosition(window);


        // Check if the return back sprite is clicked

        if (cnt != 0 && mousePos.x > 5 && mousePos.x < 25 + menu.SpriteReturnBack.getGlobalBounds().width &&
            mousePos.y > 5 && mousePos.y < 25 + menu.SpriteReturnBack.getGlobalBounds().height)
        {
            // Reset cnt to return to the first screen
            cnt = 0;
        }
    }
    // levels
    if (Mouse::isButtonPressed(Mouse::Left) && cnt == 1)
    {
        Vector2i mousePos = Mouse::getPosition(window);
        if (mousePos.x > 749 && mousePos.x < 929 && mousePos.y >161 && mousePos.y < 258) {
            diff = 0;
            cc = 1;
        }
        if (mousePos.x > 727 && mousePos.x < 927 && mousePos.y >325 && mousePos.y < 425) {
            diff = 1;
            cc = 1;
        }
        if (mousePos.x > 740 && mousePos.x < 927 && mousePos.y >501 && mousePos.y < 596) {

            diff = 2;
            cc = 1;
        }
        //cout << diff << "  ";
    }

}
void animation()
{
    Bird.Animate();
    menu.moveLands();
    if (cnt == 3)
    {
        credits.move();
        credits.wingMove();
    }
}

void drawW() {
    if ( cc ==0)
        draw();
    if (cc == 1)
        Game.Flappy_Bird();
        
}
