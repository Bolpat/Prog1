// Entwurf zu Aufgabe 2, WS 2014/2015
// Programmieren II fuer Mathematiker

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class Polynom
{ 
private:
    vector<T> a;
    
    inline void rmZeros()
    {
        int j = a.size();
        while (a[--j] == 0) a.pop_back();
    }
    
public:
    Polynom(vector<T> v = vector<T>()) : a(v) { rmZeros(); }
    Polynom(unsigned grad, T val = 1) : a(grad+1) { a[grad] = val; }
    
    int grad() const { return a.size() - 1; }
    
    Polynom deriv() const
    {
        if (a.size() == 0) return Polynom();

        Polynom ps;
        for (int i = 1; i < a.size(); ++i)
            ps.a.push_back(i * a[i]);
        return ps;
    }
    
    friend Polynom operator+(Polynom const & p, Polynom const & q) 
    {
        Polynom         r = (p.grad() > q.grad()) ? p : q;
        Polynom const & s = (p.grad() > q.grad()) ? q : p;

        // r ist die Kopie des "größeren" Polynoms,
        // s ist eine Referenz auf das kleinere.

        for (int i = 0; i < s.a.size(); ++i) r.a[i] += s.a[i];

        r.rmZeros();

        return r;
    }
    
    friend Polynom operator-(Polynom const & p, Polynom const & q) 
    {
        Polynom         r = (p.grad() > q.grad()) ? p : q;
        Polynom const & s = (p.grad() > q.grad()) ? q : p;
        
        // r ist die Kopie des "größeren" Polynoms,
        // s ist eine Referenz auf das kleinere.
            
        for (int i = 0; i < s.a.size(); ++i) r.a[i] -= s.a[i];
        
        r.rmZeros();
        
        return r;
    }
    
    friend Polynom operator*(Polynom const & p, Polynom const & q)
    {
        const int m   = p.grad(),
                  n   = q.grad(),
                  szR = m+n+1;
        
        Polynom r;
        for (int k = 0; k < szR; ++k)
        {
            T sum = 0;
            for (int i = max(0, k-n); i <= min(k, m); ++i)
            {
                sum += p.a[i] * q.a[k-i];
            }
            r.a.push_back(sum);
        }
        
        return r;
    }
    
    friend ostream& operator<<(ostream & stream, Polynom const & p)
    {
        if (p.a.size() == 0) return stream << '0';
        if (p.a.size() == 1) return stream << p.a[0];
        
        int i = p.a.size() - 1;
        if (i == 1)
        {
            if      (p.a[1] ==  1) stream <<           'x';
            else if (p.a[1] == -1) stream <<          "-x";
            else                   stream << p.a[1] << 'x';
            
            if      (p.a[0] > 0) stream << " + " <<  p.a[0];
            else if (p.a[0] < 0) stream << " - " << -p.a[0];
        }
        else
        {
            if (p.a[i] != 0)
            {
                if      (p.a[i] ==  1) stream <<  "x^" << i;
                else if (p.a[i] == -1) stream << "-x^" << i;
                else                   stream << p.a[i] << "x^" << i;
                --i;
            }
            for (; i > 1; --i)
            {
                if      (p.a[i] ==  1) stream << " + x^" << i;
                else if (p.a[i] == -1) stream << " - x^" << i;
                else if (p.a[i] > 0)   stream << " + " <<  p.a[i] << "x^" << i;
                else if (p.a[i] < 0)   stream << " - " << -p.a[i] << "x^" << i;
            }
            // i = 1
            if      (p.a[1] ==  1) stream << " + x";
            else if (p.a[1] == -1) stream << " - x";
            else if (p.a[1] > 0)   stream << " + " <<  p.a[1] << 'x';
            else if (p.a[1] < 0)   stream << " - " << -p.a[1] << 'x';
            
            if      (p.a[0] > 0) stream << " + " <<  p.a[0];
            else if (p.a[0] < 0) stream << " - " << -p.a[0];
        }

        return stream;
    }
};

int main()
{
  // Polynomkoeffizienten auf Vektor a einlesen, Polynom p erzeugen
  int m;
  cout << "m: "; cin >> m;
  vector<double> a;
  a.reserve(m+1);
  for (int i = 0; i <= m; ++i)
  {
    double inp; cin >> inp;
    a.push_back(inp);
  }
  
  Polynom<double> p(a);

  cout << "p  = " << p << endl;
  
  int n;
  cout << "n: "; cin >> n;
  vector<double> b;
  a.reserve(n+1);
  for (int i = 0; i <= n; ++i)
  {
    double inp; cin >> inp;
    b.push_back(inp);
  }
  
  Polynom<double> q(b);
  
  cout << "q  = " << q << endl;
  
  Polynom<double> p_  = p.deriv(),
                  q_  = q.deriv(),
                  pq_ = (p * q).deriv();
  
  cout << "(pq)'     = " << pq_ << endl;
  
  cout << "p'q + pq' = " << p_ * q + p * q_ << endl;
  
  return 0;
}
