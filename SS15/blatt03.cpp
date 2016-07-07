#include <iostream>
#include <iomanip>

#include <limits>

#include <cmath>

using namespace std;

int main()
{
    // a und b einlesen, eps (in Abhängigkeit von a) bestimmen.
    double a, b;
    cout << "a, b: " << flush;
    cin >> a >> b;
    if (a < 0 or b < 0 or b > a)
    {
        cout << "a >= b > 0 erforderlich!" << endl;
        return 0;
    }
    double const eps = a*a * numeric_limits<double>::epsilon() / 2;
    
    // Kontrollausgabe mit 4 Nachkommastellen im Festpunktformat.
    cout << fixed << setprecision(4)
         << "U(" << a << ", " << b << ")  =  ";
    
    // Definiere rekursive Hilfsvariablen.
    double c = a*a - b*b, // Anfangswert c0.
           w = c / 2,     // 2^(n-1) * c, Anfangswert ist c/2.
           U = a*a - w;   // a^2 - 1.Summand (Rest wird zukzessive abgezogen)
    int    n = 0;
    while (w > eps)
    {
        double a_ = a;    // a_ ist eine Hilfsvariable und sichert a...
        a = (a + b) / 2;  // ...das hier verändert wird...
        b = sqrt(a_ * b); // ...hier jedoch der alte Wert gebraucht wird.
        
        c *= c;
        c /= 16 * a * a;
        
        w = ldexp(c, n);
        U -= w;
        ++n;
    }
    
    U *= 2 * M_PI / a;
    cout << scientific << setprecision(15) << U << endl;
}

/*
int main()
{
    // a und b einlesen.
    double a, b;
    cout << "a, b: " << flush;
    cin >> a >> b;
    if (a < 0 or b < 0 or b > a)
    {
        cout << "a >= b > 0 erforderlich!" << endl;
        return 0;
    }
    double const eps = numeric_limits<double>::epsilon() / 2;
    
    // Kontrollausgabe mit 4 Nachkommastellen im Festpunktformat.
    cout << fixed << setprecision(4)
         << "U(" << a << ", " << b << ")  =  ";
    
    double   e = 1.0 - (b*b)/(a*a), // wurzel weglassen, e^2 durch e ersetzen.
             x = e / 4,
             U = 1.0;
    unsigned k = 2;
    while (x >= eps)
    {
        U -= x;
        ++k;
        x *= e * (k-2)*k;// / ((k+2)*(k+2));
        ++k;
        x /= k*k;
    }
    
    U *= 2 * M_PI * a;
    
    cout << scientific << setprecision(15) << U << endl;
}
*/