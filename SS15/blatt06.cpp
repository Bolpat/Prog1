/// COMPILE WITH: clang++ -std=c++11

#include <iostream>

#include <cmath>
#include <cerrno>
#include <cstring>

#include <vector>


using namespace std;

/// Berechnet den Binomialkoeffizienten n über k.
double binom(unsigned n, unsigned k)
{
    // Vereinfachen
    if (k > n / 2) k = n - k;
    // Spezialfall abfangen
    if (k == 0) return 1;
    
    // Auch wenn die Berechnung nur Zahlen dividiert,
    // wo der Divisor den Dividend teilt, wird double
    // verwendet, da ansonsten Überläufe entstehen können.
    double res = n - k + 1;
    for (unsigned i = 2; i <= k; ++i)
    {
        res *= n - k + i;
        res /= i;
    }
    return res;
}

/// Berechnet den Multinomialkoeffizienten n über k0 ... kn.
double multinom(unsigned n, vector<unsigned> k)
{
    // Überprüfen, ob wohldefiniert (*)
    unsigned sum = 0;
    for (unsigned l = 0; l < k.size(); ++l) sum += k[l];
    if (sum != n) { errno = EDOM; return 0.0; }
    
    double res = 1.0;

    for (unsigned l = 0; l < k.size(); ++l)
    {
        // Binomialkoeffizient bestimmen, wobei n hier bereits
        // n - k0 - ... darstellt.
        res *= binom(n, k[l]);
        // n entsprechend für die nächste Berechnung verringern.
        n -= k[l];
    }

    /* Andere Möglichkeit mit fortgeschrittenen vector-Techniken
     * und der Überlegung, dass die Reihenfolge der Verarbeitung
     * der ki unwichtig ist. Es wird von hinten nach vorn gearbeitet.
     * 
     * Vorteil: Benötigt keine Zählvariable etc.
    while (not k.empty()) // Solange Werte zu verarbeiten
    {
        // Bestimme Binomialkoeffizient mit letztem vector-Eintrag
        res *= binom(n, k.back());
        // n entsprechend für die nächste Berechnung verringern.
        n -= k.back();
        
        // Letzten Eintrag entfernen. Der vorletzte ist jetzt der letzte.
        k.pop_back();
    }
    */
    return res;
    // (*) andere Möglichkeit: statt n als Parameter
    //     an Ort und Stelle aus k0 + ... + kn berechnen.
}

int main_a()
{
    // Leider nicht gerade elegant, aber insgesamt die beste Variante:
    // Für jede Eingabe ein Vektor,...
    vector<unsigned> k_0(3);  k_0[0] =   2, k_0[1] =   3, k_0[2] =  3;
    vector<unsigned> k_1(3);  k_1[0] =  20, k_1[1] =  20, k_1[2] = 10;
    vector<unsigned> k_2(4);  k_2[0] =  25, k_2[1] =  11, k_2[2] = 37, k_2[3] = 27;
    vector<unsigned> k_3(5);  k_3[0] = 200, k_3[1] = 150, k_3[2] = 20, k_3[3] = 80, k_3[4] = 50;
    // ... die Vektoren zusammenfassen und eintsprechend n zufügen.
    vector<unsigned> ns(4);  ns[0] = 8, ns[1] = 50, ns[2] = 100, ns[3] = 500;
    vector< vector<unsigned> > ks(4);  ks[0] = k_0, ks[1] = k_1, ks[2] = k_2, ks[3] = k_3;
    
    // Ein Durchlauf für jede Eingabe.
    for (int i = 0; i < 4; ++i)
    {
        // Aufruf mit Fehlerbehandlung.
        errno = 0;
        // ns[i] ist das zu Aufgabe i gehörende n; ks[i] der Vektor.
        double result = multinom(ns[i], ks[i]);
        if (errno)  cout << "(" << i+1 << "): Aufruf mit unzulaessigen Parametern." << endl;
        else        cout << "(" << i+1 << "): " << result << endl;
    }
    
    return 0;
}

int main_b()
{
    // Einlesen
    cout << "x, y, z: " << flush;
    double x, y, z;
    cin >> x >> y >> z;
    
    cout << "n: " << flush;
    unsigned n;
    cin >> n;
    
    // Kontrollausgabe
    cout << "x = " << x << ",  y = " << y << ",  z = " << z << ",  n = " << n << endl;
    
    // Berechnung
    double sum = 0.0;
    for (unsigned k0 = 0; k0 <= n;    ++k0)
    for (unsigned k1 = 0; k1 <= n-k0; ++k1)
    {
        unsigned k2 = n - k0 - k1;
        //vector<unsigned> k = {k0, k1, k2}; /* (für C++11) */
        vector<unsigned> k(3);  k[0] = k0, k[1] = k1, k[2] = k2;
        sum += multinom(n, k) * pow(x, k0) * pow(y, k1) * pow(z, k2);
    }

    
    cout << "(" << x << " + " << y << " + " << z << ")^" << n << "  =  "
         << sum << "  =  "
         << pow(x + y + z, n)
         << endl;
    
    return 0;
}

int main()
{
    for(;;)
    {
        char aufg;
        cout << "Aufgabe: " << flush;
        cin  >> aufg;

        switch (aufg)
        {
            case 'a': return main_a();
            case 'b': return main_b();
            default:
                cout << "Es gibt nur Aufgabenteile a und b." << endl;
        }
    }
}
