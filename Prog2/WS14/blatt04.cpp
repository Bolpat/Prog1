// Entwurf zu Aufgabe 4, WS 2014/2015
// Programmieren II fuer Mathematiker

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>


using namespace std;

const unsigned N = 2000;
const double eps = 1e-12;

class TEntwicklung
{
  private:
    vector<double> c;
  public:
    TEntwicklung(double f(double), unsigned n)
        : c(n + 1)
    {
        n += 1;
        
        for (unsigned j = 0; j < n; ++j)
        {
            double cos_arg = M_PI * (2*j + 1) / (2 * n);
            c[0] += f(cos(cos_arg));
        }
        c[0] /= n;
        
        for (unsigned k = 1; k < n; ++k)
        {
            for (unsigned j = 0; j < n; ++j)
            {
                double cos_arg = M_PI * (2*j + 1) / (2 * n);
                c[k] += f(cos(cos_arg)) * cos(cos_arg * k);
            }
            c[k] = 2 * c[k] / n;
        }
    }

    double operator()(double x) const
    {
        if (c.size() == 0) return 0;
        if (c.size() == 1) return c[0];
        
        const unsigned n = c.size() - 1;
        double skm2 = c[n];
        double skm1 = skm2 * x + c[n-1];
        double sk = 0;
        for (unsigned k = 2; k <= n; ++k)
        {
            sk = 2 * x * skm1 - skm2 - c[n-k+1] * x + c[n-k];
            // Für die nächste Rechnung:
            skm2 = skm1;
            skm1 = sk;
        }
        
        return skm1;
    }

    friend ostream& operator<<(ostream& stream, TEntwicklung const & p)
    {
        // Ausgabe: Tschebyshew-Koeff. mit Index falls Betrag>eps
        for (unsigned i = 0; i < p.c.size(); ++i)
            if (abs(p.c[i]) > eps)
                stream << "[" << setw(2) << i << "] " << p.c[i] << endl;
        return stream;
    }
};

double f(double x) { return ((x - 3.0)*x + 2.0)*x - 5.0; }

double g(double x) { return 2.0 + x*cos(x); }

double h(double x) { return exp(-4*x*x) * sin(4.0*x); }

double dist(double f(double), TEntwicklung const & p)
{
    // max_i |f(x_i) - p(x_i)| berechnen
    double d = 0;
    const double h = 2.0 / N;
    for (unsigned i = 0; i <= N; ++i)
    {
        const double x = i*h - 1.0;
        d = max(d, abs(f(x) - p(x)));
    }
    return d;
}

int main() 
{
  TEntwicklung
    p(f,  3),
    q(g, 30),
    r(h, 50);

    cout << p << "dist(f, p) = " << dist(f, p) << endl;
    cout << q << "dist(g, q) = " << dist(g, q) << endl;
    cout << r << "dist(h, r) = " << dist(h, r) << endl;
}

