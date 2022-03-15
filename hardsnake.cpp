#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <ncurses.h>

//Testing branches

using namespace std;

int hoehe;
int breite;
char input;
int score;

class Player{
    public:
    int pposX;
    int pposY;
    int velocityx;
    int velocityy;
    int pposXh[100];
    int pposYh[100];
    int nTail;
    int nTailx[100];
    int nTaily[100];
    char sign;

    Player (int x, int y, char c){
        pposX = x;
        pposY = y;
        sign = c;
    }

    void move(char direction){
         
         switch(direction){
            case 'w': velocityy = -1; velocityx = 0; break;
            case 's': velocityy = 1; velocityx = 0; break;
            case 'd': velocityx = 1; velocityy = 0; break;
            case 'a': velocityx = -1; velocityy = 0; break;
        }
        //Neue Position
        if(velocityx == 1) {
            for(int i = nTail; i >= 1; i--){ 
                nTailx[i] = nTailx[i-1];
                nTaily[i] = nTaily[i-1];
            }
            nTailx[0] = pposX;
            nTaily[0] = pposY;     
            pposX = pposX + 1;
        }
        else if(velocityx == - 1){
            for(int i = nTail; i >= 1; i--){ 
                nTailx[i] = nTailx[i-1];
                nTaily[i] = nTaily[i-1];
            }
            nTailx[0] = pposX;
            nTaily[0] = pposY;
            pposX = pposX -1;
            
        }
        else if(velocityy == 1){
            for(int i = nTail; i >= 1; i--){ 
                nTailx[i] = nTailx[i-1];
                nTaily[i] = nTaily[i-1];
            }
            nTailx[0] = pposX;
            nTaily[0] = pposY;
            pposY = pposY + 1;
            
        }
        else if(velocityy == - 1){
            for(int i = nTail; i >= 1; i--){ 
                nTailx[i] = nTailx[i-1];
                nTaily[i] = nTaily[i-1];
            }
            nTailx[0] = pposX;
            nTaily[0] = pposY;
            pposY = pposY - 1;
            
        }
        //Prüfen ob Schlange an Wand
        if(pposY == 0) {
            //pposY = pposY +1;
            input = 'x';
        }
        if(pposY == hoehe-1){
            //pposY = pposY -1;
            input = 'x';

        }
        if(pposX == 0){
            //pposX = pposX +1;
            input = 'x';
        }
        if(pposX == breite-1){
            //pposX = pposX -1;
            input = 'x';
        }
        //Prüfen ob Schlangenkopf mit Schlange kollidiert
        for(int i = 0; i < nTail; i++){
            if(pposX == nTailx[i] && pposY == nTaily[i]){
                input = 'x';
            }
        }
    }
};

class Food{
    public:
    char fsign;
    int fposX;
    int fposY;

    Food(char c){
        fsign = c;
    }

    void spawn(){
        //srand((unsigned) time(0));
        fposX = rand() % (breite-2) + 1; // Zufallszahl zwischen 1 und breite-1
        fposY = rand() % (hoehe-2) + 1;// Zufallszahl zwischen 1 und hoehe-1
        //printf("%d\n", fposX);
        //printf("%d\n", fposY);
    }
};

void printFrame(char *field, Player player, Food food);
int checkFood(Player& player, Food food);

int Ymax;
int counter = 0;

int posX = 0;
int posY = 0;

char sframe = '#';

int main(){
    srand((unsigned) time(0));
    input = 'w';

    //cout << "Rahmenhöhe: \n";
    //cin >> hoehe;
    hoehe = 12;
    //cout << "Rahmenbreite: \n";
    //cin >> breite;
    breite = 12;
    Ymax = breite;

    //Player initialisierung
    Player player1((hoehe-1)/2, (breite-1)/2, '0');
    player1.velocityx = (rand() - RAND_MAX/2) % 2;
    player1.velocityy = (rand() - RAND_MAX/2) % 2;
    player1.nTail = 0;
    player1.nTailx[0] = player1.pposX;
    player1.nTaily[0] = player1.pposY;

    //Food initialisieren
    Food food1('*');
    food1.spawn();
    //Spielfeld initialisieren
    char *field = NULL;
    field = new char[breite*hoehe];
    for(int i = 0; i<breite*hoehe; i++){
        *(field+i) = ' ';
    }

    initscr();
    noecho();
    nodelay(stdscr, TRUE);

    do{
        //cout << "WASD zum steuern" << "x zum beenden";
        //input = getch();
        printw("SNAKE\n");
        player1.move(input);
        if(checkFood(player1, food1)){ //checkFood: 1 wenn Player auf Food, 0 wenn Player nicht auf Food
            food1.spawn();
            for(int i = 0; i <= player1.nTail; i++){
                if(player1.nTailx[i] == food1.fposX && player1.nTaily[i] == food1.fposY){
                    food1.spawn();
                }
            }
        }
        printFrame(field, player1, food1);
        printw("Score: %i\n", score);
        //printw("nTail: %i\n", player1.nTail);

        refresh();
        clear();
        //usleep(1000000 * 0.2);
        if(input != 'x'){
            usleep(1000000 * 0.1);
            input = getch();
        }
    }while(input != 'x');
    clear();
    nodelay(stdscr, FALSE);
    printw("GAME OVER!\n");
    refresh();
    endwin();
    delete [] field;
}

void printFrame(char *field, Player player, Food food){
    do{
        if(posY == 0 || posY == hoehe-1){
            *(field + posX + posY * Ymax) = sframe;
            printw("%c ", *(field + posX + posY * Ymax));
            posX = posX + 1;
            counter++;
            if(posX == breite-1){
                *(field + posX + posY * Ymax) = sframe;
                printw("%c\n", *(field + posX + posY * Ymax));
                posY = posY + 1;
                posX = 0;
                counter++;
            }
        }
        else if(posY !=0 || posY !=hoehe-1){
            if(posX == 0){ 
                *(field + posX + posY * Ymax) = sframe;
                printw("%c ", *(field + posX + posY * Ymax));
                posX = posX + 1;
                counter++;
            }else if(posX == breite-1){
                *(field + posX + posY * Ymax) = sframe;
                printw("%c\n", *(field + posX + posY * Ymax));
                posY = posY + 1;
                posX = 0;
                counter++;
            }
            else{
                for(int i = 0; i<player.nTail; i++){
                    if(posX == player.nTailx[i] && posY == player.nTaily[i]){
                        printw("%c ", player.sign);
                        posX = posX + 1;
                        counter++;
                        i = -1;
                    }
                }
                if(posX == player.pposX && posY == player.pposY){
                    printw("%c ", player.sign);
                    posX = posX +1;
                    counter++;
                } 
                else if(posX == food.fposX && posY == food.fposY){
                    printw("%c ", food.fsign);
                    posX = posX +1;
                    counter++;
                }else if(posX == breite-1){
                    *(field + posX + posY * Ymax) = sframe;
                    printw("%c\n", *(field + posX + posY * Ymax));
                    posY = posY + 1;
                    posX = 0;
                    counter++;
                }
                else{   
                    printw("%c ", *(field + posX + posY * Ymax));
                    posX = posX + 1;
                    counter++;
                }    
            }
        }

    }while(counter < hoehe*breite);
    posX = 0;
    posY = 0;
    counter = 0;
}

int checkFood(Player& player, Food food){
    if(player.pposX == food.fposX && player.pposY == food.fposY){
        player.nTail++;
        score = score + 10;
        return 1;
    }
    else{
        return 0;
    }
}
