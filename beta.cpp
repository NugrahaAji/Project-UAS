#include<conio.h>
#include<windows.h>
#include<iostream>
using namespace std;

#define SCHEIGHT 26
#define SCWIDTH 90
#define MAPHEIGHT 16
#define MAPWIDTH 81

#define BLUE 1
#define YELLOW 14
#define WHITE 7
#define LRED 12
#define GREY 8

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursor;

int maxX = SCHEIGHT;
int maxY = SCWIDTH;
int midX = maxX/2;
int midY = maxY/2;
int shipX = 7;
int shipY = 6;

bool game = true;
int score = 0;

char ship[3][13] = {{' ', ' ', '_','\xC9', '\xCB', '\xCB', '\xB2', '\xCB', '\xBB', '_', '_','_',' '},
                   {' ', '}', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDF', '\xDF'},
                   {' ', ' ', ' ', '\xDF', '\xDF', '\xDF', '\xC1', '\xDF', '\xDF', ' ', ' ', ' ', ' '}
                  };

char lmap[18][7] = {{'\xCD','\xCD','\xCB','\xCD','\xBB',' '},
                   {' ',' ','\xC8','\xCB','\xBC',' ',' '},
                   {' ',' ',' ','\xBA',' ',' ',' '},
                   {'\xB0','\xB0','\xC9','\xBC',' ',' ',' '},
                   {'\xB0','\xB0','\xB0','\xB0',' ',' ',' '},
                   {'\xB1','\xB1','\xB1','\xB0','\xB0',' ',' '},
                   {'\xB1','\xB1','\xB1','\xB1','\xB0',' ',' '},
                   {'\xB2','\xB1','\xB1','\xB0','\xB0',' ',' '},
                   {'\xB2','\xB2','\xB1','\xB1','\xB1',' ',' '},
                   {'\xDB','\xB2','\xB1','\xB0',' ',' ',' '},
                   {'\xB2','\xB1','\xB1',' ',' ',' ',' '},
                   {'\xDB','\xB2','\xB0','\xB0',' ',' ',' '},
                   {'\xB2','\xB2','\xB2','\xB1','\xB1',' ',' '},
                   {'\xB2','\xB2','\xB2','\xB1','\xB0','\xB0',' '},
                   {'\xDB','\xB2','\xB2','\xB2','\xB1','\xB1',' '},
                   {'\xDB','\xB2','\xB2','\xB1','\xB1','\xB0',' '},
                   {'\xDB','\xDB','\xB2','\xB1','\xB0',' ',' '},
                   {'\xDB','\xB2','\xB2','\xB2','\xB0','\xB0','\xB0'},
                  };

char bmap[2][88] = {{'\xDB','\xDB','\xDB','\xB2','\xB2','\xB1','\xB1','\xB1','\xB0',
                    '\xB0','\xB0','\xB0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                    ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                    ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                    ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                    ' ',' ',' ',' ',' ','\xB0','\xB0','\xB0','\xB0','\xB1','\xB1',
                    '\xB1','\xB2','\xB2','\xB2','\xB2','\xDB','\xDB','\xDB'},
                    {'\xDB','\xDB','\xDB','\xDB','\xDB','\xB2','\xB2','\xB2','\xB2',
                    '\xB2','\xB2','\xB2','\xB1','\xB1','\xB1','\xB2','\xB2','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xB1','\xB1','\xB1','\xB1','\xB1','\xB1',
                    '\xB2','\xB2','\xB2','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB'}
                   };

int net[5];
int netX[5];
int netY[5];

int fish[6];
int fishX[6];
int fishY[6];

void textcol(int color){
    SetConsoleTextAttribute(console, color);
}

void gotoxy(int x, int y){
    cursor.X = x;
    cursor.Y = y;
    SetConsoleCursorPosition (console, cursor);
}

void setcursor(bool visible, DWORD size){
    if(size == 0) size = 20;

    CONSOLE_CURSOR_INFO lpcursor;
    lpcursor.bVisible = visible;
    lpcursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpcursor);
}

