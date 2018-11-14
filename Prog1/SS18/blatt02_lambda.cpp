// compile with -std=c++11

#include <limits>
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

// iteration nimmt eine Funktion vom Typ (double &) --> void als Parameter
void pi_approximation(double iteration(double))
{
    double s = sqrt(2.0) / 2.0;
    int n = 2;
    
    cout<< ' ' << setw( 3) << "n "
        << ' ' << setw(10) << "Ecken  "
        << ' ' << setw(23) << "Seitenlaenge    "
        << ' ' << setw(18) << "Inneneck    "
        << ' ' << setw(18) << "Auszeneck    "
        << ' ' << '\n' << endl;
    while (s*s > numeric_limits<double>::epsilon() / 2.0)
    {
        double u = ldexp(s, n);
        double U = u / sqrt(1.0 - s*s);

        cout<< ' ' << setw( 3) << setprecision( 0) << fixed      << n
            << ' ' << setw(10) << setprecision( 0) << fixed      << ldexp(1.0, n)
            << ' ' << setw(23) << setprecision(15) << scientific << s
            << ' ' << setw(18) << setprecision(15) << fixed      << u
            << ' ' << setw(18) << setprecision(15) << fixed      << U
            << endl;
        // Hier wird die Parameter-Funktion aufgerufen.
        // Sie berechnet s_(n+1) aus s_n in einer vom Aufrufer vorgegebenen Weise.
        s = iteration(s);
        n += 1;
    }
}

int main()
{
    cout << "Instabile Iteration\n" << endl;
    // Die Funktion pi_approximation wird mit einer sogenannten Lambda-Funktion aufgerufen.
    // Im Wesentlichen ist eine Lambda-Funktion eine Funktion ohne Namen.
    // "[]" gehört zur Syntax einer Lambda-Funktion in C++;
    // danach folgen die Parameter, hier eine double-Zahl.
    // Schlussendlich folgt ein Rumpf, der hier nur aus einer return-Anweisung besteht.
    // Der Vorteil dieser Herangehensweise ist offensichtlich: pi_approximation braucht nur
    // einmal implementiert werden und man kann die Iteration einfach austauschen.
    pi_approximation([](double s) { return sqrt((1.0 - sqrt(1.0 - s*s)) / 2.0); });
    cout << '\n' << endl;
    cout << "Stabile Iteration\n" << endl;
    pi_approximation([](double s) { return s / sqrt(2.0 * (1.0 + sqrt(1.0 - s*s))); });
    return 0;
}