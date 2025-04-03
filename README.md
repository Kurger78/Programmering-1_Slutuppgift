# Bingo
## Sammanfattning
Det här är gjort som slutuppgift i programmering 1.
Spelet är som vanlig bingo fast det är du som skriver nummrena istället och försöker skapa x i rad på ett bräde.
## Bakgrund

## Nyckelaspekter
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


## Hur används programmet

## Utmaningar

## Vad kommer härnäst
