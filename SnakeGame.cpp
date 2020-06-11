#include "SnakeGame.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <fstream>
using namespace std;

/*------------------------------------*
 *         WINDOW OBJECT              *
 *------------------------------------*/

Window::Window() 
{
    this->WIN_WIDTH = 640;
    this->WIN_HEIGHT = 480;

    SDL_Init(SDL_INIT_VIDEO);
}

Window::Window(int WIN_WIDTH, int WIN_HEIGHT) 
{
    this->WIN_WIDTH = WIN_WIDTH;
    this->WIN_HEIGHT = WIN_HEIGHT;
}

void Window::SetWindowSize(int WIN_WIDTH, int WIN_HEIGHT) 
{
    this->WIN_WIDTH = WIN_WIDTH;
    this->WIN_HEIGHT = WIN_HEIGHT;
}

/* tao lap cua so va renderer */
void Window::Create_Window() 
{
    // tao cua so cho game
    window = SDL_CreateWindow(
        "SNAKE GAME",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIN_WIDTH,
        WIN_HEIGHT,
        SDL_WINDOW_OPENGL);

    // renderer de ve do hoa
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // kiem tra xem viec tao cua so co loi khong
    if (window == NULL) 
    {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return;
    }

    // kiem tra viec tao renderer co loi khong
    if (renderer == NULL) 
    {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return;
    }
}

void Window::Destroy() 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);  //dong cua so lai
    SDL_Quit(); //thoat khoi he thong SDL
}



/*------------------------------------*
 *         POSITION OBJECT            *
 *------------------------------------*/

 // lay toa do x
int Position::getXpos() 
{
    return x;
}

// lay toa do y
int Position::getYpos() 
{
    return y;
}

// thay doi toa do x,y
void Position::setPosition(int x, int y) 
{
    this->x = x;
    this->y = y;
}



/*------------------------------------*
 *         SNAKE OBJECT               *
 *------------------------------------*/

 //khoi tao nhung dac trung cua ran
Snake::Snake() {
    length = 3; //chieu dai ban dau
    size = 30;  //kich thuoc moi o ran

    direction = 'r';    //huong di chuyen mac dinh ban dau la sang phai
}

int Snake::getLength() 
{
    return this->length;
}

// thay doi chieu dai ran
void Snake::setLength(int length) 
{
    this->length = length;
}

// lay kich thuoc moi o ran
int Snake::getSize() 
{
    return this->size;
}

// doi toa do cua ran dang hien thi tren cua so game
void Snake::setPosition(int x, int y) 
{
    CurrPos.setPosition(x, y);
}

// lay toa do x hien tai
int Snake::getXpos() 
{
    return CurrPos.getXpos();
}

// lay toa do y hien tai
int Snake::getYpos() 
{
    return CurrPos.getYpos();
}

void Snake::setDirection(char direction) 
{  //xac dinh huong di chuyen cua ran
    if (direction == 'u' && this->direction != 'd') 
    {
        this->direction = 'u';  //di len
    }
    else if (direction == 'd' && this->direction != 'u') 
    {
        this->direction = 'd';  //di xuong
    }
    else if (direction == 'l' && this->direction != 'r') 
    {
        this->direction = 'l';  //sang trai
    }
    else if (direction == 'r' && this->direction != 'l') 
    {   
        this->direction = 'r';  //sang phai
    }
}


char Snake::getDirection() 
{
    return direction;
}

// thiet lap lai trang thai cua ran
void Snake::reset() 
{
    length = 3;
    size = 30;

    direction = 'r';
}

// Di chuyen
void Snake::move() 
{
    int distance = 10;

    //neu ran di chuyen sang phai
    if (direction == 'r')
    {
        setPosition(getXpos() + distance, getYpos());   //thay doi lai toa do cua ran
    }
    //neu ran di chuyen sang trai
    else if (direction == 'l')
    {
        setPosition(getXpos() - distance, getYpos());
    }
    // neu ran di chuyen xuong
    else if (direction == 'd')
    {
        setPosition(getXpos(), getYpos() + distance);
    }
    // neu ran di chuyen len
    else if (direction == 'u')
    {
        setPosition(getXpos(), getYpos() - distance);
    }
}

bool Snake::eat(SDL_Rect* snake, SDL_Rect* food) 
{
    int leftSnake, rightSnake,
        topSnake, bottomSnake,
        leftFood, rightFood,
        topFood, bottomFood;

    leftSnake = snake->x;
    rightSnake = snake->x + snake->w;
    topSnake = snake->y;
    bottomSnake = snake->y + snake->h;


    leftFood = food->x;
    rightFood = food->x + food->w;
    topFood = food->y;
    bottomFood = food->y + food->h;

    //kiem tra su va cham giua snake va food
    if (bottomSnake <= topFood) 
    {
        return false;
    }

    if (topSnake >= bottomFood) 
    {
        return false;
    }

    if (rightSnake <= leftFood) 
    {
        return false;
    }

    if (leftSnake >= rightFood) 
    {
        return false;
    }

    return true;
}



/*------------------------------------*
 *          FOOD OBJECT               *
 *------------------------------------*/

void Food::showFood() 
{
    int x, y;
    srand((int)time(0));
    bool coordinate = true;
    while (coordinate)  //sinh toa do ngau nhien cho food
    {
        x = rand() % 1171,
        y = rand() % 691 + 80;
        if (x % size == 0 && y % size == 0)
        {
            coordinate = false;
        }
    }
    CurrPos.setPosition(x, y);
}

int Food::getSize() 
{
    return this->size;
}

int Food::getXpos() 
{
    return CurrPos.getXpos();
}

int Food::getYpos() 
{
    return CurrPos.getYpos();
}

void Food::setPosition(int x, int y) 
{
    CurrPos.setPosition(x, y);
}



/*------------------------------------*
 *          SCORE OBJECT              *
 *------------------------------------*/

Score::Score()  //ham tao
{
    currScore = 0;

    getfileHighScore();
}

void Score::increaseScore() 
{
    currScore += 10;
}

int Score::getCurrScore() 
{
    return this->currScore;
}

int Score::getHighScore() 
{
    return this->highScore;
}

void Score::resetScore() 
{
    currScore = 0;
}

void Score::getfileHighScore() 
{
    char num[10];
    ifstream file;

    file.open("score/highscore.txt", ios::in);
    file.getline(num, 10);

    int temp;

    // chuyen doi tu kieu char sang int
    sscanf(num, "%d", &temp);

    // luu tru highscore moi
    this->highScore = temp;

    file.close();
}

void Score::setfileHighScore() 
{
    ofstream file;

    file.open("score/highscore.txt", ios::out | ios::trunc);

    file << this->highScore;

    file.close();
}

void Score::checkNewHighScore() 
{
    if (currScore > highScore) 
    {
        highScore = currScore;

        // cap nhat lai file highscore
        setfileHighScore();
    }
}