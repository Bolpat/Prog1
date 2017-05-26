#include <cmath>
#include <utility> // std::pair
#include <limits>

#include <iostream>
#include <iomanip>

using namespace std;

const double eps = numeric_limits<double>::epsilon()/2;

double umfang_arithgeo(double a0, double b0) 
{
    pair<double, double> ab(a0, b0);
    double& a = ab.first;
    double& b = ab.second;
    
    double c = a*a - b*b;
    
    double s = 0;
    for (int n = 0; ldexp(c, n-1) > eps * (a0*a0); ++n)
    {
        s += ldexp(c, n-1);
        ab = make_pair( (a+b)/2, sqrt(a*b) );
        c = c*c / (16*(a*a));
    }
    
    return 2*M_PI * (a0*a0 - s) / a;
}

int main()
{
    double a0, b0;
    cout << "a b: " << flush;
    cin >> a0 >> b0;
    
    // a0 und b0 ausgeben:
    cout << fixed << setprecision(4);
    cout
        << "a = " << a0 << endl
        << "b = " << b0 << endl;
    
    // Umfaenge berechnen und ausgeben:
    cout << scientific << setprecision(15);
    cout << "umfang_arithgeo = " << umfang_arithgeo(a0, b0) << endl;

    return 0;
}
