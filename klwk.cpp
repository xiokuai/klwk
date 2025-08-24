#include<iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <string>
using namespace std;

// 游戏常量定义
const int MAP_SIZE = 7;
const int PLAYER = 1;
const int GOAL = 2;
const int WALL = 3;
const int TRAP = 4;
const int DOOR = 5;
const int EMPTY = 0;

// 游戏状态结构
struct GameState {
    int map[MAP_SIZE][MAP_SIZE];
    int playerX, playerY;
    int goalX, goalY;
    vector<pair<int, int>> traps;
    int level;
};

// 函数声明
void initializeConsole();
void initializeGame(GameState& game, int level);
void generateTraps(GameState& game);
void printMap(const GameState& game);
void processInput(GameState& game, char input);
bool checkGameStatus(const GameState& game);
void clearScreen();

int main() {
    initializeConsole();
    srand(time(0));
    
    for(int level = 1; ; level++) {
        GameState game;
        initializeGame(game, level);
        
        while(true) {
            printMap(game);
            
            if(!checkGameStatus(game)) {
                break;
            }
            
            char input;
            cout << "请进行移动 ";
            cin >> input;
            
            processInput(game, input);
            clearScreen();
        }
        
        clearScreen();
    }
    
    return 0;
}

void initializeConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void initializeGame(GameState& game, int level) {
    // 初始化基础地图
    int baseMap[MAP_SIZE][MAP_SIZE] = {
        {3,5,3,3,3,3,3},
        {5,1,0,0,0,0,3},
        {3,0,0,0,0,0,3},
        {3,0,0,0,0,0,3},
        {3,0,0,0,0,0,3},
        {3,0,0,0,0,2,3},
        {3,3,3,3,3,3,3}
    };
    
    // 复制基础地图
    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            game.map[i][j] = baseMap[i][j];
            if(game.map[i][j] == PLAYER) {
                game.playerX = i;
                game.playerY = j;
            } else if(game.map[i][j] == GOAL) {
                game.goalX = i;
                game.goalY = j;
            }
        }
    }
    
    game.level = level;
    game.traps.clear();
    generateTraps(game);
}

void generateTraps(GameState& game) {
    // 生成第一个陷阱
    int trap1X = rand() % 5 + 1;
    int trap1Y;
    if(trap1X == 1) {
        trap1Y = rand() % 4 + 2;
    } else if(trap1X == 5) {
        trap1Y = rand() % 4 + 1;
    } else {
        trap1Y = rand() % 5 + 1;
    }
    game.map[trap1X][trap1Y] = TRAP;
    game.traps.push_back({trap1X, trap1Y});
    
    // 生成第二个陷阱（确保不与第一个重合）
    int trap2X, trap2Y;
    do {
        trap2X = rand() % 5 + 1;
        if(trap2X == 1) {
            trap2Y = rand() % 4 + 2;
        } else if(trap2X == 5) {
            trap2Y = rand() % 4 + 1;
        } else {
            trap2Y = rand() % 5 + 1;
        }
    } while(trap2X == trap1X && trap2Y == trap1Y);
    
    game.map[trap2X][trap2Y] = TRAP;
    game.traps.push_back({trap2X, trap2Y});
}

void printMap(const GameState& game) {
    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            switch(game.map[i][j]) {
                case PLAYER: cout << "人"; break;
                case EMPTY: cout << "__"; break;
                case GOAL: cout << "星"; break;
                case WALL: cout << "墙"; break;
                case TRAP: cout << "火"; break;
                case DOOR: cout << "门"; break;
                default: cout << "  "; break;
            }
        }
        cout << endl;
    }
    
    cout << endl << "当前关数：" << game.level << endl;
    if(game.level == 5) {
        cout << "PS：您可以输入ddd来右移三格，以此类推" << endl;
    }
}

void processInput(GameState& game, char input) {
    int newX = game.playerX;
    int newY = game.playerY;
    
    switch(input) {
        case 's': if(newX < 5) newX++; break;
        case 'w': if(newX > 1) newX--; break;
        case 'd': if(newY < 5) newY++; break;
        case 'a': if(newY > 1) newY--; break;
        default:
            cout << "\n输" << endl;
            system("pause");
            exit(0);
    }
    
    // 更新玩家位置
    game.map[game.playerX][game.playerY] = EMPTY;
    game.playerX = newX;
    game.playerY = newY;
    game.map[game.playerX][game.playerY] = PLAYER;
}

bool checkGameStatus(const GameState& game) {
    // 检查胜利条件
    if(game.playerX == game.goalX && game.playerY == game.goalY) {
        cout << "\nvictory" << endl;
        system("pause");
        return false;
    }
    
    // 检查陷阱条件
    for(const auto& trap : game.traps) {
        if(game.playerX == trap.first && game.playerY == trap.second) {
            cout << "\nlose" << endl;
            system("pause");
            exit(0);
        }
    }
    
    // 检查特殊边界条件
    if((game.playerX == 1 && game.playerY == 0) || (game.playerX == 0 && game.playerY == 1)) {
        if(rand() % 2 == 1) {
            cout << "\nvictory" << endl;
            system("pause");
            return false;
        } else {
            cout << "\nlose" << endl;
            system("pause");
            exit(0);
        }
    }
    
    return true;
}

void clearScreen() {
    system("cls");
}
