#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
//#include <stack>
#include <sstream>
#include <initializer_list>

using namespace std;

template <unsigned p>
class Z
{
private:
  unsigned x;
  constexpr
  Z(unsigned x, bool) : x(x) {  }
  
  Z inv()
  {
    Z one = Z(1, true);
    for (unsigned i = 1; i < p; ++i)
    {
      Z zi = Z(i, true);
      if (zi * x == one) return zi;
    }
    return 0;
  }
  
public:
  constexpr
  Z(unsigned x = 0u) : x(x % p) {  }
  
  constexpr
  Z(signed x) : x( x < 0 ? (p-x) % p : x % p ) {  }
  
  friend constexpr
  bool operator==(Z z1, Z z2) { return z1.x == z2.x; }
  
  friend constexpr
  bool operator!=(Z z1, Z z2) { return z1.x != z2.x; }
  
  constexpr
  Z operator+() { return Z(x,   true); }
  
  constexpr
  Z operator-() { return Z(p-x, true); }
  
  friend constexpr
  Z operator+(Z z1, Z z2) { return Z(z1.x + z2.x); }
  
  friend constexpr
  Z operator-(Z z1, Z z2) { return Z(z1.x - z2.x); }
  
  friend constexpr
  Z operator*(Z z1, Z z2) { return Z(z1.x * z2.x); }
  
  friend // constexpr
  Z operator/(Z z1, Z z2) { return Z(z1.x * z2.inv().x); }
  
  //constexpr
  Z & operator+=(Z const & z) { return *this = *this + z; }
  
  //constexpr
  Z & operator-=(Z const & z) { return *this = *this - z; }
  
  //constexpr
  Z & operator*=(Z const & z) { return *this = *this * z; }
  
  //constexpr
  Z & operator/=(Z const & z) { return *this = *this / z; }
  
  friend
  ostream & operator<<(ostream & os, Z z) { return os << z.x << '[' << p << ']'; }
};

template <typename T>
class Monom
{
public:
  T        coeff;
  unsigned deg;
  
  Monom (T coeff = 0, unsigned deg = 0) : coeff(coeff), deg(deg) {  }
  
  Monom operator-() const { return Monom(-coeff, deg); }
  
  friend 
  unsigned grad(Monom const & m) { return m.deg; }
  
  friend
  bool operator< (Monom m1, Monom m2) { return m1.deg < m2.deg ? true : m1.coeff < m2.coeff; } 
  
  friend
  bool operator==(Monom m1, Monom m2) { return m1.deg == m2.deg && m1.coeff == m2.coeff; }
  
  friend
  Monom operator*(T const & t, Monom const & m) { return Monom(t * m.coeff, m.deg); }
  
  friend
  Monom operator*(Monom const & m, T const & t) { return Monom(t * m.coeff, m.deg); }
  
  friend
  Monom operator*(Monom const & m1, Monom const & m2) 
  { return Monom(m1.coeff * m2.coeff, m1.deg + m2.deg); }
  
  Monom & operator*=(Monom const & m)
  {
    coeff *= m.coeff;
    deg += m.deg;
    return *this;
  }
  
  template <typename S, S zero = S(), S one = S(1)> inline
  S operator()(S const & value)
  {
    if (deg == 0)      return one;
    if (value == zero) return zero;
    
    unsigned hiBit = ~((~0u) >> 1u);
    while ((hiBit & deg) == 0) hiBit >>= 1u;
    // hiBit maskiert erstes gesetztes Bit von deg (ist != 0, da deg != 0);
    
    S erg = value;
    
    while (hiBit >>= 1u)
    {
      erg *= erg;
      if (hiBit & deg) erg *= value;
    }
    
    return erg;
  }
  
  friend ostream & operator<<(ostream & os, Monom const & m)
  {
    return os << m.coeff << "x^" << m.deg;
  }
};

template <typename T>
class Polynom
{
private:
  list<Monom<T>> coeffs;
  
  struct is_zero
  {
    bool operator(Monom<T> const & m) { return m.coeff == T(); }
  };

  struct is_near_zero
  {
    T eps;
    is_near_zero(T const & eps) : eps(eps) {  }
    bool operator(Monom<T> const & m) { return -eps < m.coeff && m.coeff < eps; }
  };
  
public:
  
  Polynom() { }
  Polynom(T const & t) : coeffs(1, Monom<T>(t, 0)) {  }
  Polynom(Monom<T> const & monom) : coeffs(1, monom) {  }
  
  void eliminateZeros()
  {
    coeffs.remove_if(is_zero);
  }
  
  void eliminateZeros(T eps)
  {
    coeffs.remove_if(is_near_zero(eps));
  }
  
  Polynom operator-() const
  {
    Polynom ret = *this;
    for (auto it =  ret.coeffs.begin();
              it != ret.coeffs.end();
            ++it)
         {
           it->coeff = -(it->coeff);
         }
         return ret;
  }
  
