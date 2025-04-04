# Bingo
## Sammanfattning
Det här är gjort som slutuppgift i programmering 1.
Spelet är som vanlig bingo fast det är du som skriver nummrena istället och försöker skapa x i rad på ett bräde.
Jag tycker det här blev väldigt lyckat i användar vänlighet och är extremt nöjd med hur meny systemet fungerar även om koden hade kunnat vara bättre.
Det är också helt säkrat från missinputs och har inte hittat ett enda sätt att paja spelet på.

## Hur används programmet
Spelet har väldigt lätta kontroller, piltangenter för att navigera menyn och enter för att verifiera val.
Escape för att gå tillbaka till menyn och avsluta.
När spelet har startats kan man endast skriva in siffror. För att skicka in din gissning klickar man på Enter och för att sudda klickar man på Backspace.

Målet är att få fyra i rad ifall brädet är 4x4. Det gör man genom att bara gissa tills det händer. Det finns Utvecklar läge eller Developer mode där ett fuskbräde syns som visar brädet du ska försöka gissa på.
Det finns också Flerspelarläge där man kan köra 1 mot 1 och man gör varannan gissning och står då vems tur det är i botten av spelplanen.

## Nyckelaspekter
### String till Int
En utmaning jag mötte var att inputa en flersiffrig integer med hjälp av _getch().
Jag ville inte använda cin här eftersom det betyder att jag inte kan kontrollera vad som skrivs och programmet kan då bråka ifall bokstäver eller specialtecken skrivs in. Jag ville också ha möjligheten att gå tillbaka till menyn med hjälp av Esc-knappen och enda lösningen jag såg till det här var att använda _getch().
Det här löste jag genom att först kolla om inputen var ett nummer och ifall det var det sätta nummret i en string för lagring.

int inp;

string text;

inp = _getch()

if ('0' <= inp && inp <= '9') {

  text += inp
  
}

Nu ligger talet som kan ha flera siffror exakt som användaren skrev det förutom att det är i en string.
Ifall änvändaren suddar tas bara sista elementet i stringen bort.
Endast när användaren klickar "Enter" för att säkerställa sin gissning överförs stringen till en integer.
Att bara plus ihop alla nummer som ligger lagrade ger oss siffersumman och är då inte rätt svar. Ifall det är ett två siffror långt tal måste första siffran gångras med 10
och andra gångras med 1. Jag kom då på att jag kan använda mig av tio potenser och kom fram till den här lösningen.

guess är integeren som hanteras som den slutgiltiga gissningen
och number är en string som förvarar numrena i ordform.

for (int i = 0; i < number.size(); i++) {

guess += (number[i] - '0') * pow(10, number.size() - i - 1)

}

Går igenom varje siffra som ligger i stringen. Tar sedan siffran * 10^(storleken av stringen - 1)


I ett tresiffrigt tal blir då första siffran

x * 10^(3-1) = x*10^2

om hela talet är xyz blir då gissningen

x * 100 + y * 10 + z * 1


## Utmaningar
Det är inte riktigt något jag känner att spelet verkligen saknar och fanns inget problem jag inte lyckades lösa.

## Vad kommer härnäst
Jag har inte gjort så att det finns flera vinstkombinationer eftersom jag kände att det inte hade lagt till något och bara gjort så att man måste gissa på i stort sett alla nummer. Att lägga till en AI man kan köra mot hade varit en annan sak jag hade kunnat göra men kände inte heller att den skulle göra så mycket för spelet. Båda sakerna är dock något som hade gett spelet mer variation och däför förbättrat det.

