#include<iostream> 
#include<vector>//used to create vector
#include<deque>//used to create deque

#include<thread>//multithreading support
#include<chrono>//time measurement

#include<cstdlib>//clear the screen
#include<ctime>

#include <conio.h>//non-blocking detection
#include<windows.h>

#include<algorithm>

using namespace std;
using namespace std::chrono;

int HEIGHT=20,WIDTH=40;//the size of the map
int INIT_LENGTH=3;//initiative snake's body length
int MOVE_INTERNAL_MS=150;//time the game need to renew

enum class Direction{UP,DOWN,LEFT,RIGHT};//the four kind of direction

struct Point{int x,y;};//represent the position
deque<Point> snake;    //the snake's position
Direction dir;    //the direction snake moves, and initiate it as right
Direction nextDir;//the direction snake moves next, right too
Point food; //food's positon
int score;//your total score
bool gameOver;

//---------Cursor Operation--------
void gotoxy(int x,int y){//gotoxy's way to locate is column first, row second  !!!
    COORD coord = {(SHORT)x,(SHORT)y};//use COORD type to record needed position
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//select this command line
    //AI said is getting handle to console output, but I dont get it
    SetConsoleCursorPosition(hConsole,coord);//set the position of cursor
}

void hideCursor(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    SetConsoleCursorPosition(hConsole,{0,0});
    SetConsoleCursorInfo(hConsole,&info);
}

//--------initiate the game--------
void generateFood(){
    do{
        food.x=rand()%(WIDTH-2)+1;
        food.y=rand()%(HEIGHT-2)+1;//make sure that the food will in the map;
    }while(find_if(snake.begin(),snake.end(),
            [&](const Point&p){return p.x==food.x&&p.y==food.y;})!=snake.end());
}

void initiateGame(){
    srand((unsigned)time(nullptr));//srand comes from cstdlib, and time comes from ctime.
    //first feed this function with current time so that we can then produce random number.

    snake.clear();
    int startX=WIDTH/2;
    int startY=HEIGHT/2;
    for(int i=0;i<INIT_LENGTH;i++){
        snake.push_back({startX-i,startY});//initiate the position of snake
    }
    dir=Direction::RIGHT;
    nextDir=Direction::RIGHT;
    score=0;
    gameOver=false;

    generateFood();
}

//--------draw the map--------
void draw(){
    //first the wall
    for(int i=0;i<HEIGHT;i++){
        gotoxy(0,i);//column first, column second
        for(int j=0;j<WIDTH;j++){
            if(i==0||i==HEIGHT-1||j==0||j==WIDTH-1){
                cout<<"#";
            }
            else cout<<' ';
        }
    }

    //second the snake
    for(int i=0;i<snake.size();i++){
        gotoxy(snake[i].x,snake[i].y);
        if(i==0) cout<<'@';
        else cout<<'0';
    }

    //third the food
    gotoxy(food.x,food.y);
    cout<<'O';

    //last the score
    gotoxy(0,HEIGHT);
    cout<<"Your Score:"<<score;
    gotoxy(0,HEIGHT+1);
    cout<<"Press Arrow Keys to move. Press ESC to quit";
}

//--------non blocking input processing--------
void handleInput(){
    if(_kbhit()){
        //full name keyboard hit to check if buffer has something in it.
        //if yes, you have to use _getch to achieve.
        //if no, return 0 right now.
        int key = _getch();
        //eww...maybe every key has its own number to represent itself
        //and the direction key additionally has a front number to handle
        if(key==224){
            key=_getch();
            switch(key){
            case 72:nextDir=Direction::UP;break;
            case 80:nextDir=Direction::DOWN;break;
            case 75:nextDir=Direction::LEFT;break;
            case 77:nextDir=Direction::RIGHT;break;
            }
        }
        else if(key==27){//represent the "ESC"
            gameOver=true;
        }
    }
}

//--------move the snake--------
bool moveSnake(){
    //find the direction snake ready to move towards
    Direction newDir=nextDir;
    if((newDir == Direction::UP && dir == Direction::DOWN) ||
        (newDir == Direction::DOWN && dir == Direction::UP) ||
        (newDir == Direction::LEFT && dir == Direction::RIGHT) ||
        (newDir == Direction::RIGHT && dir == Direction::LEFT)){
            newDir=dir;// the reversal is invalid)
        }
    dir=newDir;//get the directino

    Point newHead = snake.front();
    switch(dir){
        case Direction::UP: newHead.y--;break;
        case Direction::DOWN: newHead.y++;break;
        case Direction::LEFT: newHead.x--;break;
        case Direction::RIGHT: newHead.x++;break;
    }

    //collision detection
    //detect the wall
    if(newHead.x <= 0 || newHead.x >= WIDTH-1 || newHead.y <= 0 || newHead.y >= HEIGHT-1){
        return false;
    }
    //detect the snake itself
    auto bodyPart=find_if(snake.begin(),snake.end(),[&](const Point&p){return p.x==newHead.x&&p.y==newHead.y;});
    if(bodyPart!=snake.end()&&bodyPart!=snake.end()-1){return false;}

    snake.push_front(newHead);

    bool ateFood = newHead.x==food.x&&newHead.y==food.y;//check if snake ate the food
    if(ateFood){
        score++;
        generateFood();
    }
    else snake.pop_back();
    
    return true;
}

//--------deal with the gameOver--------
void gameEnd(){
    gotoxy(0,HEIGHT-2);
    cout<<"Game Over! Final Score:"<<score<<". Press any key to exit...";
    _getch();
}

int main(){
    system("title The Greedy Snake");
    hideCursor();//deal with the command line

    initiateGame();
    draw();//initiate the game at first

    auto lastMove=steady_clock::now();
    while(!gameOver){
        handleInput();

        auto now=steady_clock::now();
        if(duration_cast<milliseconds>(now-lastMove).count()>=MOVE_INTERNAL_MS){
            if(!moveSnake()){
                gameOver=true;
                break;
            }
            draw();
            lastMove=now;
        }

        this_thread::sleep_for(milliseconds(10));//reduce the occupation of CPU
    }

    gameEnd();
    return 0;
}