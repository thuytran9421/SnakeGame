#include "SnakeGame.h"
#undef main

class PlaySnake {
    Snake snake;
    Food food;
    Score score;

    Window gameWindow;
    SDL_Rect* rectSnakeTail, rectFood, rectScore, rectGame;
    SDL_Event event;

    TTF_Font* font;

    bool isStarted, gameReset;

public:
    PlaySnake(int WIDTH, int HEIGHT) {
        gameWindow.SetWindowSize(WIDTH, HEIGHT);

        isStarted = false;   
        gameReset = false;   

        snake.setPosition((WIDTH /10), (HEIGHT * 0.9) / 2); //thiet lap vi tri ban dau cua snake
        rectSnakeTail = new SDL_Rect[snake.getLength()];

        rectSnakeTail[0].w = snake.getSize();
        rectSnakeTail[0].h = snake.getSize();
        rectSnakeTail[0].x = snake.getXpos();
        rectSnakeTail[0].y = snake.getYpos();

        for (int i = 1; i<snake.getLength(); i++) {
            rectSnakeTail[i].w = snake.getSize();
            rectSnakeTail[i].h = snake.getSize();
            rectSnakeTail[i].x = rectSnakeTail[i - 1].x - snake.getSize();
            rectSnakeTail[i].y = rectSnakeTail[i - 1].y;
        }

        // dien tich cho score
        rectScore.x = 0;
        rectScore.y = 0;
        rectScore.w = gameWindow.WIN_WIDTH;
        rectScore.h = gameWindow.WIN_HEIGHT * 0.1;

        // dien tich de choi game
        rectGame.x = 0;
        rectGame.y = rectScore.h;
        rectGame.w = gameWindow.WIN_WIDTH;
        rectGame.h = gameWindow.WIN_HEIGHT * 0.9;
    }

    bool cham(SDL_Rect head, SDL_Rect body)
    {
        int left1Head, right1Head,
            left2Head, right2Head,
            left1Body, right1Body,
            left2Body, right2Body;

        left1Head = head.x;
        right1Head = head.x + head.w;
        left2Head = head.x;
        right2Head = head.x + head.w + snake.getSize();


        left1Body = body.x;
        right1Body = body.x + body.w;
        left2Body = body.x;
        right2Body = body.x + body.w + snake.getSize();

        //kiem tra su va cham giua dau va than ran
        if (left1Head == left2Body && right1Head == right2Body) //cham tu duoi len
        {
            return true;
        }
        else if (left1Head == right1Body && left2Head == right2Body) //cham tu phai sang
        {
            return true;
        }
        else if (left2Head == left1Body && right2Head == right1Body)  //cham tu tren xuong
        {
            return true;
        }
        else if (right1Head == left1Body && right2Head == left2Body) //cham tu trai sang phai
        {
            return true;
        }
        else return false;

    }

