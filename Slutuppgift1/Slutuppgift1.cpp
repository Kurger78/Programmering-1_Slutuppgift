#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <fstream>
#include <cmath>
using namespace std;

// dG = Displaygrid, nG = Numbergrid F�rkortas f�r att kunna koda l�ttare
// Endiminsionella arrayer anv�nds s� kordinaterna komm att uttryckas i form "y * gridsize + x"
// T.ex kordinaten (2,3) kommer bli 3 * gridsize + 2

// Definerar v�rdet av olika tangenter f�r l�sbarhet // Anv�nds f�r att kolla vilken knapp trycktes i _getch()
#define ENTER 13
#define ESC 27 // Escape
#define BACK 8 // Backspace
#define F 102 // F
#define UP 72 // Piltangenter Upp
#define LEFT 75 // Ner
#define RIGHT 77 // H�ger
#define DOWN 80 // V�nster


struct settings { // Definerar en struct d�r inst�llningarna lagras
    bool devMode; // Developer mode, fuskbr�de d�r alla svar finns f�r att l�ttare testa
    bool multiMode; // Multiplayer mode, ifall enspelar eller flerspelarl�ge �r ig�ng
    int gridsize; // Spelplanens storlek
    int turn; // Antalet gissningar som utf�rts // Visas i slutet och hanterar vems tur det �r i multiMode
    bool showUsedNum; // Ett andra br�de som visar vilka nummer som redan gissats och vilka som finns kvar
};


// F�rg Funktioner (ANSI kod)
string bold(string inp) // Tjock text
{
    return "\033[1m" + inp + "\033[0m";
}
string white(string inp) // Vit text
{
    return "\033[97m" + inp + "\033[0m";
}
string red(string inp) // R�d text
{
    return "\033[31m" + inp + "\033[0m";
}
string blue(string inp) // Bl� text
{
    return "\033[34m" + inp + "\033[0m";
}
string green(string inp) // Gr�n text
{
    return "\033[92m" + inp + "\033[0m";
}
string darkgray(string inp) // M�rkgr� text
{
    return "\033[90m" + inp + "\033[0m";
}


void clearScreen() // Clears screen
{
    cout << "\033[H\033[0J\033[H"; // Ansi kod f�r att rensa sk�rmen
}
void eraseBelow() // Suddar bort allting under mark�ren
{
    cout << "\033[0J";
}


// Filhantering
void readSettings(settings& s) // Funktion f�r att l�sa in inst�llningarna fr�n en textfil vid spelets start
{
    int text;
    ifstream readFile("Settings.txt"); // �ppnar filen "Settings.txt" f�r l�sning
    readFile >> text; // L�ser f�rsta raden och s�tter
    s.devMode = text; // det som v�rde p� "devmode" (1/0)
    readFile >> text; // L�ser n�sta rad
    s.multiMode = text; // Blir v�rde p� "multiMode" (1/0)
    readFile >> text; // L�ser n�sta rad
    s.gridsize = text; // Blir v�rde p� planens storlek (3-7)
    readFile.close(); // St�nger filen
}
void writeSettings(settings s) // Funktion f�r att skriva inst�llningarna till en textfil efter att de �ndrats
{
    int text;
    ofstream writeFile("Settings.txt"); // �ppnar filen "Settings.txt" f�r skrivning
    text = s.devMode; // Tar v�rdet av "devMode"
    writeFile << text; // Skriver det p� f�rsta raden i filen
    text = s.multiMode; // Tar v�rdet av multiMode
    writeFile << "\n" << text; // Skriver det p� andra raden
    text = s.gridsize; // Tar spelplanens storlek
    writeFile << "\n" << text; // Skriver det p� tredje raden
    writeFile.close(); // St�nger filen
}


