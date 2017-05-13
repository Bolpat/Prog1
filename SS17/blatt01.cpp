#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double sin_Fkt(double x)
{
  double s = x, a = x;
  for (int k = 1; abs(a) > 1e-15*abs(s); ++k)
    s += (a *= -(x*x) / ((2*k + 1)*2*k));
  return s;
}

int main()
{
  double x;
  cout << "x:";
  cin >> x;
  cout << "x = " << setprecision(4) << x << endl;
  
  cout << "sin(x) = " << setprecision(15) << sin(x) << endl;
  
  // Faktor vor dem Sinus.
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