    void RenderObjects() {
        // Score
        SDL_RenderSetClipRect(gameWindow.renderer, &rectScore);
        SDL_SetRenderDrawColor(gameWindow.renderer, 180, 200, 90, 255);
        SDL_RenderFillRect(gameWindow.renderer, &rectScore);
        DisplayScore();

        // Game
        SDL_SetRenderDrawColor(gameWindow.renderer, 150, 217, 109, 255);
        SDL_RenderSetClipRect(gameWindow.renderer, &rectGame);
        SDL_RenderFillRect(gameWindow.renderer, &rectGame);

        // Food
        SDL_SetRenderDrawColor(gameWindow.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(gameWindow.renderer, &rectFood);

        //Snake
        for (int i = 0; i < snake.getLength(); i++)
        {
            SDL_SetRenderDrawColor(gameWindow.renderer, 0, 0, 225, 255);
            SDL_RenderFillRect(gameWindow.renderer, &rectSnakeTail[i]);
        }

        // hien thi moi thu len cua so game
        SDL_RenderPresent(gameWindow.renderer);
 
        if (isStarted) {    
            snake.move();
        }

        // cap nhat lai vi tri cho ran
        rectSnakeTail[0].w = snake.getSize();
        rectSnakeTail[0].h = snake.getSize();
        rectSnakeTail[0].x = snake.getXpos();
        rectSnakeTail[0].y = snake.getYpos();
        for (int i = snake.getLength()-1; i > 0; i--)
        {
            rectSnakeTail[i].w = snake.getSize();
            rectSnakeTail[i].h = snake.getSize();
            rectSnakeTail[i].x = rectSnakeTail[i - 1].x;
            rectSnakeTail[i].y = rectSnakeTail[i - 1].y;
        }

        // cap nhat lai vi tri do an
        rectFood.x = food.getXpos();
        rectFood.y = food.getYpos();

        // kiem tra xem ran co cham vien khong
        // neu co, gameover va reset lai vi tri cho ran
        int sx = rectSnakeTail[0].x,
            sy = rectSnakeTail[0].y;

        if ((sx == 0) || (sx == 1170) ||
            (sy == 80) || (sy == 770)) {
            isStarted = false;
            gameReset = true;
            score.checkNewHighScore();
        }
        // kiem tra xem ran an duoc do an khong
        if (snake.eat(&rectSnakeTail[0], &rectFood)) {
            food.showFood();
            score.increaseScore();
            snake.setLength(snake.getLength() +  1);
        }
        // kiem tra ran co tu can minh khong
        if (snake.getLength() > 5)
        {
            for (int i = 4; i < snake.getLength(); i++)
            {
                if (cham(rectSnakeTail[0], rectSnakeTail[i]))
                {
                    isStarted = false;
                    gameReset = true;
                    score.checkNewHighScore();
                }
            }
        }
    }

    // ve text len cua so game
    void DisplayScore() {
        // khoi tao thu vien render font chu
        TTF_Init();

        // mo file font chu
        TTF_Font* font = TTF_OpenFont("font/RockoFLF-Bold.ttf", 150);

        // hien thi diem hien tai
        char* text = new char[16];
        sprintf(text, "Score: %d", score.getCurrScore());

        SDL_Color color = { 170, 68, 0 };
        //tao doi tuong surface va texture
        SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);

        int TextWidth = 200,
            TextHeight = 90;

        SDL_Rect rect = { 970, -7, TextWidth, TextHeight };
        SDL_RenderCopy(gameWindow.renderer, texture, NULL, &rect);

        // hien thi highscore
        sprintf(text, "High Score: %d", score.getHighScore());

        surface = TTF_RenderText_Blended(font, text, color);
        texture = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);

        //toa do ve
        rect.x = 20;
        rect.w = 330;

        SDL_RenderCopy(gameWindow.renderer, texture, NULL, &rect);

        // giai phong bo nho
        delete text;
        TTF_CloseFont(font);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        TTF_Quit();
    }

    //kiem tra xem game chay khong
    bool checkIsGameStarted() {
        if (!isStarted) {
            return true;
        }
        else {
            return false;
        }
    }


    void startRestartGame() {
        if (gameReset == true) {
            // cap nhat lai vi tri cua snake
            snake.setPosition(
                (gameWindow.WIN_WIDTH / 10),
                (gameWindow.WIN_HEIGHT * 0.9) / 2);

            food.setPosition(0, 0);
            snake.reset();
            score.resetScore();

            gameReset = false;
        }
        else {
            // tao food
            food.showFood();

            rectFood.w = food.getSize();
            rectFood.h = food.getSize();
            rectFood.x = food.getXpos();
            rectFood.y = food.getYpos();

            // bat dau game
            isStarted = true;
        }
    }


    bool HandleEvents() {   //ham xu ly su kien
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                if (gameWindow.window) {
                    gameWindow.Destroy();
                    gameWindow.window = NULL;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    if (checkIsGameStarted()) {
                        startRestartGame();
                        break;
                    }
                    else {
                        snake.setDirection('l');
                    }
                    break;   
                case SDLK_RIGHT:
                    if (checkIsGameStarted()) {
                        startRestartGame();
                        break;
                    }
                    else {
                        snake.setDirection('r');
                    }
                    break; 
                case SDLK_UP:
                    if (checkIsGameStarted()) {
                        startRestartGame();
                        break;
                    }
                    else {
                        snake.setDirection('u');
                    }
                    break;   
                case SDLK_DOWN:
                    if (checkIsGameStarted()) {
                        startRestartGame();
                        break;
                    }
                    else {
                        snake.setDirection('d');
                    }
                    break;

                default:
                    if (checkIsGameStarted()) {
                        startRestartGame();
                    }
                }
                break;

            case SDL_QUIT:
                gameWindow.Destroy();
                return true;
            }
        }
    }

    void StartGame() {
        gameWindow.Create_Window();

        bool done = false;

        while (!done) {
            done = HandleEvents();

            RenderObjects();

            SDL_Delay(20);
        }
    }
};

int main(int argc, char argv[]) {
    PlaySnake SnakeGame(1200, 800);

    SnakeGame.StartGame();
}