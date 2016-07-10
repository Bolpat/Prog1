/** Diese Lösung ist für diejenigen, die schon etwas Ahnung von Klassen
  * und Operatorüberladung haben. 
  * Hier wird die Möglichkeit Polynome allgemein zu definieren und
  * auszuwerten zur Verfügung gestellt.
  * 
  * Diese Lösung und die andere sind nicht zueinander kompatibel.
  * 
  * Es gibt (noch aufwändigere) Lösungen, die beide Ansätze, also
  * eingebaute Funktionen mit Ableitung und Klassen abdecken können.
  * Dies wird nur genannt, um die Existenz klarzustellen. Wie sie aus-
  * sieht ist unerheblich und mit gegenwärzigen Mitteln nicht einfach
  * zu erreichen.
  */

#include <iostream>
#include <vector>

#include <cmath>


using namespace std;

class poly
{
    vector<double> a;

public:
    poly(vector<double> const & a) : a(a) { }

    double operator()(double x, double & deriv)
    {
        deriv = 0;
        if (a.empty()) return 0;
        
        double result = 0;
        for (unsigned i = a.size() - 1; ; --i)
        {
            result *= x;
            result += a[i];
            // hier abbrechen, da für Ableitung der letzte Term fehlt.
            if (i == 0) return result;
            deriv *= x;
            deriv += i * a[i];
        }
    }
};

void newton_maehly(double           x0,         // Startwert der Suche
                   unsigned         n,          // Anzahl der zu suchenden Nulltstellen
                   unsigned         itmax,      // Iterationsschranke
                   double           eps,        // Minimaländerung für Abbruch
                   vector<double> & xs,         // Ergebnisvektor
                   poly             p)          // Polynomfunktion
{
    xs.clear();
    for (unsigned m = 0; m < n; ++m)
    {
        double xk, xkp1 = x0;
        unsigned k = itmax;
        do
        {
            if (--k == 0) return;
            xk = xkp1;
            double p_xk;
            double pxk = p(xk, p_xk);
            for (unsigned i = 0; i < m; ++i) p_xk -= pxk / (xk - xs[i]);
            xkp1 -= pxk / p_xk;
        }
        while ( abs(xkp1 - xk) > eps * max(1.0, abs(xkp1)) );
        xs.push_back(xkp1);
    }
}

void ausgabe(int n, vector<double> const & xs)
{
    int m = xs.size();
    cout << "Nullstellen gefunden: " << m << '\n'
         << "Nullstellen gesucht:  " << n << endl;
    // Nullstellen ausgeben
    for (int i = 0; i < m; ++i) cout << xs[i] << endl;
}

int main()
{
    unsigned n;
    cout << "Polynomfunktion eingeben:" << endl;
    cout << "Grad: " << flush;
    cin >> n;
    
    vector<double> a(n + 1);
    for (int i = n; i >= 0; --i)
    {
        cout << "a_" << i << ": " << flush;
        cin >> a[i];
    }
    
    double x0;
    cout << "x0: " << flush;
    cin >> x0;
    
    vector<double> xs;
    newton_maehly(x0, n, 20, 1e-12, xs, poly(a));
    ausgabe(n, xs);
}
