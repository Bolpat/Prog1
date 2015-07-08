#include <iostream>

#include <vector>
#include <cmath>

using namespace std;

/// p1  =  x^3 - 6 x^2 + 11 x - 6
/// p1' =  3 x^2 - 12 x + 11
double p1(double x, double & deriv)
{
    deriv = (3 * x - 12) * x + 11;
    return ((x - 6) * x + 11) * x - 6;
}

/// p2  =  x^3 + 3 x^2 - 1
/// p2' =  3 x^2 + 6 x
double p2(double x, double & deriv)
{
    deriv = (3 * x + 6) * x;
    return (x + 3) * x*x - 1;
}

/// p3  =  x^5 - 2 x^4 - 13 x^3 + 14 x^2 + 24 x - 1
/// p3' =  5 x^4 - 8 x^3 - 36 x^2 + 28 x + 24
double p3(double x, double & deriv)
{
    deriv = (((5 * x - 8) * x - 36) * x + 28) * x + 24;
    return ((((x - 3) * x - 13) * x + 14) * x + 24) * x - 1;
}

// Eine allgemeine Funktion zu schreiben, die aus gegebenen Koeffizienten
// eine Funktion der obigen Art baut, ist in C++ zwar möglich,
// ist aber mit den bisherigen Mitteln nicht umsetzbar.

/// Bestimmt Nullstellen mit Newton-Maehly-Verfahren.
/// Für Variablen mit _: Lies z. B. p_xk: "p-Strich xk".
void newton_maehly(double           x0,         // Startwert der Suche
                   int              n,          // Anzahl der zu suchenden Nulltstellen
                   int              itmax,      // Iterationsschranke
                   double           eps,        // Minimaländerung für Abbruch
                   vector<double> & xs,         // Ergebnisvektor
                   double           p(double, double &)) // Polynomfunktion
{
    xs.clear(); // Vektor löschen
    
    // Es wird die (m + 1)-te Nullstelle gesucht.
    for (int m = 0; m < n; ++m)
    {
        double xk,          // x(k+1): Variablen muss für Vergleich...
               xkp1 = x0;   // ...außerhalb der Schleife deklariert werden.
                            // Startwert für xk ist das gegebene x0.
        int k = 0;          // Zähler für itmax.
        do
        {
            xk = xkp1;      // Alten Wert zuweisen; im Folgenden xk neu bestimmen.
            
            double p_xk;    // p'(xk): Da als Referenzparameter eingesetzt, vorher zu deklarieren
            double pxk = p(xk, p_xk); // p(xk) und p'(kx) bestimmen.
            
            // Sukzessives Abziehen. Bisherige Nullstellen sind im Vektor xs.
            for (int i = 0; i < m; ++i) p_xk -= pxk / (xk - xs[i]);
            // x(k+1) = xk von oben. Wird hier aktualisiert.
            xkp1 -= pxk / p_xk;
        }
        while ( ++k < itmax  and  abs(xkp1 - xk) > eps * max(1.0, abs(xkp1)) );
        
        // Ermittelte Näherung an die Nullstelle hinten an den Vektor anhängen.
        xs.push_back(xkp1);
    }
}


void ausgabe(int n, vector<double> const & xs)
{
    int m = xs.size();
    cout << "Nullstellen gefunden: " << m << '\n'
         << "Nullstellen gesucht:  " << n << endl;
    // Nullstellen ausgeben
    for (int i = 0; i < m; ++i) cout << xs[i] << '\n';
    cout << endl;
}


int main()
{
    // Eingabevariablen werden entsprechend der
    // ausgewählten Teilaufgabe später gesetzt.
    double  x0;
    int     n;
    double (*p)(double, double &); // Variable (ein Funktionszeiger) für das Polynom.
    
    // Ergebnisvektor.
    vector<double> xs;
    
    
  ask_again: // Falls der Benutzer eine ungültige Aufgabennummer wählt.
    char antw;
    cout << "Aufgabennummer: " << flush;
    cin  >> antw;
    
    switch (antw)
    {
        case 'a':
            x0 = 4.0;
            n = 3;
            p = p1;
            break;
        case 'b':
            x0 = 1.0;
            n = 3;
            p = p2;
            break;
        case 'c':
            x0 = 5.0;
            n = 5;
            p = p3;
            break;
        default:
            goto ask_again;
    }
    
    newton_maehly(x0, n, 20, 1e-12, xs, p);
    ausgabe(n, xs); 
    
    return 0;
}
