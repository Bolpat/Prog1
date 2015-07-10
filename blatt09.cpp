#include <iostream>
#include <iomanip>

#include <string>
#include <sstream>

#include <cmath>
#include <cctype>
#include <cstdlib>

using namespace std;

// Anmerkung: Globale Variablen wie diese vermeiden.
string str;    // speichert die bereits gelesenen Zeichen

// liest naechsten Ausdruck, Term, Faktor bzw. Operand aus der Eingabe.
double WertAusdruck();
double WertTerm();
double WertFaktor();
double WertOperand();

// Gibt eine string-Repraesentation der double-Zahl zurueck.
string double2string(double);

// Fehlerbehandlung mit optionaler Fehlermedlung
void fehler(string const & = "");

int main()
{
    cout << "Arithm. Ausdruck: " << flush;
    double ergebnis = WertAusdruck();
    if (cin.peek() == '\n') cout << "Ergebnis: " << ergebnis << endl;
    else
    {
        str += cin.peek();
        fehler("Ende der Eingabe erwartet.");
    }
    return 0;
}

double WertAusdruck()
{
    double summe;
    char z = cin.peek();
    switch (z) {
        case '+': cin >> z; str += z; summe =  WertTerm(); break;
        case '-': cin >> z; str += z; summe = -WertTerm(); break;
        default :                     summe =  WertTerm(); break;
    }
    
    z = cin.peek();
    while( z == '+' or z == '-' )
    {
        switch(z)
        {
            case '+': cin >> z; str+=z; summe += WertTerm(); break;
            case '-': cin >> z; str+=z; summe -= WertTerm(); break;
        }
        z = cin.peek();
    }
    return summe;    
}

double WertTerm()
{
    double produkt = WertFaktor();
    char z = cin.peek();
    while (z == '*' or z == '/')
    {
        switch (z)
        {
            case '*': cin >> z; str += z; produkt *= WertFaktor(); break;
            case '/': cin >> z; str += z; produkt /= WertFaktor(); break;
        }
        z = cin.peek();
    }
    return produkt;
}

double WertFaktor()
{
    // Ersten Operanden speichern.
    double pot = WertOperand();
    char z = cin.peek();
    // Falls ein Hoch-Zeichen auftritt...
    if (z == '^')
    {
        cin >> z;
        str += z;
        pot = pow(pot, WertOperand()); // ...Potenz zurückgeben...
    }
    // ...ansonsten hatten war das Ergebnis schon vorher richtig.
    return pot;
}

double WertOperand()
{
    double wert = 0.0;
    char z = cin.peek();
    if (z == '(')
    {
        cin >> z; str += z;
        wert = WertAusdruck(); 
        cin >> z;
        str += z;
        if (z != ')') fehler("Schließende Klammer erwartet.");
    }
    else if (isdigit(z))
    {
        cin >> wert;
        str += double2string(wert);
    }
    else if (isalpha(z))
    {
        // Den Funktionsnamen bestimmen und in func_name speichern.
        string func_name;
        do
        {
            cin >> z;
            func_name += z;
            z = cin.peek();
        }
        while (isalpha(z));
        
        str += func_name;
        cin >> z; str += z;
        
        if (z != '(') fehler("Oeffnende Klammer nach Funktionsname erwartet.");
        
        // Funktionsname fallunterscheiden. switch fuer string nicht moeglich.
        wert = WertAusdruck();
        if      (func_name == "sin")  wert = sin (wert);
        else if (func_name == "cos")  wert = cos (wert);
        else if (func_name == "exp")  wert = exp (wert);
        else if (func_name == "ln")   wert = log (wert);
        else if (func_name == "sqrt") wert = sqrt(wert); // Zusatz weil praktisch.
        else                          fehler("Funktion nicht unterstützt.");
        
        if (cin.peek() != ')') fehler("Schließende Klammer nach Funktionsargument erwartet.");
        cin >> z; str += z;
    }
    else fehler("Vorzeichenlose Zahl, öffnende Klammer oder Funktion erwartet.");
    return wert;
}

string double2string(double wert)
{
    ostringstream ostr;
    ostr << wert;
    return ostr.str();
}


void fehler(string const & msg)
{
    cout << "Fehler: " << msg << endl;
    if (str[str.size() - 1] == '\n') str.resize(str.size() - 1);
    cout << str << endl;
    for (size_t i = 0; i < str.size()-1; ++i) cout << ' ';
    cout << "~|~" << endl;
    exit(1);
}
