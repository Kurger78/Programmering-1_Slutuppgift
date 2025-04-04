#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <fstream>
#include <cmath>
using namespace std;

// dG = Displaygrid, nG = Numbergrid Förkortas för att kunna koda lättare
// Endiminsionella arrayer används så kordinaterna komm att uttryckas i form "y * gridsize + x"
// T.ex kordinaten (2,3) kommer bli 3 * gridsize + 2

// Definerar värdet av olika tangenter för läsbarhet // Används för att kolla vilken knapp trycktes i _getch()
#define ENTER 13
#define ESC 27 // Escape
#define BACK 8 // Backspace
#define F 102 // F
#define UP 72 // Piltangenter Upp
#define LEFT 75 // Ner
#define RIGHT 77 // Höger
#define DOWN 80 // Vänster


struct settings { // Definerar en struct där inställningarna lagras
    bool devMode; // Developer mode, fuskbräde där alla svar finns för att lättare testa
    bool multiMode; // Multiplayer mode, ifall enspelar eller flerspelarläge är igång
    int gridsize; // Spelplanens storlek
    int turn; // Antalet gissningar som utförts // Visas i slutet och hanterar vems tur det är i multiMode
    bool showUsedNum; // Ett andra bräde som visar vilka nummer som redan gissats och vilka som finns kvar
};


// Färg Funktioner (ANSI kod)
string bold(string inp) // Tjock text
{
    return "\033[1m" + inp + "\033[0m";
}
string white(string inp) // Vit text
{
    return "\033[97m" + inp + "\033[0m";
}
string red(string inp) // Röd text
{
    return "\033[31m" + inp + "\033[0m";
}
string blue(string inp) // Blå text
{
    return "\033[34m" + inp + "\033[0m";
}
string green(string inp) // Grön text
{
    return "\033[92m" + inp + "\033[0m";
}
string darkgray(string inp) // Mörkgrå text
{
    return "\033[90m" + inp + "\033[0m";
}


void clearScreen() // Clears screen
{
    cout << "\033[H\033[0J\033[H"; // Ansi kod för att rensa skärmen
}
void eraseBelow() // Suddar bort allting under markören
{
    cout << "\033[0J";
}


// Filhantering
void readSettings(settings& s) // Funktion för att läsa in inställningarna från en textfil vid spelets start
{
    int text;
    ifstream readFile("Settings.txt"); // Öppnar filen "Settings.txt" för läsning
    readFile >> text; // Läser första raden och sätter
    s.devMode = text; // det som värde på "devmode" (1/0)
    readFile >> text; // Läser nästa rad
    s.multiMode = text; // Blir värde på "multiMode" (1/0)
    readFile >> text; // Läser nästa rad
    s.gridsize = text; // Blir värde på planens storlek (3-7)
    readFile.close(); // Stänger filen
}
void writeSettings(settings s) // Funktion för att skriva inställningarna till en textfil efter att de ändrats
{
    int text;
    ofstream writeFile("Settings.txt"); // Öppnar filen "Settings.txt" för skrivning
    text = s.devMode; // Tar värdet av "devMode"
    writeFile << text; // Skriver det på första raden i filen
    text = s.multiMode; // Tar värdet av multiMode
    writeFile << "\n" << text; // Skriver det på andra raden
    text = s.gridsize; // Tar spelplanens storlek
    writeFile << "\n" << text; // Skriver det på tredje raden
    writeFile.close(); // Stänger filen
}