// Funktioner som kollar bingo
bool hBingo(vector<string> dG, settings s) // Kollar om n�gon rad har bingo
{
    bool redBingo, blueBingo;
    for (int y = 0; y < s.gridsize; y++) // Loopar igenom varje rad
    {
        redBingo = true;
        blueBingo = true;
        for (int x = 0; x < s.gridsize; x++) // Loppar igenom varje kolumn
        {
            if (dG[y * s.gridsize + x] != red("#")) // Om n�got tecken p� raden inte �r r�tt har r�d inte bingo
                redBingo = false;
            if (dG[y * s.gridsize + x] != blue("#")) // Om n�got tecken p� raden inte �r bl�tt har bl� inte bingo
                blueBingo = false;

            if (x + 1 == s.gridsize && (redBingo || blueBingo)) // Ifall vi �r p� sista kolumnen och alla tecken vart en enda f�rg
                return true;
        }
    }
    return false; // Ifall ingen rad haft bingo
}
bool vBingo(vector<string> dG, settings s) // Kollar om n�gon kolumn har bingo
{ // Allt samma f�rutom att X och Y bytt plats f�r att kolla kolumn ist�llet f�r rad
    bool redBingo, blueBingo;
    for (int x = 0; x < s.gridsize; x++) {
        redBingo = true;
        blueBingo = true;
        for (int y = 0; y < s.gridsize; y++)
        {
            if (dG[y * s.gridsize + x] != red("#"))
                redBingo = false;
            if (dG[y * s.gridsize + x] != blue("#"))
                blueBingo = false;

            if (y + 1 == s.gridsize && (redBingo || blueBingo))
                return true;
        }
    }
    return false;
}
bool LtoRDBingo(vector<string> dG, settings s) // Kollar om diagonalen fr�n v�nster till h�ger har bingo
{
    bool redBingo = true, blueBingo = true;
    for (int i = 0; i < s.gridsize; i++) // V�nster till h�ger diagonal 
    { // betydder att x och y har samma v�rde och d�rf�r anv�nds endast en for loop
        if (dG[i * s.gridsize + i] != red("#")) // Ifall n�got tecken inte �r r�tt har inte r�d bingo
            redBingo = false;
        if (dG[i * s.gridsize + i] != blue("#")) // Ifall n�got tecken inte �r bl�tt har inte bl� bingo
            blueBingo = false;
    }
    if (blueBingo || redBingo)  // Ifall bl� eller r�d har bingo
        return true;
    else
        return false; // Om varken bl� eller r�d har bingo
}
bool RtoLDBingo(vector<string> dG, settings s) // Kollar om diagonalen fr�n h�ger till v�nster har bingo
{ // I stort sett samma som andra diagonalen
    bool redBingo = true, blueBingo = true;
    for (int i = 0; i < s.gridsize; i++) {
        if (dG[i * s.gridsize + (s.gridsize - i + 1)] != red("#"))
            redBingo = false;
        if (dG[i * s.gridsize + (s.gridsize - i + 1)] != blue("#"))
            blueBingo = false;
    }
    if (blueBingo || redBingo)
        return true;
    else
        return false;
}

bool bingocheck(vector<string> dG, settings s) // Sammanst�ller booleanska v�rdena
{ // Ifall bingo p� n�got s�tt
    if (hBingo(dG, s))
        return true;
    if (vBingo(dG, s))
        return true;
    if (LtoRDBingo(dG, s))
        return true;
    if (RtoLDBingo(dG, s))
        return true;
    return false; // Annars
}


//Spelet
void generateGrid(vector<int>& nG, vector<string>& dG, settings s) // Funktion som skapar br�det
{
    // Skapar f�rsta hur spelplanen ser ut med bokst�verna i stigande ordning i varje cell
    string text;
    for (int i = 0; i < s.gridsize * s.gridsize; i++) // G�r igenom varje cell p� planen
    {
        if (i < 26) // De f�rsta 26 cellerna f�r stora bokst�ver
            text = 'A' + i;
        else // Resten f�r sm�
            text = 'a' + i - 26;
        dG.push_back(text); // L�gger varje bokstav sist i arrayen
    } 

    srand(time(0)); // Ger rand() ett fr� baserat p� tiden
    bool used;

    while (nG.size() < (s.gridsize * s.gridsize)) // Loopar tills Numbergrid �r en array av r�tt storlek
    {
        used = false;
        int num = rand() % (s.gridsize * s.gridsize) + 1; // Slumpar ett nummer mellan 1 och 25 (ifall gridsize �r 5)

        for (int i = 0; i < nG.size(); i++) // Loopar genom varje element i numbergrid
        {
            if (num == nG[i]) // Ifall numret som slumpats redan anv�nts
                used = true;
        }
        if (!used) // Om numret inte anv�nts
        {
            nG.push_back(num); // L�gger numret sist i arrayen
        }
    }
}

