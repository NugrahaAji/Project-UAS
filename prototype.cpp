#include<conio.h>
#include<windows.h>
#include<iostream>
#include<ncurses/ncurses.h>
using namespace std;

#define WIN_WIDTH 70

#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define PURPLE 5
#define DARKYELLOW 6
#define WHITE 7
#define GREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define PINK 13
#define YELLOW 14
#define LIGHTWHITE 15

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursor;

int maxX = 90, maxY = 26; // Ukuran layar game
bool game = true;
int score = 0; // score awal
int shipX = 5, shipY = 6; // Posisi spawn kapal
int netX[8];
int netY[8];
int fishX[6];
int fishY[6];
int fish[6];
char ship[3][13] = {{' ', ' ', '_','\xC9', '\xCB', '\xCB', '\xB2', '\xCB', '\xBB', '_', '_','_',' '},
                    {' ', '}', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDF', '\xDF'},
                    {' ', ' ', ' ', '\xDF', '\xDF', '\xDF', '\xC1', '\xDF', '\xDF', ' ', ' ', ' ', ' '}};
char lborder[18][7] = {{'\xCD','\xCD','\xCB','\xCD','\xBB',' '},
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
char bborder[2][90] = {{'\xDB','\xDB','\xDB','\xD2','\xB2','\xB1','\xB1','\xB1','\xB0',
                        '\xB0','\xB0','\xB0',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\xB0','\xB0','\xB0','\xB0',
                        '\xB1','\xB1','\xB1','\xB2','\xB2','\xB2','\xB2','\xDB','\xDB','\xDB'},
                        {'\xDB','\xDB','\xDB','\xDB','\xDB','\xB2','\xB2','\xB2','\xB2','\xB2','\xB2','\xB2','\xB1','\xB1','\xB1','\xB2','\xB2',
                        '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                        '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                        '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB',
                        '\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB','\xB1','\xB1','\xB1','\xB1','\xB1',
                        '\xB1','\xB2','\xB2','\xB2','\xDB','\xDB','\xDB','\xDB','\xDB','\xDB'},};

void textcolor(int color){
    SetConsoleTextAttribute(console, color);// Mendeklarasikan fungsi mengubah warna text
}

void gotoxy(int x, int y){
    //meletakan cursor pada kolom x dan baris y
    cursor.X = x; 
    cursor.Y = y;
    SetConsoleCursorPosition(console,cursor);
}

void setcursor(bool visible,DWORD size){
    if(size == 0)size = 20;

    CONSOLE_CURSOR_INFO lpcursor;
    lpcursor.bVisible = visible;
    lpcursor.dwSize = size; 
    SetConsoleCursorInfo(console, &lpcursor);
}

void drawborder(){
    system("cls");
    textcolor(YELLOW);
    gotoxy(0,7);
    for(int i = 0; i <18; i++){
        for(int j = 0; j < 7;j++){
            cout << lborder[i][j];
        }
        cout << endl;
    }

    gotoxy(0,24);
    for(int i = 0; i <2; i++){
        for(int j = 0; j < 90;j++){
            cout << bborder[i][j];
        }
        cout << endl;
    }
    textcolor(WHITE);
}  

void drawship(){
    for(int i = 0; i <3; i++){
        gotoxy(shipX,shipY + i);
        if(i == 2){
            textcolor(RED);
        }
        else{
            textcolor(WHITE);
        }
        for(int j = 0; j < 13;j++){
            cout << ship[i][j];        
        }
        cout << endl;
    }
    textcolor(WHITE);
}

void net(){
    textcolor(GREY);
    gotoxy(shipX + 5, shipY + 3);
    cout<<'\xB2';
    textcolor(WHITE);
}   


void genfish(int fy){
    // membuat ikan spawn pada baris 8 < y < 24, karena tinggi border
    // atas ada  di y = 8 dan tinggi maks 24
    fishY[fy] = 8 + rand()%(17);
}

void drawfish(int fy){
    // print ikan pada pada kolom fishX[fx] dan baris fishY[fy]
    if(fish[fy]){
        textcolor(LIGHTGREEN);
        gotoxy(fishX[fy], fish[fy]);
        cout<<"}\xE5";
        textcolor(WHITE);
    }
}

void removefish(int fy){
    // menghapus print ikan saat untuk membuat animasi berjalan
    if(fish[fy]){
        gotoxy(fishX[fy], fish[fy]);
        cout<<"  ";
    }
}

void drawship(){
    // menghapus print kapal untuk membuat animasi berjalan
    for(int i = 0; i <3; i++){
        gotoxy(shipX,shipY + i);
        for(int j = 0; j < 13;j++){
            cout << " ";        
        }
        cout << endl;
    }
}

void resetfish(int fy){
    removefish(fy);
    fishY[fy] = 1;
    genfish(fy);
}

// void collision(int fy){
//     if (netY[8] - fishY[fy] >= 0 && netY[8] - fishY[fy] < 2)
// 	{
// 		if (netX[8] - fishY[fy] >= 0 && netX[8] - fishX[fy] < 2)
// 		{
// 			return true;    
// 		}
// 	}
// 	return false;
// }

void algorithm(){
    shipX = 5;
    shipY = 6;
    score = 0;

    fish[0] = true;
    fish[1] = false;
    fish[2] = true;
    fish[3] = true;
    fish[4] = false;
    fish[5] = false;

    system ("cls");
    drawborder();

    genfish(0);
    genfish(1);
    genfish(2);
    genfish(3);
    genfish(4);
    genfish(5);

    //gotoxy();
    textcolor(LIGHTBLUE);
    cout << "Press ";
    textcolor(YELLOW);
    cout << "SPACE ";
    textcolor(LIGHTBLUE);
    cout << "to start";
}

void done(){

}

int main(){
    setcursor(false,0);
    drawborder();
       
    }
 