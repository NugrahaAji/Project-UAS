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
#define LBLUE 9

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
double score = 0;
double timer;

char ship[3][14] = {{' ', ' ', '_','\xC9', '\xCB', '\xCB', '\xB2', '\xCB', '\xBB', '_', '_','_',' ',' '},
                   {' ', '}', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDF', '\xDF', ' '},
                   {' ', ' ', ' ', '\xDF', '\xDF', '\xDF', '\xC1', '\xDF', '\xDF', ' ', ' ', ' ', ' ',' '}
                  };

char lmap[18][7] = {{'\xCD','\xCD','\xCB','\xCD','\xBB',' '},
                   {' ',' ','\xC8','\xCB','\xBC',' ',' '},
                   {' ',' ',' ','\xBA',' ',' ',' '},
                   {'\xB2','\xB2','\xC9','\xBC',' ',' ',' '},
                   {'\xB2','\xB2','\xB2','\xB2',' ',' ',' '},
                   {'\xB2','\xB2','\xB2','\xB2','\xB2',' ',' '},
                   {'\xB2','\xB2','\xB2','\xB2','\xB2',' ',' '},
                   {'\xDB','\xB2','\xB2','\xB2','\xB2',' ',' '},
                   {'\xDB','\xDB','\xB2','\xB2','\xB2',' ',' '},
                   {'\xDB','\xDB','\xB2','\xB2',' ',' ',' '},
                   {'\xDB','\xB2','\xB2',' ',' ',' ',' '},
                   {'\xDB','\xDB','\xB2','\xB2',' ',' ',' '},
                   {'\xDB','\xDB','\xDB','\xB2','\xB2',' ',' '},
                   {'\xDB','\xDB','\xDB','\xB2','\xB2','\xB2',' '},
                   {'\xDB','\xDB','\xDB','\xDB','\xB2','\xB2',' '},
                   {'\xDB','\xDB','\xDB','\xB2','\xB2','\xB2',' '},
                   {'\xDB','\xDB','\xDB','\xB2','\xB2',' ',' '},
                   {'\xDB','\xDB','\xDB','\xDB','\xB2','\xB2','\xB2'},
                  };

char bmap[2][88] = {{'\xDB','\xDB','\xDB','\xDB','\xDB','\xB2','\xB2','\xB2','\xB2',
                    '\xB2','\xB2','\xB2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                    ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                    ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                    ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                    ' ',' ',' ',' ',' ','\xB2','\xB2','\xB2','\xB2','\xB2','\xB2',
                    '\xB2','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB'},
                    {'\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xB2','\xB2','\xB2','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                    '\xDB','\xDB','\xDB','\xB2','\xB2','\xB2','\xB2','\xB2','\xB2',
                    '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB'}
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
    textcol(WHITE);
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
	system("cls");
	
	textcol(BLUE);
	gotoxy(midX - 3, midY - 4);
	cout << "Objective";
	gotoxy(midX - 3, midY - 3);
	cout << "---------";
	
	gotoxy(midX - 14, midY-2);
	textcol(WHITE);
	cout << "Cacth the ";
	textcol(LRED);
	cout << "fish";
	
	gotoxy(midX - 7, midY - 2);
	textcol(WHITE);
	cout << " as many as possible";
		gotoxy(midX - 14, midY-1);
	cout << "Before timerr runs out";


	textcol(LBLUE);
	gotoxy(midX - 10, 15);
	cout << "movement:";
	gotoxy(midX-10, 16);
	cout << "W = up";
	gotoxy(midX-10, 17);
	cout << "S = down";
	gotoxy(midX-10, 18);
	cout << "SPACE = Shoot";
	textcol(YELLOW);
	gotoxy(midX-10, 19);
	cout << "Press any key to go back to menu";
	textcol(WHITE);
	getche();
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
        for(int j = 0; j < 14;j++){
            cout << ship[i][j];        
        }
        cout << endl;
    }
    textcol(WHITE);
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
    net[loc] = false;
}

// Fish section
void genfish(int loc){

    // memberi batasan spawn ikan pada absis [10,22]
    fishY[loc] = 10 + rand() % (14);
}

