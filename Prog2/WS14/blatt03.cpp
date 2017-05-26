// Programmteilentwurf zu Aufgabe 3, WS 2014/2015
// Programmieren II fuer Mathematiker

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <vector>

using namespace std;


class rational {

private:
  long p,q;
  
  rational inv() const { return rational(q, p); }
  
  void normalize()
  {
    long g = ggt(p, q);
    p /= g;
    q /= g;
    
    if (q < 0)
    {
      p = -p;
      q = -q;
    }
  }
  
  static long ggt(long a, long b)
  {
    long r;
    while (b != 0)
    {
      r = a % b;
      a = b;
      b = r;
    }
    return a;
  }
  
  // if simple p*p' / q*q' is obviously normalized
  rational & fastMultWith(rational r)
  {
    p *= r.p;
    q *= r.q;
    return *this;
  }
  
  // if p/q is already normalized
  rational(long p, long q, bool)
  : p(p), q(q) {  }
  
public:
  rational(long p = 0, long q = 1)
  : p(p), q(q) { normalize(); }

  friend rational operator+(rational r) { return rational( r.p, r.q, true); } 
  friend rational operator-(rational r) { return rational(-r.p, r.q, true); } // falls r normalisiert ist normalisieren nicht notwendig..
 
  friend rational operator+(rational r, rational s) { return rational(r.p * s.q + s.p * r.q,   r.q * s.q); }
  friend rational operator-(rational r, rational s) { return rational(r.p * s.q - s.p * r.q,   r.q * s.q); }
  friend rational operator*(rational r, rational s) { return rational(r.p * s.p,   r.q * s.q); }
  friend rational operator/(rational r, rational s) { return rational(r.p * s.q,   r.q * s.p); }
  friend rational operator/(rational r, long l)     { return rational(r.p, r.q * l); }
  friend rational operator/(long l, rational r)     { return rational(r.p * l, r.q); }

  rational & operator+=(rational r) { return *this = *this + r; }  
  rational & operator-=(rational r) { return *this = *this - r; } 
  rational & operator*=(rational r) { return *this = *this * r; }
  rational & operator/=(rational r) { return *this = *this / r; }
  
  friend ostream & operator<<(ostream & o, rational const & r) { return o << r.p << '/' << r.q; }
  
  friend istream & operator>>(istream & i, rational       & r)
  {
    string s; i >> s;
    size_t slash = s.find('/');
    if (slash != string::npos)
    {
      s[slash] = ' ';
    }
    std::istringstream iss(s);
    long p, q;
    if (slash != string::npos)
    {
      iss >> p >> q;
      r = rational(p, q);
    }
    else
    {
      iss >> p;
      r = rational(p);
    }
    return i;
  }
  
  friend long double rational2longdouble(rational r)
  {
    long double p = r.p, q = r.q;
    return p / q;
  }
  
  friend rational pow(rational const & r, int n)
  {
    return n < 0 ? pow(r.inv(), (unsigned)(-n)) : pow(r, (unsigned)n);
  }
  
  friend rational pow(rational const & r, unsigned n)
  {
    if (n == 0)   return rational(1);
    if (r.p == 0) return rational(0);
    
    unsigned hiBit = ~((~0u) >> 1u);
    while ((hiBit & n) == 0) hiBit >>= 1u;
    // hiBit maskiert erstes gesetztes Bit von n (ist != 0, da n != 0);
    
    rational erg = r;

    while (hiBit >>= 1u)
    {
      erg *= erg;
      if (hiBit & n) erg *= r;
    }

    return erg;
  }
  
  friend rational geometricPow(rational r, unsigned n)
  {
    rational sum = r;
    rational exp = r;
    for (unsigned i = 2; i <= n; ++i) sum += exp.fastMultWith(r);
    return sum;
  }
};

rational chain(vector<unsigned> a, vector<unsigned> b)
{
  if (b.size() != a.size() + 1) throw string("Illegal vector sizes.");
                                             rational ret = *(b.rbegin()); // b[n] exists, see condition above and consider a.size() >= 0.
  for (unsigned i = a.size() - 1; i > 0; --i)
  {
    ret = b[i] + a[i]/ret;
  }
  return ret;
}

int main()
{
  unsigned n = 8;
  const rational r(2, 3), s(-10, 7);
  
  char exNo;
  cout << "Aufgabe wählen: " << endl;
  cin >> exNo;
  switch (exNo)
  {
    case 'a':
    {
      rational res = geometricPow(r, n);
      cout << "r  =  " << r << endl;
      cout << "sum[k = 1 .. " << n << "] k*r^k  =  " << res;
      cout << " ~ " << rational2longdouble(res) << endl;
      
      res = geometricPow(s, n);
      cout << "s = " << s << endl;
      cout << "sum[k = 1 .. " << n << "] k*s^k  =  " << res;
      cout << " ~ " << rational2longdouble(res) << endl;
      
      break;
    }
    
    case 'b':
    {
      rational res = ( n * pow(r, n+2) - (n+1) * pow(r, n+1) + r ) / pow(r-1, 2);
      cout << "r = " << r << endl;
      cout << "( n * r^(n+2) - (n+1) * r^(n+1) + r ) / (r - 1)^2  =  "
           << res << endl;
      cout << " ~ " << rational2longdouble(res) << endl;
      
      res = ( n * pow(s, n+2) - (n+1) * pow(s, n+1) + s ) / pow(s-1, 2);
      cout << "s = " << s << endl;
      cout << "( n * s^(n+2) - (n+1) * s^(n+1) + s ) / (s - 1)^2  =  "
           <<  res << endl;
      cout << " ~ " << rational2longdouble(res) << endl;
      
      break;
    }
    
    case 'c':
    {
      vector<unsigned> a(4, 1);
      vector<unsigned> b;
      b.reserve(5);
      b.push_back(  3);
      b.push_back(  7);
      b.push_back( 15);
      b.push_back(  1);
      b.push_back(292);
      
      rational res = chain(a, b);
      cout << "[3,1,7,1,15,1,1,1,292]  =  " << res << endl;
      cout << " ~ " << rational2longdouble(res) << endl;
    
      break;
    }
    
    case 'd':
    {
      for (unsigned n = 1; n <= 10; ++n)
      {
        cout << "n  =  " << n << endl;
        
        vector<unsigned> a, b;
        a.reserve(n-1);
        b.reserve(n);
        
        a.push_back(4);
        b.push_back(0);
        
        for (unsigned i = 1; i < n; ++i)
        {
          a.push_back(i*i);
          b.push_back(2*i - 1);
        }
        b.push_back(2*n - 1);
        
        cout << '[';
        for (unsigned i = 0; i < a.size(); ++i)
        {
          cout << a[i] << "," << b[i] << ",";
        }
        cout << *(b.rbegin()) << "]  =  ";
        rational res = chain(a, b);
        cout << res << "  ~  " << rational2longdouble(res) << endl;
      }
      break;
    }
    
    default:
      cout << "Keine Aufgabennummer " << exNo << " gestellt." << endl;
  }
  return 0;
}      