void drawborder(){
    system("cls");
    textcol(GREY);
    for(int i = 0; i < 90; i++){ // border atas
        gotoxy(0+i,0);
        cout << "\xDB";
    }
    for(int i = 0; i < 90; i++){ // border bawah
        gotoxy(i,26);
        cout << "\xDB";
    }
    for(int i = 0; i < 26; i++){ // border atas
        gotoxy(0,i);
        cout << "\xDB";
    }
    for(int i = 0; i < 26; i++){ // border bawah
        gotoxy(89,i);
        cout << "\xDB";
    }
    textcol(WHITE);
}

void drawmap(){
    textcol(YELLOW);
    for(int i = 0; i < 18; i++ ){
        gotoxy(1,6+i);
        for(int j = 0; j < 7; j++){
            cout << lmap[i][j];
        }
        cout << endl;
    }

    for(int i = 0; i < 2; i++ ){
        gotoxy(1,24+i);
        for(int j = 0; j < 88; j++){
            cout << bmap[i][j];
        }
        cout << endl;
    }
    textcol(WHITE);
}

void instructions(){
	// system("cls");
	
	// textcolor(BLUE);
	// gotoxy(midX - 3, midY - 4);
	// cout << "Objective";
	// gotoxy(midX - 3, midY - 3);
	// cout << "---------";
	
	// gotoxy(midX - 17, midY-2);
	// textcol(WHITE);
	// cout << "Cacth the ";
	// textcol(RED);
	// cout << "fish";
	
	// gotoxy(midX - 10, midY - 2);
	// textcol(WHite);
	// cout << " as many as possible";
	// 	gotoxy(midX - 17, midY-1);
	// cout << "Before time runs out";
	// textcolor(BLUE);
	// gotoxy(midX - 10, midY - 2);
	
	
	// cout << "movement:";
	// gotoxy(midx-10, midy-1);
	// cout << "W = up";
	// gotoxy(midx-10, midy);
	// cout << "S = down";
	// gotoxy(midx-10, midy+1);
	// cout << "SPACE = Shoot";
	// textcolor(YELLOW);
	// gotoxy(midx-10, midy+4);
	// cout << "Press any key to go back to menu";
	// textcolor(WHITE);
	// getche();
}

// Ship section
void drawship(){
    for(int i = 0; i <3; i++){
        gotoxy(shipX,shipY + i);
        if(i == 2){
            textcol(LRED);
        }
        else{
            textcol(WHITE);
        }
        for(int j = 0; j < 13;j++){
            cout << ship[i][j];        
        }
        cout << endl;
    }
    textcol(WHITE);
}

void removeship(){
    for(int i = 0; i <3; i++){
        gotoxy(shipX,shipY + i);
        cout << "             "  << endl;
    }
}

// Net section
void drawnet(int loc){
    if(net[loc] == true){
        textcol(GREY);
        gotoxy(netX[loc], netY[loc]);
        cout << "\xB2";
        textcol(WHITE);
    }
}

void removenet(int loc){
    gotoxy(netX[loc], netY[loc]);
    cout << " ";
}

void gennet(int loc){
    netX[loc] = shipX + 6;
    netY[loc] = shipY + 2;
}

void resetnet(int loc){
    removenet(loc);
    net[loc];
}

// Fish section
void genfish(int loc){

    // memberi batasan spawn ikan pada absis [10,22]
    fishY[loc] = 10 + rand() % (19);
}

void drawfish(int loc){
    if(fish[loc] == true){
        textcol(BLUE);
        gotoxy(fishX[loc], fishY[loc]);
        cout << "}Q";
        textcol(WHITE);
    }
}

void removefish(int loc){
    gotoxy(fishX[loc], fishY[loc]);
    cout << "  ";
}

void resetfish(int loc){
    removefish(loc);
    fishX[loc] = 7;
    genfish(loc);
}



int main(){
    drawborder();
    drawmap();
    getch();
}