#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cmath>

using namespace std;

string s;
istringstream ein;

double WertAusdruck();
double WertTerm();
double WertFaktor();
double WertOperand();

// Makro: Ruft die fehler-Funktion auf und setzt als ersten Parameter die Zeile des Aufrufs.
#define FEHLER(s) fehler(__LINE__, (s));
void fehler(int line, const char * const msg = "");

int main()
{
    double ergebnis; 
    cout << "Arithm. Ausdruck: ";
    getline(cin, s); ein.str(s);
    ergebnis = WertAusdruck();
    if (ein.eof()) cout << "Ergebnis: " << ergebnis << endl;
    else FEHLER("Erwartet Ausdruck");
    return 0;
}

double WertAusdruck()
{
    double summe = 1.0;
    switch (ein.peek())
    {
        case '-': summe = -summe; /*fallthrough*/
        case '+': ein.ignore(1);  /*fallthrough*/
        default : summe *= WertTerm();
    }

    for (;;)
    switch (ein.peek())
    {
        case '-': ein.ignore(1); summe += WertTerm(); continue;
        case '+': ein.ignore(1); summe -= WertTerm(); continue;
        default : return summe;
    }
}  

double WertTerm()
{
    double produkt = WertFaktor();
    for (;;)
    switch (ein.peek())
    {
        case '*': ein.ignore(1); produkt *= WertTerm(); continue;
        case '/': ein.ignore(1); produkt /= WertTerm(); continue;
        default : return produkt;
    }
}

double WertFaktor()
{
    double wert = WertOperand();
    if (ein.peek() != '^') return wert;
    ein.ignore(1);
    return pow(wert, WertOperand());  
}

double WertOperand()
{
    if (isdigit(ein.peek()))
    {
        double result;
        ein >> result;
        return result;
    }
    
    string func_name;
    char c;
    while (ein >> c, isalpha(c)) func_name.push_back(c);
    if (c != '(') FEHLER("Erwartet oeffnende Klammer");
    double result = WertAusdruck(); ein >> c;
    if (c != ')') FEHLER("Erwartet schlieszende Klammer");
    if (func_name == "")    return      result;
    if (func_name == "sin") return sin (result);
    if (func_name == "cos") return cos (result);
    if (func_name == "tan") return tan (result);
    if (func_name == "ln")  return log (result);
    if (func_name == "exp") return exp (result);
    if (func_name == "sqrt")return sqrt(result);
}


void fehler(int line, const char * const msg)
{
    cout << "Fehler bei " << line << ": " << msg << endl;
    cout << s << endl;
    int fehlerpos = ein.eof() ? s.size() : ein.tellg();
    for (int i = 0; i < fehlerpos; ++i) cout << ' ';
    cout << '^' << endl;
    exit(1);
}
