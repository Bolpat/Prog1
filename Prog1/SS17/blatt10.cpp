#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

/// Zerlegt mit b = frexp4(a, n), sodass a == b * 4^n.
/// Für a != 0 ist 0.25 <= abs(b) < 1.
double frexp4(double a, int & n)
{
    a = frexp(a, &n);
    if (n % 2) // n ungerade
    {
        n += 1;
        a /= 2.0;
    }
    n /= 2;
    return a;
}

/// Berechnet die Quadratwurzel ohne Gleitpunkt-Division.
double sqrt2(double a)
{
    const double eps = 8 * numeric_limits<double>::epsilon();
    int n;
    double b = frexp4(a, n);
    
    double x = 0.25, x_alt;
    do
    {
        x_alt = x;
        x *= 0.5 * (3.0 - b * x * x);
    }
    while (abs(x - x_alt) > eps * 0.25);
    
    return ldexp(b * x, n);
}

int main()
{
    cout << "Divisionsfreies Radizieren\n" << endl;
    double a;
    cout << "a: ";
    cin >> a;
    
    cout.precision(15);
    cout << "STL-sqrt(" << a << ") = " << sqrt (a) << endl;
    cout << "DVF-sqrt(" << a << ") = " << sqrt2(a) << endl;
}