void printGrid(vector<int> nG, vector<string> dG, settings s, vector<int> usedNumbers) // Skriver ut spelplanen
{
    cout << "\033[H"; // Ansi kod f�r att g� till f�rsta raden p� sk�rmen
    cout << "= BINGO ===="; // Ritar upp spelplanets gr�nser

    for (int i = 0; i < s.gridsize-3; i++) // Justerar spelplanens gr�nser beroende p� hur stor planen �r
    {
        cout << "===";
    }
    if (s.showUsedNum) // Ifall br�det med anv�nda nummer �r ig�ng
    {
        cout << "============"; // F�rstorar gr�nserna
        for (int i = 0; i < s.gridsize - 3; i++) // Justerar spelplanens gr�nser beroende p� hur stor planen �r
        {
            cout << "===";
        }
    }
    cout << "\n";


    for (int y = 0; y < s.gridsize; y++) // G�r igenom varje rad
    {
        cout << "| "; // Spelplan gr�ns
        for (int x = 0; x < s.gridsize; x++) // G�r igenom varje kolumn
        {
            cout << dG[y * s.gridsize + x] << "  "; // Skriver ut displaygrid
        }
        cout << "|  "; // Spelplan gr�ns


        // Extra br�de f�r att visa vilka nummer som anv�nts
        if (s.showUsedNum) {
            for (int x = 1; x <= s.gridsize; x++) // G�r igenom varje kolumn
            {
                bool used = false; // Definerar och �terst�ller v�rdet
                for (int l = 0; l < usedNumbers.size(); l++) // G�r igenom varje nummer som redan gissats
                {
                    if (usedNumbers[l] == y * s.gridsize + x) // Ifall samma
                        used = true;
                } // Kollar ifall nummret anv�nts
                if (used) // om anv�nts skriver r�tt X ist�llet
                    cout << red("X") << "  ";
                else // Annars skriver siffran
                {
                    cout << y * s.gridsize + x << " ";
                    if (y * s.gridsize + x < 10) // Anv�nds f�r att h�lla spelplanen j�mn (kan skilja p.g.a antalet siffror)
                        cout << " ";
                }
            }
            cout << "|"; // Spelplan gr�ns
        }
        cout << endl; // G�r till n�sta rad
    }

    // Ritar upp planens nedre gr�nser
    if (s.multiMode) // om multiplayer skriver vems tur det �r
    {
        if (s.turn % 2 == 0)
            cout << "= " << red("Turn") << " =====";
        else
            cout << "= " << blue("Turn") << " =====";
    }
    else
        cout << "============";
    if (s.showUsedNum) {
        cout << "============";
        for (int i = 0; i < s.gridsize - 3; i++)
        {
            cout << "===";
        }
    }
    for (int i = 0; i < s.gridsize - 3; i++)
    {
        cout << "===";
    }
    cout << endl;

    if (s.devMode) // Ifall fuskbr�det �r aktiverat
    { // Ritar upp ett br�de d�r alla nummers position visas
        for (int i = 0; i < s.gridsize * s.gridsize; i++) {
            cout << nG[i] << " ";
            if (nG[i] < 10)
                cout << " ";
            if (i % s.gridsize == s.gridsize - 1) // Ifall "gridsize" antal nummer skrivits ut
                cout << endl; // Ny rad
        }
    }
}

