void gameOver() {
    system("cls");
    textcolor(RED);
    gotoxy(maxX / 2 - 5, maxY / 2);
    cout << "Game Over!";
    textcolor(YELLOW);
    gotoxy(maxX / 2 - 8, maxY / 2 + 2);
    cout << "Your score: " << score;
    textcolor(LIGHTBLUE);
    gotoxy(maxX / 2 - 10, maxY / 2 + 4);
    cout << "Press ENTER to play again";
    textcolor(WHITE);

    while (_getch() != 13) 
        ;

    algorithm(); 
}

int main() {
    setcursor(false, 0);
    algorithm(); 

    while (game) {
  
        if (shipY <= 8) {
            game = false;
            gameOver();
        }
    }

    return 0;
}

