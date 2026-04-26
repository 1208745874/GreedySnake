#include<iostream> 
#include<vector>//used to create vector
#include<thread>//multithreading support
#include<chrono>//time measurement
#include<cstdlib>//clear the screen
#include <conio.h>//non-blocking detection

int main(){
    std::vector<std::vector<char>> map(20, std::vector<char>(20,' '));
    //first create a map for the snake to move
    std::vector<int> direction{1,2,3,4};
    //set the direction

    int x=0,y=0;
    while(1){
        map[x++][y]=' ';
        if(x==20)x=0;
        map[x][y]='#';
        for(int i=0;i<20;i++){//print the map to the console
            for(int j=0;j<20;j++){
                std::cout<<map[i][j];
            }
            std::cout<<std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));//wait 500ms
        system("cls");//clear the screen

        /*if (_kbhit()) {//listen the input
            _getch();
            break;
        }*/
    }
    
    
    return 0;
}