// -stdlib=libc++ -lc++abi
#include <iostream>
#include <iomanip>

#include <limits>
#include <algorithm>

#include <string>
#include <sstream>

#include <cmath>
#include <cctype>
#include <cstdlib>

#include <vector>

/* Anmerkungen:
 * Ein  istringstream  ist ein Strom wie  cin, der jedoch
 * aus dem angegebenen string (und nicht vom Terminal)
 * liest.
 * 
 * Mit  in.get()  wird aus einem Strom (z. B. cin)
 * ein Zeichen konsumiert und zurückgegeben.
 * Wird der Rückgabewert nicht verwendet, muss (im
 * Gegensatz zu  in >> z  wird aber keine Variable
 * benötigt.
 * 
 * Das Schlüsselwort  try  leitet die strukturierte Fehler-
 * behandlung ein. Wo auch immer darin ein  throw  auftritt
 * - insbesondere in Unterfunktionen - wird die Ausführung
 * beim entsprechenden  catch  fortgesetzt. Dabei steht in
 * der  catch-Variable das bei  throw  angegebene Objekt.
 */

using std::istream;

long double WertAusdruck(istream &);
long double WertTerm    (istream &);
long double WertFaktor  (istream &);
long double WertOperand (istream &);

int main()
{
    std::cout << "Arithmetischer Ausdruck: " << std::flush;
    std::string input; std::cin >> input;
    std::istringstream in(input);
    long double ergebnis;
    try
    {
        ergebnis = WertAusdruck(in);
        // Wenn die Ausführung hier angelangt ist,
        // hat das Auswerten keine Fehler produziert.
        // Das Ergebnis ist valide.
        std::cout << std::setprecision(std::numeric_limits<long double>::digits10);
        std::cout << "Ergebnis: " << ergebnis << std::endl;
    }
    catch (char const * error_msg)
    {
        // Eine der  throw-Anweisungen hat gefeuert und
        // die Fehlermeldung steht in  error_msg.
        
        std::cout << "Fehler: " << error_msg << std::endl;

        size_t g = std::min<size_t>(in.tellg(), input.size());
        
        std::cout << input.substr(0, g) << ' ' << input.substr(g) << std::endl;
        std::cout << std::string(g, ' ') << '^' << std::endl;
    }
    catch (...)
    {
        // Irgendeine andere (nicht hier vorkommende)  throw
        // hat einen Fehler produziert. Diese kann z. B. von
        // Bibliotheksfunktionen etc. kommen.
        std::cout << "Unbekannter Fehler aufgetreten." << std::endl;
    }
    return 0;
}

inline
bool peek_test(istream & in, char expected)
{
    bool result = (in.peek() == expected);
    if (result) in.get();
    return result;
}

long double WertAusdruck(istream & in)
{
    long double sum = 1.0;
    switch (in.peek())
    {
        case '-': sum = -sum;   //[[clang::fallthrough]];
        case '+': in.get();     //[[clang::fallthrough]];
        default : sum *= WertTerm(in); 
    }
    for (;;)
    switch(in.peek())
    {
        case '+': in.get(); sum += WertTerm(in); continue;
        case '-': in.get(); sum -= WertTerm(in); continue;
        default:     
        return sum;
    }
}

long double WertTerm(istream & in)
{
    long double produkt = WertFaktor(in);
    for (;;)
    switch (in.peek())
    {
        case '*': in.get(); produkt *= WertFaktor(in); continue;
        case '/': in.get(); produkt /= WertFaktor(in); continue;
        default:     return produkt;
    }
}

long double WertFaktor(istream & in)
{
    /* // Linksassoziative Potenz: a^b^c == (a^b)^c
    long double pot = WertOperand(in);
    while (peek_test(in, '^'))
    {
        pot = pow(pot, WertOperand(in));
    }
    return pot;
    */
    
    // Rechtsassoziative Potenz: a^b^c == a^(b^c)
    long double pot = WertOperand(in);
    if (peek_test(in, '^'))
    {
        std::vector<long double> exps(1, pot);
        do
            exps.push_back(WertOperand(in));
        while (peek_test(in, '^'));
        
        pot = exps.back();
        exps.pop_back();
        while (not exps.empty())
        {
            pot = pow(exps.back(), pot);
            exps.pop_back();
        }
    }
    return pot;
}

long double WertOperand(istream & in)
{
    if (std::isdigit(in.peek()))
    {
        long double wert;
        in >> wert;
        return wert;
    }

    std::string func_name = "";
    while (std::isalpha(in.peek())) func_name.push_back(in.get());
    if (not peek_test(in, '('))
        throw func_name.empty()
            ? "Öffnende Klammer '(' oder Zahl erwartet."
            : "Öffnende Klammer '(' erwartet.";
    
    long double wert = WertAusdruck(in);
    if (not peek_test(in, ')')) throw "Schließende Klammer ')' nach Funktionsargument erwartet.";

    // Noch besser wäre natürlich eine 'Hilfsfunktion' H, die die Zeichenketten auf die
    // entsprechende Funktion (id, sin, ...) abbildet. Damit ist sofort mittels
    //  return H(func_name)(wert)  alles getan; H(func_name)  ist die Funktion, in die
    // sofort das Argument eingesetzt wird. Dazu fehlen noch die Mittel.
    if      (func_name == ""    ) return      wert;
    else if (func_name == "sin" ) return sin (wert);
    else if (func_name == "cos" ) return cos (wert);
    else if (func_name == "tan" ) return tan (wert);
    else if (func_name == "atan") return atan(wert);
    else if (func_name == "exp" ) return exp (wert);
    else if (func_name == "ln"  ) return log (wert);
    else if (func_name == "sqrt") return sqrt(wert);
    else                          throw "Funktion nicht unterstützt.";
}
