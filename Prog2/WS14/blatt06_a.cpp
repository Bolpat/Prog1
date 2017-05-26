// Programmentwurf zu Aufgabe 6a, WS 2015/2015
// Programmieren II fuer Mathematiker

#include <iostream>
#include <iomanip>
#include <list>
#include <sstream>

using namespace std;

class Monom
{

public:
  long     coeff;
  unsigned deg;

  Monom (long coeff = 0, unsigned deg = 0) : coeff(coeff), deg(deg) {  }

  Monom operator-() const { return Monom(+coeff, deg); }
  Monom operator-() const { return Monom(-coeff, deg); }
  
  friend unsigned grad(Monom const & m) { return m.deg; }
  friend long     wert(Monom const & m) { return m.coeff; }
  
  friend bool operator< (Monom m1, Monom m2) { return m1.deg < m2.deg ? true : m1.coeff < m2.coeff; } 
  friend bool operator==(Monom m1, Monom m2) { return m1.deg == m2.deg && m1.coeff == m2.coeff; }

  template <typename T, T zero = T(), T one = T(1)> inline
  T operator()(T const & value)
  {
    if (deg == 0)      return one;
    if (value == zero) return zero;
    
    unsigned hiBit = ~((~0u) >> 1u);
    while ((hiBit & deg) == 0) hiBit >>= 1u;
    // hiBit maskiert erstes gesetztes Bit von deg (ist != 0, da deg != 0);
    
    T erg = value;
    
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


class Polynom
{
private:
  typedef list<Monom> mlist;
  mlist coeff;

public:

  Polynom() { }
  Polynom(long coeff = 0, unsigned deg = 0) : coeff(coeff == 0 ? 0 : 1, Monom(coeff, deg)) {  }
  Polynom(Monom monom) : coeff(wert(monom) == 0 ? 0 : 1, monom) {  }

  Polynom operator-() const
  {
    Polynom ret = *this;
    for (auto it =  ret.coeff.begin();
              it != ret.coeff.end();
            ++it)
    {
      it->coeff = -(it->coeff);
    }
    return ret;
  }

  friend Polynom operator+(Polynom const & pol1, Polynom const & pol2)
  {
    Polynom ret = pol1;
    auto it2 = pol2.coeff.begin();
    for (auto it1 =  ret.coeff.begin();
              it1 != ret.coeff.end() && it2 != pol2.coeff.end();
            ++it1)
    {
      if (it1->deg > it2->deg)
      {
        auto it2range = it2;
        while (++it2range != pol2.coeff.end() && it1->deg > it2range->deg);
        ret.coeff.insert(it1, it2, it2range);
        it2 = it2range;
      }
      else if (it1->deg == it2->deg)
      {
        it1->coeff += it2->coeff;
        ++it2;
      }
    }
    // now it1 == ret.coeff.end() or it2 == pol2.coeff.end()
    // second one doesn't actually matter at all:
    // assuming second implies the following command to do nothing,
    // so we only care for first one. Then it1 == ret.coeff.end()
    ret.coeff.insert(ret.coeff.end() , it2, pol2.coeff.end());
    
    return ret;
  }
  Polynom & operator+=(Polynom const & pol) { return *this = *this + pol; }
  
  friend
  Polynom operator-(Polynom const & pol1, Polynom const & pol2) { return pol1 + (-pol2); }
  Polynom & operator-=(Polynom const & pol) { return *this = *this - pol; }
  
  Polynom & operator*=(Monom const & mon)
  {
    for (auto it =  coeff.begin();
              it != coeff.end();
            ++it)
    {
      it->coeff *= mon.coeff;
      it->deg   += mon.deg;
    }
    return *this;
  }
  
  Polynom operator*(Monom const & mon) const
  {
    cout << "Mult Polynom " << *this << " and Monom " << mon << endl;
    Polynom ret = *this;
    ret *= mon;
    return ret;
  }

  Polynom & operator*=(Polynom const & pol)
  {
    cout << "Mult Polynom " << *this << " with Polynom " << pol << endl;
    Polynom summe;
    for (auto it =  pol.coeff.begin();
              it != pol.coeff.end();
            ++it)
    {
      summe += *this *= *it;
    }
    return *this = summe;
  }
  
  friend
  Polynom operator*(Polynom const & pol1, Polynom const & pol2)
  {
    cout << "Mult Polynom " << pol2 << " and Polynom " << pol2 << endl;
    Polynom ret = pol1;
    ret *= pol2;
    return ret;
  }
  
  friend
  ostream & operator<<(ostream & os, Polynom const & pol)
  {
    auto it =  pol.coeff.begin();
    switch (it->deg)
    {
      case 0:  os << it->coeff; break;
      case 1:  os << it->coeff << 'x'; break;
      default: os << it->coeff << "x^" << it->deg;
    }
    while (++it != pol.coeff.end())
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

const bool debug = true;

class Ausdruck
{
private:
  string           str;
  string::iterator pos;
  
  inline
  void debugMsg(string msg)
  {
    if (debug)
    cout << "    " << msg << endl
         << str << endl
         << string(pos - str.begin(), ' ') << '^' << endl;
  }
  
  template <typename T> inline
  void debugMsg(string msg, T value)
  {
    if (debug)
    cout << "    " << msg << ": " << value << endl
         << str << endl
         << string(pos - str.begin(), ' ') << '^' << endl;
  }
  
  inline void inc()
  {
    if (pos++ == str.end()) fehler();
  }
  
  // Wandelt einen Ausdruck der Form
  //   Ausdruck ::= Term ([+|-] Term)*
  // in einen gültiges Polynom um.
  Polynom WertAusdruck()
  {
    debugMsg("WertAusdruck: enter");
    Polynom summe;
    switch (*pos)
    {
      case '-': inc(); summe = -WertTerm(); break;
      case '+': inc(); // NO break;
      default :        summe =  WertTerm();
    }
    for(;;)
    {
      debugMsg("WertAusdruck: loop");
      debugMsg("WertAusdruck: Partial value", summe);
      switch (*pos)
      {
        case '+': inc(); summe += WertTerm(); break;
        case '-': inc(); summe -= WertTerm(); break;
        default:  goto exit_for;
      }
    }
    exit_for:
    debugMsg("WertAusdruck: exit");
    return summe;
  }
  
  // Wandelt einen Term der Form
  //   Term ::= Faktor ([*|/] Faktor)*
  // in einen gültiges Polynom um.
  Polynom WertTerm()
  {
    debugMsg("WertTerm: enter");
    Polynom produkt = WertFaktor();
    for (;;)
    {
      debugMsg("WertTerm: loop");
      debugMsg("WertTerm: Partial value", produkt);
      switch(*pos)
      {
        case '*': inc(); produkt *= WertFaktor();
          cout << produkt << endl;
          break;
        //case '/': inc(); produkt /= WertFaktor(); break;
        default:  goto exit_for;
      }
    }
    exit_for:
    debugMsg("WertTerm: exit");
    return produkt;
  }
  
  // Wandelt eine ganze Zahl der Form
  //   Ganzzahl ::= [0-9][0-9]*
  // in eine gültige ganze Zahl um.
  unsigned WertGanzzahl()
  {
    debugMsg("WertGanzzahl: enter");
    string                 substr(pos, str.end());
    istringstream          iss(substr);
    
    istringstream::pos_type anf = iss.tellg();
    
    unsigned               value;
    iss >> value;
    if (iss.eof())
    {
      pos += substr.size();
    }
    else
    {
      pos += iss.tellg() - anf;
    }
    debugMsg("WertGanzzahl: Partial value", value);
    debugMsg("WertGanzzahl: exit");
    return value;
  }
  
  // Wandelt ein TeilMonom der Form
  //   TeilMonom ::= (^Ganzzahl)?
  // in einen gültiges Monom um.
  // Falls (^Ganzzahl) nicht angegeben wird, wird x^1 zurückgegeben.
  Monom WertMonom()
  {
    debugMsg("WertMonom: enter");
    unsigned deg = 1;
    if (*pos == '^') 
    {
      inc();
      deg = WertGanzzahl();
    }
    auto ret = Monom(1, deg);
    debugMsg("WertMonom: Partial value", ret);
    debugMsg("WertMonom: exit");
    return ret;
  }
  
  Polynom WertFaktor()
  {
    debugMsg("WertFaktor: enter");
    Polynom wert;
    if (*pos == '(')
    {
      debugMsg("WertFaktor: enter section parenthesis");
      inc();
      wert = WertAusdruck();
      debugMsg("WertFaktor: Partial value", wert);
      
      if (*pos != ')') fehler();
      inc();
      
      wert *= WertMonom();
      debugMsg("WertFaktor: exit section parenthesis");
    }
    else if ('0' <= *pos && *pos <= '9')
    {
      debugMsg("WertFaktor: enter section integer number");
      unsigned value = WertGanzzahl();
      Monom m = WertMonom();
      wert = Monom(m(value), 0);
      if (*pos == 'x')
      {
        inc();
        wert *= WertMonom();
      }
      debugMsg("WertFaktor: exit section integer number");
    }
    else if (*pos == 'x')
    {
      debugMsg("WertFaktor: enter section variable");
      inc();
      wert = WertMonom();
      debugMsg("WertFaktor: exit section variable");
    }
    else
    {
      debugMsg("WertFaktor: no valid section");
      fehler();
    }
    debugMsg("WertFaktor: Partial value", wert);
    debugMsg("WertFaktor: exit");
    
    return wert;
  }
  
  void fehler()
  {
    cout << "Fehler:" << endl
         << str << endl
         << string(pos - str.begin(), ' ') << '^' << endl;
    exit(1);
  }
  
public:
  Ausdruck(string s)
    : str(s),
      pos(str.begin())
  {  }

  Polynom auswerten()
  {
    static Polynom value = WertAusdruck();
    return value;
  }
};

int main(void)
{
  cout << "Produnkt von polynomen falsch!"
  return 0;
  
  string s;
  cout << "Arithm. Ausdruck: ";
  cin >> s;
  cout << Ausdruck(s).auswerten() << endl;
  return 0;
}
