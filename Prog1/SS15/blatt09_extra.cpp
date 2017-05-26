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

// #include <vector>

/* Anmerkungen:
 * Ein istringstream ist ein Strom wie  cin, der jedoch
 * aus dem angegebenen string (und nicht vom Terminal)
 * liest.
 *
 * Mit in.get() wird aus einem Strom (z. B. cin)
 * ein Zeichen konsumiert und zurueckgegeben.
 * Wird der Rueckgabewert nicht verwendet, wird (im
 * Gegensatz zu  in >> z  wird aber keine Variable
 * benoetigt.
 *
 * Das Schluesselwort try leitet die strukturierte Fehler-
 * behandlung ein. Wo auch immer darin ein  throw  auftritt
 * - insbesondere in Unterfunktionen - wird die Ausfuehrung
 * beim entsprechenden  catch  fortgesetzt. Dabei steht in
 * der catch-Variable das bei throw angegebene Objekt.
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
    try
    {
        long double ergebnis = WertAusdruck(in);
        if (!in.eof())
            throw std::string("Ende der Eingabe erwartet");
        // Wenn die Ausfuehrung hier angelangt ist,
        // hat das Auswerten keine Fehler produziert.
        // Das Ergebnis ist valide.
        std::cout << std::setprecision(std::numeric_limits<long double>::digits10);
        std::cout << "Ergebnis: " << ergebnis << std::endl;
    }
    catch (std::string error_msg)
    {
        // Eine der throw-Anweisungen hat gefeuert und
        // die Fehlermeldung steht in error_msg.

        std::cout << "Fehler: " << error_msg << std::endl;

        unsigned g = std::min<unsigned>(in.tellg(), input.size());

        std::cout << input.substr(0, g) << ' ' << input.substr(g) << std::endl;
        std::cout << std::string(g, ' ') << '^' << std::endl;
    }
    return 0;
}

// Falls das naechste Zeichen von in expected ist, wird es heruntergenommen.
// Gibt true zurueck gdw. das naechste Zeichen expected war.
bool expect(istream & in, char expected)
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
        default : return sum;
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
        default : return produkt;
    }
}

long double WertFaktor(istream & in)
{
    /* // Linksassoziative Potenz: a^b^c == (a^b)^c
    long double pot = WertOperand(in);
    while (expect(in, '^'))
    {
        pot = pow(pot, WertOperand(in));
    }
    return pot;
    */

    // Rechtsassoziative Potenz: a^b^c == a^(b^c)
    // Imperativ
    /*
    std::vector<long double> exps(1, WertOperand(in));
    while (expect(in, '^'))
        exps.push_back(WertOperand(in));

    long double pot = exps.back();
    while (exps.pop_back(), !exps.empty())
        pot = pow(exps.back(), pot);
    return pot;
    */
    // Rekursiv
    long double pot = WertOperand(in);
    return expect(in, '^') ? pow(pot, WertFaktor(in)) : pot;
}

long double id(long double x) { return x; }

long double WertOperand(istream & in)
{
    // Zahl:
    if (std::isdigit(in.peek()))
    {
        long double wert;
        in >> wert;
        return wert;
    }

    // Funktion oder Klammerausdruck:
    std::string func_name = "";
    while (std::isalpha(in.peek()))
        func_name.push_back(in.get());
    
    // Noch besser waere natuerlich eine Hilfsfunktion H, die die Zeichenketten auf die
    // entsprechende Funktion (id, sin, ...) abbildet. Damit ist sofort mittels
    // return H(func_name)(wert)  alles getan; H(func_name)  ist die Funktion, in die
    // sofort das Argument eingesetzt wird. Dazu fehlen noch die Mittel.
    long double (*f)(long double);
    if      (func_name == ""    ) f = id;
    else if (func_name == "sin" ) f = sin;
    else if (func_name == "cos" ) f = cos;
    else if (func_name == "tan" ) f = tan;
    else if (func_name == "atan") f = atan;
    else if (func_name == "exp" ) f = exp;
    else if (func_name == "ln"  ) f = log;
    else if (func_name == "sqrt") f = sqrt;
    else                          throw "Funktion '"+func_name+"' nicht unterstuetzt.";
    
    if (!expect(in, '('))
        throw func_name.empty()
            ? std::string("Unvollstaendiger Ausdruck; oeffnende Klammer '(' oder Zahl erwartet.")
            : std::string("Oeffnende Klammer '(' nach Funktion erwartet.");

    long double wert = WertAusdruck(in);
    if (!expect(in, ')')) throw std::string("Schlieszende Klammer ')' nach Funktionsargument erwartet.");
    return f(wert);
}
