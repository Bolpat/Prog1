#include <iostream>
#include <iomanip>

#include <limits>
#include <cmath>


#include <cstring>
#include <cerrno>

using namespace std;

/// Konstante, die die Maschinengenauigkeit beschreibt.
double const EPS = numeric_limits<double>::epsilon() / 2;

int main_a()
{
    // a einlesen.
    double a;
    cout << "a: " << flush;
    cin >> a;
    
    // Wertebereich überprüfen.
    if (not (a > 0))
    {
        cout << "a > 0 erwartet!" << endl;
        return 0;
    }
    
    // Startwerte festlegen.
    double  x  = sqrt(a), // x = root(a, 2^n) = sqrt(a), da n = 0.
            l1 = a - 1.0, // Logarithmus 1. Formel
            l2 = a - 1.0; // Logarithmus 2. Formel
    double  y  = x - 1.0; // Praktische Abkürzung.
    
    // Präzises Ausgabeformat festlegen; wirt daueraft.
    cout << setprecision(15) << fixed;

    // für alle n ist x = (2^n)-te Wurzel von a.
    // In jedem Schritt wird n erhöht und x und y erhalten ihre entsprechenden
    // Werte für den nächsten Durchlauf.
    for (unsigned n = 1;  abs(y) > EPS;  ++n, x = sqrt(x), y = x - 1.0)
    {
        l1 = ldexp(y, n);
        
        l2 *= 2.0 / (1.0 + x);

        cout << setw(20) << l1
             << setw(20) << l2
             << setw(20) << log(a) << '\n';
    }
    
    // Ausgabe der Ergebnisse.
    cout << "Näherung1: " << l1 << '\n'
         << "Näherung2: " << l2 << '\n'
         << "log(a)  =  " << log(a) << endl;
    
    return 0;
}

double mylog(double a)
{
    // Spezialfälle abfangen.
    if (a < 0.0 or a != a)
    {
        errno = EDOM; // EDOM = Paramter außerhalb des Wertebreichs.
        return numeric_limits<double>::quiet_NaN();
    }
    if (a == 0.0) // besser a < eps (wobei eps klein, z. B. M.-Genauigkeit); (*)
    {
        errno = ERANGE; // ERANGE = Ergebniswert außerhalb des Wertebreichs.
        return -numeric_limits<double>::infinity();
    }
    
    // Berechnung.
    double x = a--; // Setze x = a - 1, dann verringere a um 1.
    
    // Solange die Bedingung gilt:
    //  - Ziehe die Wurzel aus x mittels x = sqrt(x).
    //  - Multipliziere a mit 2 und teile durch x (nach dem Ziehen der Wurze).
    while (abs(x - 1.0) > EPS) a *= 2.0 / (1.0 + (x = sqrt(x)));
    
    return a;
    
    // (*) abs nicht notwendig, da a >= 0 aus obiger Bedingung folgt.
    // Es gibt Zahlen, die so klein sind, dass ihr Inverses nicht darstellbar ist.
}

int main_b()
{
    // a einlesen.
    double a;
    cout << "a: " << flush;
    cin >> a;
    
    errno = 0;              // Alte Fehler ignoreren.
    double l = mylog(a);    // Berechnung durchführen.
    if (errno)         // Auf Fehler prüfen.
    {
        // Fehlermedlung ausgeben.
        cout << "Fehler aufgetreten. Details: " << strerror(errno) << endl;
    }
    else
    {
        // Ergebnis präsentieren.
        cout << setprecision(17) << fixed;
        cout << "Näherung:  " << l      << endl;
        cout << "log(a)  =  " << log(a) << endl;
    }
    
    return 0;
}

int main()
{
    for(;;)
    {
        cout << "Teilaufgabe: " << flush;
        char aufg;
        cin >> aufg;
        
        // Entsprechend Teilaufgabe in Unterfunktion weitermachen.
        switch (aufg)
        {
            case 'a': return main_a();
            case 'b': return main_b();
            default:
                cout << "Es gibt nur Aufgabenteil a und b." << endl;
        }
    }
}