// Hantering av gissning samt resultat av "bingocheck"
void userGuess(vector<int> nG, vector<string> dG, settings s)
{
    int guess; // Hanteras som sj�lva gissningen
    char inp; // Input buffer f�r _getch();
    bool used; // Booleanskt v�rde f�r att kolla om det redan gissats
    string number; // Lagrar det insatta nummret i en string p� grund av _getch() (Tar upp detta i nyckelaspekterna av readme filen).
    vector<int> usedNumbers; // Lagrar alla nummer som redan gissats

    printGrid(nG, dG, s, usedNumbers); // Skriver ut spelplanens utseende

    for (;;) // O�ndlig loop
    {
        number = {}; // Nollst�ller v�rdet
        guess = 0; //   p� variablerna
        used = false; //
        cout << "Back[Esc]   "; // Skriver ut kommandon
        if (s.showUsedNum) // Ifall br�det som visar gissade nummer �r p�
            cout << "Hide used numbers[F]"; 
        else // Ifall br�det som visar gissade nummer �r av
            cout << "Show used numbers[F]";

        cout << "\nGuess a number between 1 - " << s.gridsize * s.gridsize << endl; // S�ger vad anv�ndaren ska g�ra
        eraseBelow(); // Suddar bort allting som kommer efter mark�ren (Anv�ndarens f�rra gissning)

        for (;;) // O�ndlig loop // Integer med hj�lp av _getch() och hantering av andra kommandon
        {
            inp = _getch(); // L�ter anv�ndaren mata in ett tecken
            if ('0' <= inp && inp <= '9') // Ifall nummertangent trycks
            {
                number += inp; // L�gger till numret i slutet av stringen
                cout << inp; // skriver ut siffran som trycktes
            }
            else if (inp == BACK && number.size() > 0) // Ifall suddknappen anv�ndes och det finns ett element i stringen
            {
                number.pop_back(); // Tar bort sista nummret p� stringen
                cout << "\033[1D \033[1D"; // Suddar senaste tecknet p� sk�rmen
            }
            else if (inp == ENTER && number.size() > 0) // Ifall enter anv�ndes och det finns ett element i stringen
            {
                for (int i = 0; i < number.size(); i++) // Konverterar stringen fylld av nummer till en integer
                {
                    guess += (number[i] - '0') * pow(10, number.size() - i - 1); // Visar detta i "Nyckelaspekter" p� github readme filen
                }
                break; // Bryter sedan ut ur o�ndliga loopen f�r att hantera gissningen
            }
            else if (inp == ESC) // Ifall escape anv�ndes retuneras anv�ndaren till start menyn
            {
                return;
            }
            else if (inp == F) // Ifall F trycktes (Kommandot f�r br�det som visar gissade nummer)
            {
                if (s.showUsedNum) s.showUsedNum = false; // Ifall sant, blir falskt
                else s.showUsedNum = true;                // Ifall falskt, blir sant
                clearScreen();
                break; // Bryter loopen f�r att rendera om programmet
            }
        }

        for (int i = 0; i < usedNumbers.size(); i++) // G�r igenom varje element som redan gissats och
        {                                            // tolkar ifall det anv�nts // Ifall det har, blir "used" sant
            if (guess == usedNumbers.at(i))
                used = true;
        }
        if (!used && 0 < guess && guess < s.gridsize * s.gridsize + 1) // Ifall gissningen inte gissats tidigare
        {                                                              // och �r inom det giltiga intervallet
            s.turn++; // Antalet giltiga gissningar + 1
            usedNumbers.push_back(guess); // L�gger till gissningen sist i arrayen av anv�nda nummer

            for (int i = 0; i < s.gridsize * s.gridsize; i++) // G�r igenom varje element p� spelplanen
            {
                if (guess == nG[i]) // Positionen i d�r nummret �r samma som gissningen
                {                   // Visar vart nummret var genom att �ndra displaygriden p� samma kordinat
                    if (s.multiMode && s.turn % 2 == 0) // Om det �r bl�'s tur
                    {
                        dG[i] = blue("#"); 
                    }
                    else // Om det �r r�d's tur eller singleplayer
                        dG[i] = red("#");
                }
            }
        }
        printGrid(nG, dG, s, usedNumbers); // Skriver ut br�det efter �ndringen
        if (bingocheck(dG, s)) // Ifall Bingo p� n�got s�tt
        {
            printGrid(nG, dG, s, usedNumbers); // Uppdaterar br�det f�r att visa vart det var bingo
            cout << "\r"; // Suddar linjer och tar mark�ren till b�rjan p� linjen
            eraseBelow();
            if (s.multiMode) // Om multiplayer
            {
                if (s.turn % 2 == 1) // Kollar vems tur och skriver ut vem som vann
                    cout << "Red won! It took " << s.turn << " guesses!\n";
                else
                    cout << "Blue won! It took " << s.turn << " guesses!\n";
            }
            else // om inte multiplayer
                cout << "You got BINGO in " << s.turn << " guesses!\n"; // Gratulerar och skriver hur m�nga gissningar
            cout << "Press any key to return...";
            char inp = _getch();  // V�ntar p� en input fr�n anv�ndaren innan den g�r vidare
            clearScreen();
            return;
        }
        else if (s.turn == s.gridsize * s.gridsize) // Ifall alla rutor har gissats. �r bara
        {                                           // m�jligt i multiplayer och betyder att ingen vann
            eraseBelow();
            cout << "You both lost!\n";
            cout << "Press any key to return...";
            char inp = _getch(); // V�ntar p� en input fr�n anv�ndaren innan den g�r vidare
            return;
        }
    }
}



