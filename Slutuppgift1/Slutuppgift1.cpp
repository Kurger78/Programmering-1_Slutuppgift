#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <conio.h>
using namespace std;

struct settings {
    int gridsize;
    bool multiMode;
    bool devMode;
    int turn;
};

string red(string inp) {
    return "\033[31m" + inp + "\033[0m";
}
string blue(string inp) {
    return "\033[34m" + inp + "\033[0m";
}
string erase() {
    return "\033[2K";
}
string moveCursor(int x) {
    string text = "\033[";
    if (x > 0) {
        text += x;
        text += "C";
    }
    if (x < 0) {
        text += -x;
        text += "D";
    }
    return text;

}

bool hBingo(string dG[], int gridsize) {
    bool redBingo, blueBingo;
    for (int i = 0; i < gridsize; i++) {
        redBingo = true;
        blueBingo = true;
        for (int j = 0; j < gridsize; j++) {
            if (dG[i * gridsize + j] != red("#"))
                redBingo = false;
            if (j == gridsize - 1 && redBingo)
                return true;
            if (dG[i * gridsize + j] != blue("#"))
                blueBingo = false;
            if (j == gridsize - 1 && blueBingo)
                return true;
        }
    }
    return false;
}
bool vBingo(string dG[], int gridsize) {
    bool redBingo, blueBingo;
    for (int i = 0; i < gridsize; i++) {
        redBingo = true;
        blueBingo = true;
        for (int j = 0; j < gridsize; j++) {
            if (dG[j * gridsize + i] != red("#"))
                redBingo = false;
            if (j == gridsize - 1 && redBingo)
                return true;
            if (dG[j * gridsize + i] != blue("#"))
                blueBingo = false;
            if (j == gridsize - 1 && blueBingo)
                return true;
        }
    }
    return false;
}
bool LtoRDBingo(string dG[], int gridsize) {
    bool redBingo = true, blueBingo = true;
    for (int i = 0; i < gridsize; i++) {
        if (dG[i * gridsize + i] != red("#"))
            redBingo = false;
        if (dG[i * gridsize + i] != blue("#"))
            blueBingo = false;
    }
    if (blueBingo || redBingo)
        return true;
}
bool RtoLDBingo(string dG[], int gridsize) {
    bool redBingo = true, blueBingo = true;
    for (int i = 0; i < gridsize; i++) {
        if (dG[i * gridsize + gridsize - (i + 1)] != red("#"))
            redBingo = false;
        if (dG[i * gridsize + gridsize - (i + 1)] != blue("#"))
            blueBingo = false;
    }
    if (blueBingo || redBingo)
        return true;
}

bool bingocheck(string dG[], int gridsize) {
    if (hBingo(dG, gridsize))
        return true;
    if (vBingo(dG, gridsize))
        return true;
    if (LtoRDBingo(dG, gridsize))
        return true;
    if (RtoLDBingo(dG, gridsize))
        return true;
    return false;
}

void generateGrid(int nG[], string dG[], int gridsize) {
    for (int i = 0; i < gridsize; i++) {
        for (int j = 0; j < gridsize; j++) {
            nG[i * gridsize + j] = 0;
            if (i * gridsize + j > 25)
                dG[i * gridsize + j] = 'a' - 26 + (i * gridsize + j);
            else
                dG[i * gridsize + j] = 'A' + (i * gridsize + j);
        }
    }

    srand(time(0));
    bool notUsed;
    int numsGenerated = 0;

    while (numsGenerated < (gridsize * gridsize)) {
        notUsed = true;
        int num = rand() % (gridsize * gridsize) + 1;

        for (int i = 0; i < gridsize; i++) {
            for (int j = 0; j < gridsize; j++) {
                if (num == nG[i * gridsize + j])
                    notUsed = false;
            }
        }
        if (notUsed) {
            nG[numsGenerated] = num;
            numsGenerated++;
        }
    }
}

