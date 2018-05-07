#include <limits> // for numeric_limits
#include <cmath> // for sqrt, abs
#include <iostream> // for cout, cin, endl
#include <iomanip> // for setw, setprecision, fixed, scientific

using namespace std;

void instabile_iteration()
{
    double s = sqrt(2.0) / 2.0;
    int n = 2;
    
    // Ueberschriften fuer die Spalten
    cout<< ' ' << setw( 3) << right << "n"
        << ' ' << setw(10) << right << "Ecken"
        << ' ' << setw(25) << right << "Seitenlaenge"
        << ' ' << setw(20) << right << "Inneneck"
        << ' ' << setw(20) << right << "Auszeneck"
        << ' ' << '\n' << endl;
    while (s*s > numeric_limits<double>::epsilon() / 2.0)
    {
        // Innen- und Auszenumfang berechnen
        double u = ldexp(s, n);
        double U = u / sqrt(1.0 - s*s);
        // Eintraege fuer die aktuelle Zeile
        cout<< ' ' << setw( 3) << setprecision( 0) << fixed      << n
            << ' ' << setw(10) << setprecision( 0) << fixed      << ldexp(1.0, n)
            << ' ' << setw(25) << setprecision(15) << scientific << s
            << ' ' << setw(20) << setprecision(15) << fixed      << u
            << ' ' << setw(20) << setprecision(15) << fixed      << U
            << endl;
        // Seitenlaenge und Zaehler iterieren
        s = sqrt((1.0 - sqrt(1.0 - s*s)) / 2.0);
        n += 1;
    }
}

void stabile_iteration()
{
    double s = sqrt(2.0) / 2.0;
    int n = 2;
    
    // Ueberschriften fuer die Spalten
    cout<< ' ' << setw( 3) << right << "n"
        << ' ' << setw(10) << right << "Ecken"
        << ' ' << setw(25) << right << "Seitenlaenge"
        << ' ' << setw(20) << right << "Inneneck"
        << ' ' << setw(20) << right << "Auszeneck"
        << ' ' << '\n' << endl;
    while (s*s > numeric_limits<double>::epsilon() / 2.0)
    {
        // Innen- und Auszenumfang berechnen
        double u = ldexp(s, n);
        double U = u / sqrt(1.0 - s*s);
        // Eintraege fuer die aktuelle Zeile
        cout<< ' ' << setw( 3) << setprecision( 0) << fixed      << n
            << ' ' << setw(10) << setprecision( 0) << fixed      << ldexp(1.0, n)
            << ' ' << setw(25) << setprecision(15) << scientific << s
            << ' ' << setw(20) << setprecision(15) << fixed      << u
            << ' ' << setw(20) << setprecision(15) << fixed      << U
            << endl;
        // Seitenlaenge und Zaehler iterieren
        s /= sqrt(2.0 * (1.0 + sqrt(1.0 - s*s)));
        n += 1;
    }
}

int main()
{
    cout << "Instabile Iteration\n" << endl;
    instabile_iteration();
    cout << '\n' << endl;
    cout << "Stabile Iteration\n" << endl;
    stabile_iteration();
    return 0;
}