// Funktioner som kollar bingo
bool hBingo(vector<string> dG, settings s) // Kollar om någon rad har bingo
{
    bool redBingo, blueBingo;
    for (int y = 0; y < s.gridsize; y++) // Loopar igenom varje rad
    {
        redBingo = true;
        blueBingo = true;
        for (int x = 0; x < s.gridsize; x++) // Loppar igenom varje kolumn
        {
            if (dG[y * s.gridsize + x] != red("#")) // Om något tecken på raden inte är rött har röd inte bingo
                redBingo = false;
            if (dG[y * s.gridsize + x] != blue("#")) // Om något tecken på raden inte är blått har blå inte bingo
                blueBingo = false;

            if (x + 1 == s.gridsize && (redBingo || blueBingo)) // Ifall vi är på sista kolumnen och alla tecken vart en enda färg
                return true;
        }
    }
    return false; // Ifall ingen rad haft bingo
}
bool vBingo(vector<string> dG, settings s) // Kollar om någon kolumn har bingo
{ // Allt samma förutom att X och Y bytt plats för att kolla kolumn istället för rad
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
bool LtoRDBingo(vector<string> dG, settings s) // Kollar om diagonalen från vänster till höger har bingo
{
    bool redBingo = true, blueBingo = true;
    for (int i = 0; i < s.gridsize; i++) // Vänster till höger diagonal 
    { // betydder att x och y har samma värde och därför används endast en for loop
        if (dG[i * s.gridsize + i] != red("#")) // Ifall något tecken inte är rött har inte röd bingo
            redBingo = false;
        if (dG[i * s.gridsize + i] != blue("#")) // Ifall något tecken inte är blått har inte blå bingo
            blueBingo = false;
    }
    if (blueBingo || redBingo)  // Ifall blå eller röd har bingo
        return true;
    else
        return false; // Om varken blå eller röd har bingo
}
bool RtoLDBingo(vector<string> dG, settings s) // Kollar om diagonalen från höger till vänster har bingo
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

bool bingocheck(vector<string> dG, settings s) // Sammanställer booleanska värdena
{ // Ifall bingo på något sätt
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
void generateGrid(vector<int>& nG, vector<string>& dG, settings s) // Funktion som skapar brädet
{
    // Skapar första hur spelplanen ser ut med bokstäverna i stigande ordning i varje cell
    string text;
    for (int i = 0; i < s.gridsize * s.gridsize; i++) // Går igenom varje cell på planen
    {
        if (i < 26) // De första 26 cellerna får stora bokstäver
            text = 'A' + i;
        else // Resten får små
            text = 'a' + i - 26;
        dG.push_back(text); // Lägger varje bokstav sist i arrayen
    } 

    srand(time(0)); // Ger rand() ett frö baserat på tiden
    bool used;

    while (nG.size() < (s.gridsize * s.gridsize)) // Loopar tills Numbergrid är en array av rätt storlek
    {
        used = false;
        int num = rand() % (s.gridsize * s.gridsize) + 1; // Slumpar ett nummer mellan 1 och 25 (ifall gridsize är 5)

        for (int i = 0; i < nG.size(); i++) // Loopar genom varje element i numbergrid
        {
            if (num == nG[i]) // Ifall numret som slumpats redan använts
                used = true;
        }
        if (!used) // Om numret inte använts
        {
            nG.push_back(num); // Lägger numret sist i arrayen
        }
    }
}

void printGrid(vector<int> nG, vector<string> dG, settings s, vector<int> usedNumbers) // Skriver ut spelplanen
{
    cout << "\033[H"; // Ansi kod för att gå till första raden på skärmen
    cout << "= BINGO ===="; // Ritar upp spelplanets gränser

    for (int i = 0; i < s.gridsize-3; i++) // Justerar spelplanens gränser beroende på hur stor planen är
    {
        cout << "===";
    }
    if (s.showUsedNum) // Ifall brädet med använda nummer är igång
    {
        cout << "============"; // Förstorar gränserna
        for (int i = 0; i < s.gridsize - 3; i++) // Justerar spelplanens gränser beroende på hur stor planen är
        {
            cout << "===";
        }
    }
    cout << "\n";


    for (int y = 0; y < s.gridsize; y++) // Går igenom varje rad
    {
        cout << "| "; // Spelplan gräns
        for (int x = 0; x < s.gridsize; x++) // Går igenom varje kolumn
        {
            cout << dG[y * s.gridsize + x] << "  "; // Skriver ut displaygrid
        }
        cout << "|  "; // Spelplan gräns


        // Extra bräde för att visa vilka nummer som använts
        if (s.showUsedNum) {
            for (int x = 1; x <= s.gridsize; x++) // Går igenom varje kolumn
            {
                bool used = false; // Definerar och återställer värdet
                for (int l = 0; l < usedNumbers.size(); l++) // Går igenom varje nummer som redan gissats
                {
                    if (usedNumbers[l] == y * s.gridsize + x) // Ifall samma
                        used = true;
                } // Kollar ifall nummret använts
                if (used) // om använts skriver rött X istället
                    cout << red("X") << "  ";
                else // Annars skriver siffran
                {
                    cout << y * s.gridsize + x << " ";
                    if (y * s.gridsize + x < 10) // Används för att hålla spelplanen jämn (kan skilja p.g.a antalet siffror)
                        cout << " ";
                }
            }
            cout << "|"; // Spelplan gräns
        }
        cout << endl; // Går till nästa rad
    }

    // Ritar upp planens nedre gränser
    if (s.multiMode) // om multiplayer skriver vems tur det är
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

    if (s.devMode) // Ifall fuskbrädet är aktiverat
    { // Ritar upp ett bräde där alla nummers position visas
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
    int guess; // Hanteras som själva gissningen
    char inp; // Input buffer för _getch();
    bool used; // Booleanskt värde för att kolla om det redan gissats
    string number; // Lagrar det insatta nummret i en string på grund av _getch() (Tar upp detta i nyckelaspekterna av readme filen).
    vector<int> usedNumbers; // Lagrar alla nummer som redan gissats

    printGrid(nG, dG, s, usedNumbers); // Skriver ut spelplanens utseende

    for (;;) // Oändlig loop
    {
        number = {}; // Nollställer värdet
        guess = 0; //   på variablerna
        used = false; //
        cout << "Back[Esc]   "; // Skriver ut kommandon
        if (s.showUsedNum) // Ifall brädet som visar gissade nummer är på
            cout << "Hide used numbers[F]"; 
        else // Ifall brädet som visar gissade nummer är av
            cout << "Show used numbers[F]";

        cout << "\nGuess a number between 1 - " << s.gridsize * s.gridsize << endl; // Säger vad användaren ska göra
        eraseBelow(); // Suddar bort allting som kommer efter markören (Användarens förra gissning)

        for (;;) // Oändlig loop // Integer med hjälp av _getch() och hantering av andra kommandon
        {
            inp = _getch(); // Låter användaren mata in ett tecken
            if ('0' <= inp && inp <= '9') // Ifall nummertangent trycks
            {
                number += inp; // Lägger till numret i slutet av stringen
                cout << inp; // skriver ut siffran som trycktes
            }
            else if (inp == BACK && number.size() > 0) // Ifall suddknappen användes och det finns ett element i stringen
            {
                number.pop_back(); // Tar bort sista nummret på stringen
                cout << "\033[1D \033[1D"; // Suddar senaste tecknet på skärmen
            }
            else if (inp == ENTER && number.size() > 0) // Ifall enter användes och det finns ett element i stringen
            {
                for (int i = 0; i < number.size(); i++) // Konverterar stringen fylld av nummer till en integer
                {
                    guess += (number[i] - '0') * pow(10, number.size() - i - 1); // Visar detta i "Nyckelaspekter" på github readme filen
                }
                break; // Bryter sedan ut ur oändliga loopen för att hantera gissningen
            }
            else if (inp == ESC) // Ifall escape användes retuneras användaren till start menyn
            {
                return;
            }
            else if (inp == F) // Ifall F trycktes (Kommandot för brädet som visar gissade nummer)
            {
                if (s.showUsedNum) s.showUsedNum = false; // Ifall sant, blir falskt
                else s.showUsedNum = true;                // Ifall falskt, blir sant
                clearScreen();
                break; // Bryter loopen för att rendera om programmet
            }
        }

        for (int i = 0; i < usedNumbers.size(); i++) // Går igenom varje element som redan gissats och
        {                                            // tolkar ifall det använts // Ifall det har, blir "used" sant
            if (guess == usedNumbers.at(i))
                used = true;
        }
        if (!used && 0 < guess && guess < s.gridsize * s.gridsize + 1) // Ifall gissningen inte gissats tidigare
        {                                                              // och är inom det giltiga intervallet
            s.turn++; // Antalet giltiga gissningar + 1
            usedNumbers.push_back(guess); // Lägger till gissningen sist i arrayen av använda nummer

            for (int i = 0; i < s.gridsize * s.gridsize; i++) // Går igenom varje element på spelplanen
            {
                if (guess == nG[i]) // Positionen i där nummret är samma som gissningen
                {                   // Visar vart nummret var genom att ändra displaygriden på samma kordinat
                    if (s.multiMode && s.turn % 2 == 0) // Om det är blå's tur
                    {
                        dG[i] = blue("#"); 
                    }
                    else // Om det är röd's tur eller singleplayer
                        dG[i] = red("#");
                }
            }
        }
        printGrid(nG, dG, s, usedNumbers); // Skriver ut brädet efter ändringen
        if (bingocheck(dG, s)) // Ifall Bingo på något sätt
        {
            printGrid(nG, dG, s, usedNumbers); // Uppdaterar brädet för att visa vart det var bingo
            cout << "\r"; // Suddar linjer och tar markören till början på linjen
            eraseBelow();
            if (s.multiMode) // Om multiplayer
            {
                if (s.turn % 2 == 1) // Kollar vems tur och skriver ut vem som vann
                    cout << "Red won! It took " << s.turn << " guesses!\n";
                else
                    cout << "Blue won! It took " << s.turn << " guesses!\n";
            }
            else // om inte multiplayer
                cout << "You got BINGO in " << s.turn << " guesses!\n"; // Gratulerar och skriver hur många gissningar
            cout << "Press any key to return...";
            char inp = _getch();  // Väntar på en input från användaren innan den går vidare
            clearScreen();
            return;
        }
        else if (s.turn == s.gridsize * s.gridsize) // Ifall alla rutor har gissats. Är bara
        {                                           // möjligt i multiplayer och betyder att ingen vann
            eraseBelow();
            cout << "You both lost!\n";
            cout << "Press any key to return...";
            char inp = _getch(); // Väntar på en input från användaren innan den går vidare
            return;
        }
    }
}



// Funktioner för Inställningar
void printsettings(settings player, int x, int y) // Skriver ut inställningarna
{ // Ser väldigt komplicerat ut men mycket är samma // Ifall markerad visa en pil och markera den vit,
  // visa den aktiva inställningen i grönt och annars gör texten mörkgrå
    cout << "\033[H"; // Går till skärmens första rad och skriver då över det gammla.

    cout << "  Developer Mode\n";
    if (y == 0) // Ifall markören är på första raden
    {
        cout << ">   ";
        if (x == 0) cout << bold(white("On")); // Ifall "On" är vald skriv den i tjock vit text
        else if (player.devMode) cout << green("On"); // anars ifall det är den aktiva inställningen blir den grön
        else cout << darkgray("On"); // Annars mörkgrå
        cout << darkgray("/");
        if (x == 1) cout << bold(white("Off"));
        else if (!(player.devMode)) cout << green("Off");
        else cout << darkgray("Off");
    }
    else // Om inte vald skriver allting i grått förutom valda inställningen som blir grön
    {
        cout << "    ";
        if (player.devMode) cout << green("On") << darkgray("/Off");
        else cout << darkgray("On/") << green("Off");
    }
    // Resten följer samma princip och inte särskilt annorlunda

    cout << "\n\n  Multiplayer Mode\n";
    if (y == 1) // Ifall markören är på andra raden
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
    if (y == 2) // Ifall markören är på tredje raden
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
        cout << darkgray("  Back[Esc] ");  // Mörkgrå text
    }
    return;

}
void settingsmenu(settings& s) {
    int x = 0; // Kordinat system 
    int y = 0; //
    char inp; // Variabel som buffer för input
    for (;;) {
        printsettings(s, x, y); // Skriver ut inställningarna
        inp = _getch(); // Tar en input
        // Styr kordinat systemet med hjälp av piltangenterna
        // Upp och ner eftersom det skrivs från första rad (y = 0) till sista rad (y = 3)
        if (inp == UP)
            y--;
        if (inp == DOWN)
            y++;
        if (inp == LEFT)
            x--;
        if (inp == RIGHT)
            x++;

        // Så att man inte kan gå utanför alternativen
        if (y < 0) y = 0; // y kan inte bli mindre än 0
        if (y > 3) y = 3; // y kan inte bli mindre än 3 eftersom det endast finns 4 alternativ
        if (x < 0) x = 0; // x kan inte bli mindre än 0
        if (y == 2) // Finns fem olika alternativ när y = 2 på grund av de olika spelplan storlekarna
        {
            if (x > 4) x = 4;
        }
        else // Annars finns det endast 2 alternativ, On/Off
        {
            if (x > 1) x = 1;
        }


        if (inp == ESC) // Ifall Escape
        {
            writeSettings(s); // Sparar inställningarna och går tillbaka till startmenyn
            return;
        }
        if (inp == ENTER) // Ifall Enter sätt nytt värde på de olika inställningarna
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
                writeSettings(s); // Sparar inställningarna och går tillbaka till startmenyn
                return;
            }
        }
    }
}