void drawfish(int loc){
    if(fish[loc] == true){
        textcol(LBLUE);
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
    gotoxy(maxX - 21, 3);
    cout << "Fish Caught : " << score;
}

void timerr(){
    gotoxy(maxX - 15, 2);
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
	textcol(LBLUE);
    gotoxy(22,3); 
    cout << " _______ _                                  _ ";
    gotoxy(22,4);
    cout << "(_______|_)                             _  | |";
    textcol(WHITE);
    gotoxy(22,5); 
    cout << "    _    _ ____  _____     ___  _   _ _| |_| |";
    gotoxy(22,6);
    cout << "   | |  | |    \\| ___ |   / _ \\| | | (_   _)_|";
    gotoxy(22,7); 
    cout << "   | |  | | | | | ____|  | |_| | |_| | | |_ _ ";
    gotoxy(22,8);
    cout << "   |_|  |_|_|_|_|_____)   \\___/|____/   \\__)_|";

	textcol(YELLOW);
	gotoxy(midX - 10, midY - 10);
	cout << "press anything to continue";

	getch();
	textcol(GREY);
	gotoxy(37, 10);
	cout << "Fish Caught : " << score;
	textcol(YELLOW);
    
	getch();
	gotoxy(midX-10, 15);
	cout << "select an option:            ";
	textcol(YELLOW);
	gotoxy(midX-10, 16);
	cout << "1. Menu";
	gotoxy(midX-10, 17);
	cout << "2. Quit";

	textcol(WHITE);
	bool loop = true;
	textcol(0);
	while(loop == true){
	char op3 = getche();

		if(op3 == '1'){
			loop = false;
		}
		else if (op3 == '2'){
			textcol(WHITE);
			exit(0);
		}
		else{
			loop = true;
		}
		
	}
	
}

void sailing(){
        score = 0;

        for(int i = 0; i < 20; i++){
            if(i == 0 || i == 3 || i == 11 || i == 17){
                fish[i] = true;
            }
            else{
                fish[i] = false;
            }
        }
        for(int i = 0; i < 20; i++){
            net[1] = false;
        }
    
        for(int i = 0; i < 8; i++){
            fishX[i] = 7;
        }
        for(int i = 0; i < 8; i++){
            netX[i] = shipX + 6;
        }
        for(int i = 0; i < 8; i++){
            netY[i] = shipY + 2;
        }
    
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
                    if (shipX < maxX - 15) {
                        //removeship();
                        shipX++;
                    }
                } else if (motion == 'a' || motion == 'A' || motion == LEFT) {
                    if (shipX > 6) {
                        //removeship();
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

        if(fishX[1] == 14){
            if(fish[2] == false){
                fish[2] = true;
            }
        }

        if(fishX[2] == 17){
            if(fish[3] == false){
                fish[3] = true;
            }
        }

        if(fishX[3] == 46){
            if(fish[0] == false){
                fish[0] = true;
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
        timer = timer - 0.10;
        timerr();
        drawship();
        if(timer <= 0){
            finish();
            timer = 60;
            return;
        }
            
    }
}
 
int main(){
    setcursor(0,0);
	
	do{
		system("cls");
		textcol(LBLUE);
        gotoxy(11, 3);
        cout << " _______ _      _                        _     ______ _     _       ";
        gotoxy(11, 4);
        cout << "(_______|_)    | |                      | |   / _____) |   (_)      ";
        gotoxy(11, 5);
        cout << " _____   _  ___| |__     _____ ____   __| |  ( (____ | |__  _ ____  ";
        gotoxy(11, 6);
        cout << "|  ___) | |/___)  _ \\   (____ |  _ \\ / _  |   \\____ \\|  _ \\| |  _ \\";
        textcol(WHITE);
        gotoxy(11, 7);
        cout << "| |     | |___ | | | |  / ___ | | | ( (_| |   _____) ) | | | | |_| |";
        gotoxy(11, 8);
        cout << "|_|     |_(___/|_| |_|  \\_____|_| |_|\\____|  (______/|_| |_|_|  __/ ";
        gotoxy(11, 9);
        cout << "                                                             |_|    ";
        

        textcol(YELLOW);
        gotoxy(39, 12);
        cout << "1.Go to Sail";

        gotoxy(38, 13);
        cout << "2.Instructions";

        gotoxy(42, 14);
        cout << "3.Exit";
		textcol(WHITE);
		char option = getche();

        if(option == '1'){
            sailing();
        }
        else if(option == '2'){
            instructions();
        }
        else if(option == '3'){
            game = false;
        }

	} while(game);
	
	
	
	return 0;
}