// Funktioner f�r Inst�llningar
void printsettings(settings player, int x, int y) // Skriver ut inst�llningarna
{ // Ser v�ldigt komplicerat ut men mycket �r samma // Ifall markerad visa en pil och markera den vit,
  // visa den aktiva inst�llningen i gr�nt och annars g�r texten m�rkgr�
    cout << "\033[H"; // G�r till sk�rmens f�rsta rad och skriver d� �ver det gammla.

    cout << "  Developer Mode\n";
    if (y == 0) // Ifall mark�ren �r p� f�rsta raden
    {
        cout << ">   ";
        if (x == 0) cout << bold(white("On")); // Ifall "On" �r vald skriv den i tjock vit text
        else if (player.devMode) cout << green("On"); // anars ifall det �r den aktiva inst�llningen blir den gr�n
        else cout << darkgray("On"); // Annars m�rkgr�
        cout << darkgray("/");
        if (x == 1) cout << bold(white("Off"));
        else if (!(player.devMode)) cout << green("Off");
        else cout << darkgray("Off");
    }
    else // Om inte vald skriver allting i gr�tt f�rutom valda inst�llningen som blir gr�n
    {
        cout << "    ";
        if (player.devMode) cout << green("On") << darkgray("/Off");
        else cout << darkgray("On/") << green("Off");
    }
    // Resten f�ljer samma princip och inte s�rskilt annorlunda

    cout << "\n\n  Multiplayer Mode\n";
    if (y == 1) // Ifall mark�ren �r p� andra raden
    {
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


    cout << "\n\n  Gridsize\n";
    if (y == 2) // Ifall mark�ren �r p� tredje raden
    {
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


    if (y == 3) // Ifall sista raden
        cout << bold(white(">  Back[Esc]")); // Tjock vit text 
    else { 
        cout << darkgray("  Back[Esc] ");  // M�rkgr� text
    }
    return;

}
void settingsmenu(settings& s) {
    int x = 0; // Kordinat system 
    int y = 0; //
    char inp; // Variabel som buffer f�r input
    for (;;) {
        printsettings(s, x, y); // Skriver ut inst�llningarna
        inp = _getch(); // Tar en input
        // Styr kordinat systemet med hj�lp av piltangenterna
        // Upp och ner eftersom det skrivs fr�n f�rsta rad (y = 0) till sista rad (y = 3)
        if (inp == UP)
            y--;
        if (inp == DOWN)
            y++;
        if (inp == LEFT)
            x--;
        if (inp == RIGHT)
            x++;

        // S� att man inte kan g� utanf�r alternativen
        if (y < 0) y = 0; // y kan inte bli mindre �n 0
        if (y > 3) y = 3; // y kan inte bli mindre �n 3 eftersom det endast finns 4 alternativ
        if (x < 0) x = 0; // x kan inte bli mindre �n 0
        if (y == 2) // Finns fem olika alternativ n�r y = 2 p� grund av de olika spelplan storlekarna
        {
            if (x > 4) x = 4;
        }
        else // Annars finns det endast 2 alternativ, On/Off
        {
            if (x > 1) x = 1;
        }


        if (inp == ESC) // Ifall Escape
        {
            writeSettings(s); // Sparar inst�llningarna och g�r tillbaka till startmenyn
            return;
        }
        if (inp == ENTER) // Ifall Enter s�tt nytt v�rde p� de olika inst�llningarna
        {
            switch (y) {
            case 0: // Devmode
                if (x == 0) s.devMode = true;
                else s.devMode = false;
                break;
            case 1: // Multimode
                if (x == 0) s.multiMode = true; 
                else s.multiMode = false;
                break;
            case 2: // Gridsize
                s.gridsize = x + 3; // Tredje alternativet (x = 2) = gridsize 5 // 2 + 3 = 5
                break;
            case 3: // Tillbaka
                writeSettings(s); // Sparar inst�llningarna och g�r tillbaka till startmenyn
                return;
            }
        }
    }
}

// Startmeny Funktioner
void printStartmenu(int selected) // Skriver ut start menyn
{
    cout << "\033[H"; // G�r till sk�rmens f�rsta rad
    if (selected == 0) // Ifall f�rsta raden
    {
        cout << bold("> Start");
    }
    else {
        cout << darkgray(" Start ");
    }
    cout << "\n";


    if (selected == 1) // Ifall Andra raden
    {
        cout << bold("> Settings");
    }
    else {
        cout << darkgray(" Settings ");
    }
    cout << "\n";


    if (selected == 2) // Ifall Tredje raden
    {
        cout << bold("> Exit[Esc]");
    }
    else {
        cout << darkgray(" Exit[Esc] ");
    }
    return;
}

bool startmenu(settings& player) {
    int selected = 0;
    char inp;
    clearScreen();
    for (;;) // O�ndlig loop f�r menyn
    {
        for (;;) // O�ndlig loop f�r inputen
        {
            printStartmenu(selected); // Skriver ut menyn
            inp = _getch(); // Anv�ndarens input
            if (inp == ENTER) // Bryter input loopen f�r att tolka valet
                break;
            else if (inp == ESC) // St�nger av
                return 1;
            else if (inp == UP) // Styrning av menyn med pil tangenterna
                selected--;
            else if (inp == DOWN)
                selected++;

            if (selected < 0) // F�r inte vara mindre �n 0
                selected = 0;
            if (selected > 2) // F�r inte vara st�rre �n 2
                selected = 2;
        }

        switch (selected) // Tolkar valet
        {
        case 0: // Startar spelet
            clearScreen(); // Rensar sk�rmen f�r att kunna rita upp spelplannen
            return false;
        case 1: // G�r int i menyn
            clearScreen(); // Rensar sk�rmen s� inst�llningarna kan skrivas ut
            settingsmenu(player); // G�r in i settings
            clearScreen(); // Rensar sk�rmen f�r att kunna rita om menyn
            break;
        case 2: // Avslutar programmet
            return true;
        }
    }
    return false;
}


int main()
{
    settings s; // Definerar vad profilen f�r settings kommer heta
    readSettings(s); // L�ser in inst�llningarna som anv�nts tidigare fr�n "settings.txt" filen

    vector <int> nG; // Skapar en vector som ska lagra numrena
    vector <string> dG; // Skapar en vector som ska lagra den visuella delen

    cout << "\033[?25l"; // G�mmer mark�ren f�r att den �r jobbig
    for (;;) // O�ndlig loop
    {
        s.turn = 0; // Nollst�ller v�rdet av variablerna efter varje omg�ng
        s.showUsedNum = false; //
        nG = {}; //
        dG = {}; //
        
        if (startmenu(s)) // Visar startmenyn s� anv�ndaren kan v�lja vad den vill g�ra
            break;
        generateGrid(nG, dG, s); // Skapar spelbr�det f�r denna omg�ngen
        userGuess(nG, dG, s); // G�r in i gissnings funktion.
    }
    return 0;
}