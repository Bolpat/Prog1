#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double sin_Fkt(double x)
{
  double a = x, s = a; // a_0 = x, s_0 = a_0
  // for k = 1, while abs(a) > eps * abs(s):
  //   let s_(k+1) = s_k + a_(k + 1) where
  //       a_(k+1) = a_k * (-x*x) / (2*k + 1) / (2*k)
  for (int k = 1; abs(a) > 1e-15*abs(s); ++k)
    s += (a *= -(x*x) / (2*k + 1) / (2*k);
  return s;
}

int main()
{
  // x einlesen und zur Kontrolle mit 4 Dezimalstellen ausgeben.
  double x;
  cout << "x:";
  cin >> x;
  cout << "x = " << setprecision(4) << x << endl;
  
  // Den Sinus von x mit der Standardbibliotheksfunktion mit 15 Dezimalstellen ausgeben.
  cout << "sin(x) = " << setprecision(15) << sin(x) << endl;
  
  // Faktor, der am Ende vor dem Sinus steht.
  // Am Ende ist a == 1.0 oder a == -1.0.
  double a = 1.0;
  
  if (x < 0.0)
    x = -x, a = -a;
  x = fmod(x, 2*M_PI);
  if (M_PI <= x) /* x < 2*M_PI */
    x = x - M_PI, a = -a;
  if (M_PI/2 <= x) // x < M_PI
    x = M_PI-x;
  if (M_PI/4 <= x) // x < M_PI/2
  {
    // Wegen abs(a) == 1.0 gilt a*a == 1.0
    double sn = sin_Fkt(M_PI/2 - x);
    cout << "Reihe  = " << sqrt(1 - sn*sn) << endl;
  }
  else
  {
    cout << "Reihe  = " << setprecision(15) << a*sin_Fkt(x) << endl;
  }
}
