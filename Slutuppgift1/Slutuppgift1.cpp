#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <fstream>
#include <cmath>
using namespace std;

#define ENTER 13
#define ESC 27
#define BACK 8
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80


struct settings {
    bool devMode;
    bool multiMode;
    int gridsize;
    int turn;
};


//Färg Funktioner
string bold(string inp) {
    return "\033[1m" + inp + "\033[0m";
}
string whiteback(string inp) {
    return "\033[100m" + inp + "\033[0m";
}
string white(string inp) {
    return "\033[97m" + inp + "\033[0m";
}
string red(string inp) {
    return "\033[31m" + inp + "\033[0m";
}
string blue(string inp) {
    return "\033[34m" + inp + "\033[0m";
}
string green(string inp) {
    return "\033[92m" + inp + "\033[0m";
}
string darkgray(string inp) {
    return "\033[90m" + inp + "\033[0m";
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

//Filhantering
void readSettings(settings& s) {
    int text;
    ifstream readFile;
    readFile.open("Settings.txt");
    readFile >> text;
    s.devMode = text;
    readFile >> text;
    s.multiMode = text;
    readFile >> text;
    s.gridsize = text;
    readFile.close();
}
void writeSettings(settings s) {
    int text;
    ofstream writeFile("Settings.txt");
    text = s.devMode;
    writeFile << text << "\n";
    text = s.multiMode;
    writeFile << text << "\n";
    text = s.gridsize;
    writeFile << text << "\n";
    writeFile.close();
}

//Bingo checkar
bool hBingo(vector<string> dG, settings s) {
    bool redBingo, blueBingo;
    for (int y = 0; y < s.gridsize; y++) {
        redBingo = true;
        blueBingo = true;
        for (int x = 0; x < s.gridsize; x++) {
            if (dG[y * s.gridsize + x] != red("#"))
                redBingo = false;
            if (dG[y * s.gridsize + x] != blue("#"))
                blueBingo = false;

            if (x + 1 == s.gridsize && redBingo)
                return true;
            if (x + 1 == s.gridsize && blueBingo)
                return true;
        }
    }
    return false;
}
bool vBingo(vector<string> dG, settings s) {
    bool redBingo, blueBingo;
    for (int x = 0; x < s.gridsize; x++) {
        redBingo = true;
        blueBingo = true;
        for (int y = 0; y < s.gridsize; y++) {
            if (dG[y * s.gridsize + x] != red("#"))
                redBingo = false;
            if (dG[y * s.gridsize + x] != blue("#"))
                blueBingo = false;

            if (y + 1 == s.gridsize && redBingo)
                return true;
            if (y + 1 == s.gridsize && blueBingo)
                return true;
        }
    }
    return false;
}
bool LtoRDBingo(vector<string> dG, settings s) {
    bool redBingo = true, blueBingo = true;
    for (int i = 0; i < s.gridsize; i++) {
        if (dG[i * s.gridsize + i] != red("#"))
            redBingo = false;
        if (dG[i * s.gridsize + i] != blue("#"))
            blueBingo = false;
    }
    if (blueBingo || redBingo)
        return true;
    else
        return false;
}
bool RtoLDBingo(vector<string> dG, settings s) {
    bool redBingo = true, blueBingo = true;
    for (int i = 0; i < s.gridsize; i++) {
        if (dG[i * s.gridsize + s.gridsize - (i + 1)] != red("#"))
            redBingo = false;
        if (dG[i * s.gridsize + s.gridsize - (i + 1)] != blue("#"))
            blueBingo = false;
    }
    if (blueBingo || redBingo)
        return true;
    else
        return false;
}

bool bingocheck(vector<string> dG, settings s) {
    if (hBingo(dG, s))
        return true;
    if (vBingo(dG, s))
        return true;
    if (LtoRDBingo(dG, s))
        return true;
    if (RtoLDBingo(dG, s))
        return true;
    return false;
}

//Spelet
void generateGrid(vector<int>& nG, vector<string>& dG, settings s) {
    string text;
    for (int i = 0; i < s.gridsize * s.gridsize; i++) {
        i = i;
        if (i < 26)
            text = 'A' + i;
        else
            text = 'a' + i - 26;
        dG.push_back(text);
    }

    srand(time(0));
    bool used;
    int numsGenerated = 0;

    while (nG.size() < (s.gridsize * s.gridsize)) {
        used = false;
        int num = rand() % (s.gridsize * s.gridsize) + 1;

        for (int i = 0; i < nG.size(); i++) {
            if (num == nG[i])
                used = true;
        }
        if (!used) {
            nG.push_back(num);
        }
    }
}

void printGrid(vector<int> nG, vector<string> dG, settings s, vector<int> usedNumbers) {
    int pos;
    cout << "\033[H";
    erase();
    cout << "= BINGO ==";
    if (s.multiMode) {
        if (s.turn % 2 == 0)
            cout << red(" Red's  Turn") << " =";
        else
            cout << blue(" Blue's Turn") << " =";
    }
    else {
        cout << "==============";
    }

    for (int i = 0; i < s.gridsize-3; i++) {
        cout << "======";
    }
    cout << "\n";


    for (int y = 0; y < s.gridsize; y++) {
        cout << "| ";
        for (int x = 0; x < s.gridsize; x++) {
            cout << dG[y * s.gridsize + x] << "  ";
        }
        cout << "|  ";

        for (int x = 1; x <= s.gridsize; x++) {
            bool used = false;
            for (int l = 0; l < usedNumbers.size(); l++) {
                if (usedNumbers[l] == y * s.gridsize + x)
                    used = true;
            }
            if (used)
                cout << red("X") << "  ";
            else {
                cout << y * s.gridsize + x << " ";
                if (y * s.gridsize + x < 10)
                    cout << " ";
            }
        }
        cout << "|";
        cout << endl;
    }

    cout << "========================";
    for (int i = 0; i < s.gridsize - 3; i++) {
        cout << "======";
    }
    cout << endl;
    if (s.devMode) {
        for (int i = 0; i < s.gridsize * s.gridsize; i++) {
            cout << nG[i] << " ";
            if (nG[i] < 10)
                cout << " ";
            if (i % s.gridsize == s.gridsize - 1)
                cout << endl;
        }
    }
}
void revampuserGuess(vector<int> nG, vector<string> dG, settings s) {
    int guess;
    char inp;
    bool used;
    string number;
    vector<int> usedNumbers;

    system("CLS");
    for (;;) {
        number = {};
        guess = 0;
        used = false;
        printGrid(nG, dG, s, usedNumbers);
        if (s.multiMode) {
            if ((s.turn) % 2 == 1)
                cout << blue("X") << " ";
            else
                cout << red("X") << " ";
        }
        cout << "Guess a number between 1 - " << s.gridsize * s.gridsize << "   Back[esc]\n" << "\033[0J";
        for (;;) {
            inp = _getch();
            if ('0' <= inp && inp <= '9') {
                number += inp;
                cout << inp;
            }
            else if (inp == BACK && number.size() > 0) {
                number.pop_back();
                cout << "\033[1D \033[1D";
            }
            else if (inp == ENTER && number.size() > 0) {
                for (int i = 0; i < number.size(); i++) {
                    guess += (number[i] - 48) * pow(10, number.size() - i - 1);
                }
                break;
            }
            else if (inp == ESC)
                return;
        }

        for (int i = 0; i < usedNumbers.size(); i++) {
            if (guess == usedNumbers.at(i))
                used = true;
        }
        if (!used && 0 < guess && guess < s.gridsize * s.gridsize + 1) {
            s.turn++;
            usedNumbers.push_back(guess);

            for (int i = 0; i < s.gridsize * s.gridsize; i++) {
                if (guess == nG[i]) {
                    if (s.multiMode && s.turn % 2 == 0) {
                        dG[i] = blue("#");
                    }
                    else
                        dG[i] = red("#");
                    printGrid(nG, dG, s, usedNumbers);
                }
            }
        }
        else {
            printGrid(nG, dG, s, usedNumbers);
        }
        if (bingocheck(dG, s)) {
            if (s.multiMode) {
                if (s.turn % 2 == 1)
                    cout << erase() << "Red won! It took " << s.turn << " guesses!\n";
                else
                    cout << erase() << "Blue won! It took " << s.turn << " guesses!\n";
            }
            else
                cout << erase() << "You got BINGO in " << s.turn << " guesses!\n";
            cout << erase();
            cout << "Press any key to return...";
            char inp = _getch();
            system("CLS");
            return;
        }
        else if (s.turn == s.gridsize * s.gridsize) {
            cout << erase() << "You both lost!\n";
            cout << erase();
            cout << "Press any key to return...";
            char inp = _getch();
            system("CLS");
            return;
        }
    }
}
void userGuess(vector<int> nG, vector<string> dG, settings s) {
    int guess;
    vector<int> usedNumbers;
    bool used;
    
    system("CLS");
    for (;;) {
        used = false;
        printGrid(nG, dG, s, usedNumbers);
        if (s.multiMode) {
            if ((s.turn) % 2 == 1)
                cout << blue("X") << " ";
            else
                cout << red("X") << " ";
        }
        cout << "Guess a number between 1 - " << s.gridsize * s.gridsize << "\n" << erase();
        cin >> guess;

        for (int i = 0; i < usedNumbers.size(); i++) {
            if (guess == usedNumbers.at(i))
                used = true;
        }
        cout << guess;
        _getch();
        if (guess == 0) {
            system("CLS");
            return;
        }
        else if (!used && 0 < guess && guess < s.gridsize * s.gridsize + 1) {
            s.turn++;
            usedNumbers.push_back(guess);

            for (int i = 0; i < s.gridsize * s.gridsize; i++) {
                if (guess == nG[i]) {
                    if (s.multiMode && s.turn % 2 == 0) {
                        dG[i] = blue("#");
                    }
                    else
                        dG[i] = red("#");
                    printGrid(nG, dG, s, usedNumbers);
                }
            }
        }
        else {
            printGrid(nG, dG, s, usedNumbers);
        }
        if (bingocheck(dG, s)) {
            if (s.multiMode) {
                if (s.turn % 2 == 1)
                    cout << erase() << "Red won! It took " << s.turn << " guesses!\n";
                else
                    cout << erase() << "Blue won! It took " << s.turn << " guesses!\n";
            }
            else
                cout << erase() << "You got BINGO in " << s.turn << " guesses!\n";
            cout << erase();
            cout << "Press any key to return...";
            char inp = _getch();
            system("CLS");
            return;
        }
        else if (s.turn == s.gridsize * s.gridsize) {
            cout << erase() << "You both lost!\n";
            cout << erase();
            cout << "Press any key to return...";
            char inp = _getch();
            system("CLS");
            return;
        }
    }
}

//Meny Funktioner
void printmenu(vector <string> menuitems, int selected) {
    cout << "\033[H";
    for (int i = 0; i < menuitems.size(); i++) {
        if (i == selected)
            cout << "> " << bold(menuitems[i]) << "\n";
        else
            cout << erase() << " " << darkgray(menuitems[i]) << "\n";
    }
    return;
}
void printsettings(settings player, int x, int y) {
    //system("CLS");
    cout << "\033[H";
    cout << "  Developer Mode\n";
    if (y == 0) {
        cout << ">   ";
        if (x == 0) cout << bold(white("On"));
        else if (player.devMode) cout << green("On");
        else cout << darkgray("On");
        cout << darkgray("/");
        if (x == 1) cout << bold(white("Off"));
        else if (!(player.devMode)) cout << green("Off");
        else cout << darkgray("Off");
    }
    else {
        cout << "    ";
        if (player.devMode) cout << green("On") << darkgray("/Off");
        else cout << darkgray("On/") << green("Off");
    }

    cout << "\n\n  Multiplayer Mode\n";
    if (y == 1) {
        cout << ">   ";
        if (x == 0) cout << bold(white("On"));
        else if (player.multiMode) cout << green("On");
        else cout << darkgray("On");
        cout << darkgray("/");
        if (x == 1) cout << bold(white("Off"));
        else if (!(player.multiMode)) cout << green("Off");
        else cout << darkgray("Off");
    }
    else {
        cout << "    ";
        if (player.multiMode) cout << green("On") << darkgray("/Off");
        else cout << darkgray("On/") << green("Off");
    }

    cout << "\n  Gridsize\n";

    if (y == 2) {
        cout << bold(white(">   "));

        if (x == 0) cout << bold(white("3x3 "));
        else if (player.gridsize - 3 == 0) cout << green("3x3 ");
        else cout << darkgray("3x3 ");

        if (x == 1) cout << bold(white("4x4 "));
        else if (player.gridsize - 3 == 1) cout << green("4x4 ");
        else cout << darkgray("4x4 ");

        if (x == 2) cout << bold(white("5x5 "));
        else if (player.gridsize - 3 == 2) cout << green("5x5 ");
        else cout << darkgray("5x5 ");

        if (x == 3) cout << bold(white("6x6 "));
        else if (player.gridsize - 3 == 3) cout << green("6x6 ");
        else cout << darkgray("6x6 ");

        if (x == 4) cout << bold(white("7x7"));
        else if (player.gridsize - 3 == 4) cout << green("7x7 ");
        else cout << darkgray("7x7");
    }
    else {
        //cout << darkgray("    3x3 4x4 5x5 6x6 7x7");
        cout << "    ";
        if (player.gridsize - 3 == 0) cout << green("3x3 ");
        else cout << darkgray("3x3 ");

        if (player.gridsize - 3 == 1) cout << green("4x4 ");
        else cout << darkgray("4x4 ");

        if (player.gridsize - 3 == 2) cout << green("5x5 ");
        else cout << darkgray("5x5 ");

        if (player.gridsize - 3 == 3) cout << green("6x6 ");
        else cout << darkgray("6x6 ");

        if (player.gridsize - 3 == 4) cout << green("7x7 ");
        else cout << darkgray("7x7 ");
    }
    cout << "\n\n";
    if (y == 3) cout << bold(white(">  Back[esc]"));
    else cout << erase() << darkgray("  Back[esc]");
    return;

}
int multichoicemenu(vector <string> menuitems) {
    int selected = 0;
    int inp;
    for (;;) {
        printmenu(menuitems, selected);
        inp = _getch();
        if (inp == UP)
            selected -= 1;
        if (inp == DOWN)
            selected += 1;
        if (selected < 0)
            selected = 0;
        if (selected > (menuitems.size() - 1))
            selected = (menuitems.size() - 1);
        if (inp == ENTER) {
            return selected;
        }
        if (inp == ESC)
            return 2;
    }
}

void settingsmenu(settings& s) {
    int x = 0;
    int y = 0;
    int inp;
    for (;;) {
        printsettings(s, x, y);
        inp = _getch();
        if (inp == UP)
            y--;
        if (inp == DOWN)
            y++;
        if (inp == LEFT)
            x--;
        if (inp == RIGHT)
            x++;
        if (y < 0) y = 0;
        if (y > 3) y = 3;
        if (x < 0) x = 0;
        if (y == 2) {
            if (x > 4) x = 4;
        }
        else {
            if (x > 1) x = 1;
        }
        if (inp == ESC) {
            writeSettings(s);
            system("CLS");
            return;
        }
        if (inp == ENTER) {
            switch (y) {
            case 0:
                if (x == 0) s.devMode = true;
                else s.devMode = false;
                break;
            case 1:
                if (x == 0) s.multiMode = true;
                else s.multiMode = false;
                break;
            case 2:
                s.gridsize = x + 3;
                break;
            case 3:
                writeSettings(s);
                system("CLS");
                return;
            }
        }
    }
}
void startmenu(settings& player) {
    vector <string> menuitems = {"Start", "Settings", "Exit[esc]"};
    system("CLS");
    for (;;) {
        switch (multichoicemenu(menuitems)) {
        case 0:
            system("CLS");
            return;
        case 1:
            system("CLS");
            settingsmenu(player);
            break;
        case 2:
            exit(0);
        }
    }
}

//Felsökning
void getchtest() {
    int inp;
    for (;;) {
        inp = _getch();
        cout << inp << " ";
    }
}



int main()
{
    //getchtest();
    settings s;
    readSettings(s);

    cout << "\033[?25l";
    for (;;) {
        s.turn = 0;
        startmenu(s);

        vector <int> nG;
        vector <string> dG;

        generateGrid(nG, dG, s);
        revampuserGuess(nG, dG, s);
    }
    return 0;
}