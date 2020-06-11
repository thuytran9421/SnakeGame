#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
using namespace std;

class Window 
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int WIN_WIDTH, WIN_HEIGHT;

public:
    Window();   //ham tao
    Window(int, int);

    friend class PlaySnake;

    void SetWindowSize(int, int);
    void Create_Window();
    void Destroy();
};

class Position 
{    //lop dung de tao, xac dinh vi tri cho snake, food
private:
    int x, y;
public:
    int getXpos();
    int getYpos();
    void setPosition(int, int);
};

class Snake 
{
private:
    Position CurrPos;   //cap nhat vi tri hien tai
    char direction; //huong di chuyen
    int length, size;

public:
    Snake();

    int getLength();
    void setLength(int);
    int getSize();     // kich thuoc cua 1 o ran (hinh vuong)

    // tao va lay vi tri cho snake
    void setPosition(int, int);
    int getXpos();
    int getYpos();
    void setDirection(char);
    char getDirection();
    void reset();

    // hanh dong dac trung cua snake
    void move();
    bool eat(SDL_Rect*, SDL_Rect*);
};

class Food 
{
    Position CurrPos;
    int size = 30;

public:
    void showFood();

    int getSize();
    int getXpos();
    int getYpos();
    void setPosition(int, int);
};

class Score
{
private:
    Position fixedPosition;
    int currScore;
    int highScore;
public:
    Score();

    void increaseScore();
    int getCurrScore();
    int getHighScore();
    void resetScore();

    void getfileHighScore();
    void setfileHighScore();
    void checkNewHighScore();
};

#endif 
