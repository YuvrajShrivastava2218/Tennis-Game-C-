#include "raylib.h"
#include <iostream>
using namespace std;

Color Green = Color{38 , 185 , 154 , 255};
Color DarkGreen = Color{20 , 160 , 133 , 255};
Color LightGreen = Color{129 , 204 , 184 , 255};
Color Yellow = Color{243 , 213 , 91 , 255};

long long playerScore = 0;
long long cpuScore = 0;

class Paddle{
protected:
    void LimitMovement(){
        if(this->y < 0) this->y = 0;
        if(this->y > GetScreenHeight() - this->height) this->y = GetScreenHeight() - this->height;
    }
public:
    float x;
    float y;
    float width;
    float height;
    int speed;

    void Draw(){
        DrawRectangleRounded(Rectangle{this->x , this->y , this->width , this->height} , 0.8 , 0 , WHITE);
    }
    
    void Update(){
        if(IsKeyDown(KEY_UP)) this->y -= this->speed;
        if(IsKeyDown(KEY_DOWN)) this->y += this->speed;
        LimitMovement();
    }
};

class AIPaddle : public Paddle{
public:
    void Update(float ballY){
        if(ballY > this->y + this->height / 2) this->y += this->speed;
        else if(ballY < this->y + this->height / 2) this->y -= this->speed;
        LimitMovement();
    }
};

class Ball{
public:
    float x;
    float y;
    float speedX;
    float speedY;

    void Draw(){
        DrawCircle(x , y , 20 , Yellow);
    }

    void Reset(){
        this->x = GetScreenWidth() / 2;
        this->y = GetScreenHeight() / 2;

        int choices[2] = {-1 , 1};

        this->speedX *= choices[GetRandomValue(0 , 1)];
        this->speedY *= choices[GetRandomValue(0 , 1)];
    }

    void Update(){
        this->x += this->speedX;
        this->y += this->speedY;

        if(this->y >= GetScreenHeight() - 20 || this->y < 20) this->speedY *= -1;

        if(this->x >= GetScreenWidth() - 20){
            playerScore++;
            Reset();
        }else if(this->x < 20){
            cpuScore++;
            Reset();
        }
    }

};

Ball ball;
Paddle player;
AIPaddle cpu;

int main(){
    const int window_width = 1280;
    const int window_height = 700;

    InitWindow(window_width , window_height , "Pong");
    SetTargetFPS(60);

    ball.x = window_width / 2;
    ball.y = window_height / 2;
    ball.speedX = 7;
    ball.speedY = 7;

    player.width = 25;
    player.height = 120;
    player.x = 10;
    player.y = window_height / 2 - player.height / 2;
    player.speed = 5;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = window_width - cpu.width - 10;
    cpu.y = window_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(DarkGreen);
        DrawRectangle(window_width / 2 , 0 , window_width / 2 , window_height , Green);
        DrawCircle(window_width / 2 , window_height / 2 , 150 , LightGreen);

        ball.Update();
        cpu.Update(ball.y);
        player.Update();

        if(CheckCollisionCircleRec(Vector2{ball.x , ball.y} , 20 , Rectangle{player.x , player.y , player.width , player.height})) ball.speedX *= -1;
        if(CheckCollisionCircleRec(Vector2{ball.x , ball.y} , 20 , Rectangle{cpu.x , cpu.y , cpu.width , cpu.height})) ball.speedX *= -1;

        ball.Draw();
        player.Draw();
        cpu.Draw();    

        DrawLine(window_width / 2 , 0 , window_width / 2 , window_height , WHITE);
        DrawText(TextFormat("%i" , playerScore) , window_width / 4 - 20 , 20 , 80 , WHITE);
        DrawText(TextFormat("%i" , cpuScore) , 3 * window_width / 4 - 20 , 20 , 80 , WHITE);

        EndDrawing();
    }

    return 0;
}