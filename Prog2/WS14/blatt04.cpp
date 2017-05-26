// Entwurf zu Aufgabe 4, WS 2014/2015
// Programmieren II fuer Mathematiker

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>


using namespace std;

const unsigned N = 2000;
const double PI = 3.141592653589793238462643383279502884197169399375105820974944;


class TEntwicklung
{
  private:
    vector<double> c;
  public:
    TEntwicklung(double f(double), unsigned n)
    {
        ++n;
        double c0 = f(cos(PI / (2.0*n)));
        for (unsigned j = 1; j < n; ++j)
        {
            c0 += f(cos((2.0*j + 1.0)/(2.0*n) * PI));
        }
        
        c.push_back(c0 / n);
        
        for (unsigned k = 1; k < n; ++k)
        {  
            double ck = f(cos(PI/(2.0*n))) * cos(k*PI / (2.0*n));
            
            for (unsigned j = 1; j < n; ++j)
            {
                ck += f(cos((2.0*j + 1.0)/(2.0*n) * PI)) * cos((2.0*j + 1)/(2.0*n) * k * PI);
            }
            c.push_back(2 * ck / n);
        }
        /*
      double c0 = f(cos(PI / (2.0*n + 2.0)));
      for (unsigned j = 1; j <= n; ++j)
      {
        c0 += f(cos((2.0*j + 1.0)/(2.0*n + 2.0) * PI));
      }
      
      c.push_back(c0 / (n+1));
      
      for (unsigned k = 1; k <= n; ++k)
      {  
        double ck = f(cos(PI/(2.0*n + 2.0))) * cos(k*PI / (2.0*n + 2.0));
        
        for (unsigned j = 1; j <= n; ++j)
        {
          ck += f(cos((2.0*j + 1.0)/(2.0*n + 2.0) * PI)) * cos((2.0*j + 1)/(2.0*n + 2.0) * k * PI);
        }
        
        c.push_back(2 * ck / (n+1));
      }
        */
    } 

    double operator()(double x)
    {
      unsigned n = c.size() - 1;
      double skm2 = c[n];
      double skm1 = skm2 * x + c[n-1];
      double sk;
      
      for (unsigned k = 2; k <= n; ++k)
      {
        sk = 2 * x * skm1 - skm2 - c[n-k+1] * x + c[n-k];
        // Für die nächste Rechnung:
        skm2 = skm1;
        skm1 = sk;
      }
      
      return sk;
    }

    friend ostream& operator<<(ostream& stream, TEntwicklung p)
    {
    // Ausgabe: Tschebyshew-Koeff. mit Index falls Betrag>eps
      for (unsigned i = 0; i < p.c.size(); ++i)
        if (abs(p.c[i]) > 1e-12)
          stream << setw(5) << i << ": " << p.c[i] << endl;
      
      return stream;
    }
};

/* constexpr */
inline double f(double x) { return ((x - 3.0)*x + 2.0)*x - 5.0; }

inline double g(double x) { return 2.0 + x*cos(x); }

inline double h(double x) { return exp(-4*x*x) * sin(4.0*x); }
/* ~constexpr */

double dist(double f(double), TEntwicklung p)
{
  // max|f(x_i)-p.wert(x_i)| berechnen
  double erg = abs(f(-1) - p(-1));
  const double h = 2.0 / N;
  for (unsigned i = 1; i <= N; ++i)
  {
    double nerg = abs(f(i*h - 1) - p(i*h - 1));
    if (nerg > erg) erg = nerg;
  }
  return erg;
}

int main() 
{
  TEntwicklung p(f,  3),
               q(g, 30),
               r(h, 50);

  cout << p << "dist(f, p) = " << dist(f, p) << endl
       << q << "dist(g, q) = " << dist(g, q) << endl
       << r << "dist(h, r) = " << dist(h, r) << endl;
}

