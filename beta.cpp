#include<conio.h>
#include<windows.h>
#include<iostream>
#include<time.h>
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
#define SPACE 32

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursor;

int maxX = SCWIDTH;
int maxY = SCHEIGHT;
int midX = maxX/2;
int midY = maxY/2;
int shipX = 6;
int shipY = 5;

bool game = true;
int score = 0;
int timer;

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

int net[8];
int netX[8];
int netY[8];

int fish[20];
int fishX[20];
int fishY[20];

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
	// cout << "Before timerr runs out";
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
        cout << "V";
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
    net[loc] = false;
}

// Fish section
void genfish(int loc){

    // memberi batasan spawn ikan pada absis [10,22]
    fishY[loc] = 10 + rand() % (14);
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
    fishX[loc] = 6;
    genfish(loc);
}

void fishcaught(){
    gotoxy(maxX - 20, 3);
    cout << "Fish Caught : " << score;
}

void timerrr(){
    gotoxy(maxX - 13, 2);
    cout << "timer : " << timer <<"s";
}

// collision section
bool fishhit(int loc){
    for(int i = 0; i < 8; i++){
        if(fishX[loc] - netX[i] >= 0 && fishX[loc] - netX[loc] <= 1){
            if(fishY[loc] - netY[loc] >= 0 && fishY[loc] - netY[loc] <= 1){
                resetnet(i);
                resetfish(loc);
                return true;
            }
        }
    }
    return false;
}

void finish(){
    system("cls");
    drawborder();

}
void sailing(){
    score = 0;
    fish[0] = true;
    fish[1] = false;
    fish[2] = true;
    fish[3] = false;
    fish[4] = false;
    fish[5] = true;
    fish[6] = false;
    fish[7] = false;
    fish[8] = false;
    fish[9] = false;
    fish[10] = false;
    fish[11] = false;
    fish[12] = false;
    fish[13] = false;
    fish[14] = false;
    fish[15] = false;
    fish[16] = false;
    fish[17] = false;
    fish[18] = false;
    fish[19] = false;

    net[0] = false;
    net[1] = false;
    net[2] = false;
    net[3] = false;
    net[4] = false;
    net[5] = false;
    net[6] = false;
    net[7] = false;

    fishX[0] = fishX[1] = fishX[2] = fishX[3] = fishX[4] = fishX[5] = fishX[6] = fishX[7] = 7;
    netX[0] = netX[1] = netX[2] = netX[3] = netX[4] = netX[5] = netX[6] = netX[7] = shipX + 6;
    netY[0] = netY[1] = netY[2] = netY[3] = netY[4] = netY[5] = netY[6] = netY[7] = shipY + 2;

    drawborder();
    drawmap();
    fishcaught();
    drawship();
    for (int i = 0; i < 8; i++) {
        genfish(i);
        gennet(i);
    }

    timer = 60;

    while (true) {
        if (kbhit()) {
            char motion = getch();
            if (motion == 'd' || motion == 'D' || motion == RIGHT) {
                if (shipX < maxX - 14) {
                    removeship();
                    shipX++;
                }
            } else if (motion == 'a' || motion == 'A' || motion == LEFT) {
                if (shipX > 6) {
                    removeship();
                    shipX--;
                }
            } else if (motion == SPACE) {
                for (int i = 0; i < 8; i++) {
                    if (net[i] == false) {
                        net[i] = true;
                        netX[i] = shipX + 6;
                        netY[i] = shipY + 2;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < 8; i++) {
            drawnet(i);
        }

        for (int i = 0; i < 8; i++) {
            drawfish(i);
        }
        for (int i = 0; i < 8; i++){
            if (fishhit(i) == true){
                score++;
                fishcaught();
            }
        }   

        Sleep(80);

        for(int i = 0; i < 8; i++){
            removenet(i);
        }
        
        for(int i = 0; i < 8; i++){
            removefish(i);
        }

        if(fishX[0] == 32){
            if(fish[1] == false){
                fish[1] = true;
            }
        }

        for(int i = 0; i < 8; i++){
            if(fish[i] == true){
                fishX[i]++;
            }
        }

        for(int i = 0; i < 8; i++){
            if(net[i] == true && netY[i] < maxY - 1){
                netY[i]++;
            }
        }

        for(int i = 0; i < 8; i++ ){
            if(netY[i] == maxY - 3 ){
                resetnet(i);
            }
        }
        for(int i=0; i<8; i++){
			if(fishX[i] == maxX - 3){
				resetfish(i);
			}
		}
    drawship();
    }

}   

int main(){
    setcursor(0,0);
    srand((unsigned)time(NULL));
    sailing();
}
