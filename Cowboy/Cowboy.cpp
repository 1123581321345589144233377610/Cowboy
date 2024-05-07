#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <vector>
#include <utility>

const int ROWS = 15;
const int COLS = 30;

bool isRunning = true;
int kills = 0;


struct enemy{
    int x;
    int y;
    char sym = '0';
    short hp = 1;
    short dm = 1;
};

struct character{
    int x;
    int y;
    char sym = '*';
    short hp = 10;
    short dm = 1;
};

character cowboy;
enemy* bandits = new enemy[10000];

std::vector<std::pair<int, int>> bandits_coor;

int size = 0;

void generate_enemies(char MAP[ROWS][COLS], std::vector<std::pair<int , int>>& bandits_coor) {
    srand(time(0));
    for (int i = 0; i < rand() % 2; ++i) {
        bandits[i].x = rand() % (ROWS - 2) + 1;
        bandits[i].y = rand() % (COLS - 3) + 1;
        MAP[bandits[i].x][bandits[i].y] = bandits[i].sym;
        bandits_coor.emplace_back(std::make_pair(bandits[i].x, bandits[i].y));
        ++size;
    }
}

void fight(int x, int y, std::vector<std::pair<int, int>>& bandits_coor) {
    std::pair<int, int> cowboy_coor = std::make_pair(x, y);
    for (int i = 0; i < size; ++i) {
        if (cowboy_coor == bandits_coor[i]) {
            if ((cowboy.hp - bandits[i].dm) > 0) {
                cowboy.hp -= bandits[i].dm;
                bandits[i].hp -= cowboy.dm;
                bandits_coor.erase(bandits_coor.begin() + i);
                --size;
                ++kills;
                break;
            }
            else if (cowboy.hp - bandits[i].dm <= 0) {
                std::cout << "\nYOU ARE DIED!\n";
                isRunning = false;
                break;
            }
        }
    }
}

void shoting(int x, int y, char dir, std::vector<std::pair<int, int>>& bandits_coor, char MAP[ROWS][COLS]) {
    int dx = 0, dy = 0;
    if (GetKeyState(' ') & 0x8000) {
        if (dir == 'W') {
            dx = -1;
        }
        else if (dir == 'S') {
            dx = 1;
        }
        else if (dir == 'A') {
            dy = -1;
        }
        else if (dir == 'D') {
            dy = 1;
        }
        int newX = x + dx;
        int newY = y + dy;

        int s = 0;

        while (newX != 0 && newX != ROWS - 1 && newY != 0 && newY != COLS - 2 && s < 1) {
            int size = bandits_coor.size();
            std::pair<int, int> bullet_coor = std::make_pair(newX, newY);
            MAP[newX][newY] = '-';
            for (int i = 0; i < size; ++i) {
                if (bullet_coor == bandits_coor[i]) {
                    remove(bandits_coor.begin(), bandits_coor.end(), bandits_coor[i]);
                    ++kills;
                    ++s;         
                    break;                    
                }
            }
            MAP[newX][newY] = ' '; 
            newX += dx;
            newY += dy;
        } 
    }
}

void output(int x, int y) {
    std::cout << std::endl << "HP:" << cowboy.hp << " || DM:" << cowboy.dm << " || KILLPOINTS:" << kills;
}

void moving(int& x, int& y,char& dir, char MAP[ROWS][COLS]) {
    MAP[x][y] = ' ';
    if (GetKeyState('W') & 0x8000) {
        --x;
        dir = 'W';
    }else if (GetKeyState('S') & 0x8000) {
        ++x;
        dir = 'S';
    }else if (GetKeyState('A') & 0x8000) {
        --y;
        dir = 'A';
    }else if (GetKeyState('D') & 0x8000) {
        ++y;
        dir = 'D';
    }
    cowboy.x = x;
    cowboy.y = y;
    MAP[cowboy.x][cowboy.y] = cowboy.sym;
}

void gotoxy(int x, int y)
{
    COORD Coor;
    Coor.X = x; Coor.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coor);
}

void map_draw(char MAP[ROWS][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        std::cout << "\n";
        for (int j = 0; j < COLS; ++j) {
            std::cout << MAP[i][j];
        }
    }
}

void check_died(int x, int y) {
    if (y == 0 || y == COLS - 2 || x == 0 || x == ROWS - 1) {
        std::cout << "YOU ARE DIED!\n";
        isRunning = false;
    }
}

void rules() {
    std::cout << "\n\033[1;31;40mCowboy - это первая игра с замечательной боёвкой и сногсшибательным сюжетом про ковбоя, который обороняет своё ранчо от бандитов!\033[0m\n";
    std::cout << "\033[5;33;40mW - ВВЕРХ\n" << "S - ВНИЗ\n" << "A - ВЛЕВО\n" << "D - ВПРАВО\n" << "SPACE - ВЫСТРЕЛ\033[0m\n";
    std::cout << "\033[1;36;40mИ помни, чем больше выстрелов ты сделал по врагу, тем больше твои KILLPOINTS!\033[0m\n";
}


int main() {
    setlocale(LC_CTYPE, "rus");
    rules();
    int x = 1, y = 1;
    char dir;
    char MAP[ROWS][COLS] = {
    "#############################",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#############################",
    };
    while (isRunning) {
        map_draw(MAP);  
        output(x, y);
        generate_enemies(MAP, bandits_coor);
        moving(x, y,dir, MAP);
        fight(x, y, bandits_coor);

        shoting(x, y, dir, bandits_coor, MAP);

        gotoxy(0, 0);
        system("pause");
        system("cls");
        check_died(x, y);
    }
    return 0;
}

    