// Startmeny Funktioner
void printStartmenu(int selected) // Skriver ut start menyn
{
    cout << "\033[H"; // Går till skärmens första rad
    if (selected == 0) // Ifall första raden
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
    for (;;) // Oändlig loop för menyn
    {
        for (;;) // Oändlig loop för inputen
        {
            printStartmenu(selected); // Skriver ut menyn
            inp = _getch(); // Användarens input
            if (inp == ENTER) // Bryter input loopen för att tolka valet
                break;
            else if (inp == ESC) // Stänger av
                return 1;
            else if (inp == UP) // Styrning av menyn med pil tangenterna
                selected--;
            else if (inp == DOWN)
                selected++;

            if (selected < 0) // Får inte vara mindre än 0
                selected = 0;
            if (selected > 2) // Får inte vara större än 2
                selected = 2;
        }

        switch (selected) // Tolkar valet
        {
        case 0: // Startar spelet
            clearScreen(); // Rensar skärmen för att kunna rita upp spelplannen
            return false;
        case 1: // Går int i menyn
            clearScreen(); // Rensar skärmen så inställningarna kan skrivas ut
            settingsmenu(player); // Går in i settings
            clearScreen(); // Rensar skärmen för att kunna rita om menyn
            break;
        case 2: // Avslutar programmet
            return true;
        }
    }
    return false;
}


int main()
{
    settings s; // Definerar vad profilen för settings kommer heta
    readSettings(s); // Läser in inställningarna som använts tidigare från "settings.txt" filen

    vector <int> nG; // Skapar en vector som ska lagra numrena
    vector <string> dG; // Skapar en vector som ska lagra den visuella delen

    cout << "\033[?25l"; // Gömmer markören för att den är jobbig
    for (;;) // Oändlig loop
    {
        s.turn = 0; // Nollställer värdet av variablerna efter varje omgång
        s.showUsedNum = false; //
        nG = {}; //
        dG = {}; //
        
        if (startmenu(s)) // Visar startmenyn så användaren kan välja vad den vill göra
            break;
        generateGrid(nG, dG, s); // Skapar spelbrädet för denna omgången
        userGuess(nG, dG, s); // Går in i gissnings funktion.
    }
    return 0;
}