void printGrid(int nG[], string dG[], int gridsize, vector<int> usedNumbers, settings player) {
    cout << "\033[H";
    erase();
    cout << "* ----- *\n\033[2K | Bingo | \n\033[2K * ------ *\n";

    for (int i = 0; i < gridsize; i++) {
        for (int j = 0; j < gridsize; j++) {
            cout << dG[i * gridsize + j] << "  ";
        }
        cout << "   ";
        for (int k = 1; k <= gridsize; k++) {
            cout << i * gridsize + k << " ";
            if (i * gridsize + k < 10)
                cout << " ";
            for (int l = 0; l < usedNumbers.size(); l++) {
                if (usedNumbers[l] == i * gridsize + k) {
                    cout << "\033[3D";
                    cout << red("X") << "  ";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
    if (player.devMode) {
        for (int i = 0; i < gridsize; i++) {
            for (int j = 0; j < gridsize; j++) {
                cout << nG[i * gridsize + j] << " ";
                if (nG[i * gridsize + j] < 10)
                    cout << " ";
            }
            cout << endl;
        }
    }
}

void userGuess(int nG[], string dG[], int gridsize, settings player) {
    int guess;
    int amountOfGuesses = 0;
    vector<int> usedNumbers;
    bool notUsed;
    
    printGrid(nG, dG, gridsize, usedNumbers, player);

    for (;;) {
        notUsed = true;
        printGrid(nG, dG, gridsize, usedNumbers, player);
        if (player.multiMode) {
            if ((player.turn + 1) % 2 == 1)
                cout << red("X") << " ";
            else
                cout << blue("X") << " ";
        }
        cout << "Guess a number between 1 - " << gridsize * gridsize << "\n" << erase();
        cin >> guess;
        if (guess == 0)
            return;
        else if (0 < guess && guess < gridsize * gridsize + 1) {
            for (int i = 0; i < usedNumbers.size(); i++) {
                if (guess == usedNumbers.at(i))
                    notUsed = false;
            }
            if (notUsed) {
                player.turn++;
                usedNumbers.push_back(guess);

                for (int i = 0; i < gridsize; i++) {
                    for (int j = 0; j < gridsize; j++) {

                        if (guess == nG[i * gridsize + j]) {
                            if (player.multiMode) {
                                if (player.turn % 2 == 1)
                                    dG[i * gridsize + j] = red("#");
                                else
                                    dG[i * gridsize + j] = blue("#");
                            }
                            else
                                dG[i * gridsize + j] = red("#");
                            printGrid(nG, dG, gridsize, usedNumbers, player);
                        }
                    }
                }
            }
            else {
                printGrid(nG, dG, gridsize, usedNumbers, player);
            }
        }
        else {
            printGrid(nG, dG, gridsize, usedNumbers, player);
        }
        if (bingocheck(dG, gridsize)) {
            if (player.multiMode) {
                if (player.turn % 2 == 1)
                    cout << erase() << "Red won! It took " << player.turn << " guesses!";
                else
                    cout << erase() << "Blue won! It took " << player.turn << " guesses!";
            }
            else
                cout << erase() << "You got BINGO in " << player.turn << " guesses!";
            return;
        }
    }
}
void updmenu(settings& player) {
    char inp = _getch();

}

void menu(settings& player) {

    char inp = 1;
    cout << erase() << "Welcome to Bingo!\n";
    while (inp != 0) {
        cout << "[1]Standard  [2]DevMode\n" << erase();
        cin >> inp;
        switch (inp) {
        case '1':
            player.devMode = false;
            break;
        case '2':
            player.devMode = true;
            break;
        }
        cout << "\033[H\n" << erase() << "[1]Singleplayer  [2]Multiplayer\n" << erase();
        cin >> inp;
        switch (inp) {
        case '1':
            player.multiMode = false;
            break;
        case '2':
            player.multiMode = true;
            break;
        }

        cout << "\033[H\n" << erase() << "[1]3x3  [2]4x4  [3]5x5  [4]6x6  [5]7x7  [0]Exit\n" << erase();
        cin >> inp;
        switch (inp) {
        case '0':
            player.gridsize = 0;
            break;
        case '1':
            player.gridsize = 3;
            break;
        case'2':
            player.gridsize = 4;
            break;
        case'3':
            player.gridsize = 5;
            break;
        case'4':
            player.gridsize = 6;
            break;
        case'5':
            player.gridsize = 7;
            break;
        default:
            //system("CLS");
            cout << "\033[H";
            cout << "Invalid input! Try again\n";
            continue;
        }
        player.turn = 0;
        return;
    }
}



int main()
{
    settings player; 
    menu(player);
    int gridsize = player.gridsize;
    bool multimode = player.multiMode;
    if (gridsize == 0)
        return 0;

    int* nG = new int[gridsize * gridsize];

    string* dG = new string[gridsize * gridsize];

    generateGrid(nG, dG, gridsize);
    userGuess(nG, dG, gridsize, player);

    delete[] nG;
    delete[] dG;

    return 0;
}