  friend Polynom operator+(Polynom const & pol1, Polynom const & pol2)
  {
    Polynom ret = pol1;
    auto it2 = pol2.coeffs.begin();
    for (auto it1 =  ret.coeffs.begin();
              it1 != ret.coeffs.end() && it2 != pol2.coeffs.end();
            ++it1)
         {
           if (it1->deg > it2->deg)
           {
             auto it2range = it2;
             while (++it2range != pol2.coeffs.end() && it1->deg > it2range->deg);
             ret.coeffs.insert(it1, it2, it2range);
             it2 = it2range;
           }
           else if (it1->deg == it2->deg)
           {
             it1->coeff += it2->coeff;
             ++it2;
           }
         }
         // now it1 == ret.coeffs.end() or it2 == pol2.coeffs.end()
         // second one doesn't actually matter at all:
         // assuming second implies the following command to do nothing,
         // so we only care for first one. Then it1 == ret.coeffs.end()
         ret.coeffs.insert(ret.coeffs.end() , it2, pol2.coeffs.end());
         
         return ret;
  }
  Polynom & operator+=(Polynom const & pol) { return *this = *this + pol; }
  
  friend
  Polynom operator-(Polynom const & pol1, Polynom const & pol2) { return pol1 + (-pol2); }
  Polynom & operator-=(Polynom const & pol) { return *this = *this - pol; }
  
  Polynom & operator*=(Monom<T> const & mon)
  {
    for (auto it =  coeffs.begin();
              it != coeffs.end();
            ++it)
      {
        it->coeff *= mon.coeff;
        it->deg   += mon.deg;
      }
    return *this;
  }
  
  Polynom operator*(Monom<T> const & mon) const
  {
    Polynom ret = *this;
    ret *= mon;
    return ret;
  }
  
  Polynom & operator*=(Polynom const & pol)
  {
    cout << "Mult Polynom " << *this << " with Polynom " << pol << endl;
    Polynom summe;
    for (auto it =  pol.coeffs.begin();
              it != pol.coeffs.end();
            ++it)
         {
           summe += *this * *it;
         }
         return *this = summe;
  }
  
  friend
  Polynom operator*(Polynom const & pol1, Polynom const & pol2)
  {
    Polynom ret = pol1;
    ret *= pol2;
    return ret;
  }
  
  friend
  Polynom operator+(T const & t, Polynom const & p) { return p + Monom<T>(t); }
  
  friend 
  Polynom operator+(Polynom const & p, T const & t) { return p + Monom<T>(t); }
  
  friend
  Polynom operator*(T const & t, Polynom const & p) { return p * Monom<T>(t); }
  
  friend 
  Polynom operator*(Polynom const & p, T const & t) { return p * Monom<T>(t); }
  
  friend
  ostream & operator<<(ostream & os, Polynom const & pol)
  {
    auto it =  pol.coeffs.rbegin();
    switch (it->deg)
    {
      case 0:  os << it->coeff; break;
      case 1:  os << it->coeff << 'x'; break;
      default: os << it->coeff << "x^" << it->deg;
    }
    while (++it != pol.coeffs.rend())
    {
      switch (it->deg)
      {
        case 0:  os << " + " << it->coeff; break;
        case 1:  os << " + " << it->coeff << 'x'; break;
        default: os << " + " << it->coeff << "x^" << it->deg;
      }
    }
    return os;
  }
};

template <typename T>
Polynom<T> fromCoeffVector(vector<T> const & coeffs)
{
  Polynom<T> ret;
  unsigned deg = 0;
  for (auto it = coeffs.begin(); it != coeffs.end(); ++it)
  {
    ret += Monom<T>(*it, deg);
    ++deg;
  }
  
  ret.eliminateZeros();
  return ret;
}

template <unsigned p>
void increment(vector<Z<p>> & v)
{
  for (auto & z : v)
    if ((z += 1) != 0)
      return;
}

int main()
{
  constexpr unsigned p = 2;
  constexpr unsigned n = 10;
  typedef Z<p> K;
  
  Polynom<K> x = Monom<K>(K(1), 1);
  
  vector<K> coeffs(n);
  
  Polynom<K> pol = fromCoeffVector<K>(coeffs);
  cout << pol << endl;
  /*
  list< Polynom<K> > lst;
  

  // konstante Polynome sind uninteressant.
  vector<K> maxcoeffs(n, K(p-1));
  vector<K> coeffs(n);
  while (coeffs != maxcoeffs)
  {
    
  }
  
  // Polynome vom Grad 1 sind immer irreduzibel (über Körpern).
  
  cout << "Irreduzible Polynom in Z/(" << p << "):" << endl;
  
  while (!stc.empty())
  {
    auto p = move(stc.top());
    cout << p << endl;
    stc.pop();
  }
  */
  return 0;
}