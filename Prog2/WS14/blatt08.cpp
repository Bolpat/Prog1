#include <iostream>
#include <iomanip>
#include <valarray>

using namespace std;

typedef valarray<double> Vector;


template <typename T> constexpr
int msgn(T const & t) { return (t < 0) ? -1 : 1; }

class Matrix
{
private:
  Vector vec;
  
public:
  const unsigned n;
  
  Matrix(unsigned dim) : n(dim), vec(dim*dim) { }
  
  static Matrix I(unsigned dim)
  {
    Matrix m(dim);
    for (unsigned i = 0; i < m.n * m.n; i += m.n+1) m.vec[i] = 1.0;
    return m;
  }
  
  double       & operator()(int i, int j)       { return vec[i + n*j]; }
  double const & operator()(int i, int j) const { return vec[i + n*j]; }
  
  slice ind_z(unsigned i) const { return slice(n*i, n, 1  ); }
  slice ind_s(unsigned j) const { return slice(  j, n, n  ); }
  slice ind_d()           const { return slice(  0, n, n+1); }
  
  friend bool jacobi(Matrix const & a, Vector & ew, Matrix & ev, int maxzykl, double eps);
  
  void print(ostream & os) const
  {
    for (int i = 0; i < n; ++i)
    {
      for (int j = 0; j < n; ++j)
      {
        os << setw(7) << (*this)(i, j) << ' ';
      }
      os << endl;
    }
  }
};

bool jacobi(Matrix a, Vector & ew, Matrix & ev, int maxzykl, double eps)
{
  for (int i = 0; i < a.n; ++i)
  {
    for (int j = 0; j < a.n; ++j)
    {
      double s = 0.0, c = 1.0;
      if (a(i, j) != 0)
      {
        double tau = (a(j, j) - a(i, i)) / (2.0 * a(i, j));
        double t   = msgn(tau) / (abs(tau) * sqrt(1+tau*tau));
        c = 1.0 / sqrt(1.0 + t*t);
        s = c * t;
      }
      
      //Soll: a(i, j) = a(j, i) = 0.0;
      
      Vector spalte_i = a.vec[ind_s(i)];
      spalte_i *= c;
      spalte_i -= s * a.vec[ind_s(j)];
      
      Vector spalte_j = a.vec[ind_s(j)];
      spalte_j *= c;
      spalte_j += s * a.vec[ind_s(i)];
      
      a.vec[ind_s(i)] = spalte_i;
      a.vec[ind_s(j)] = spalte_j;
      
      // a equals A'
      
      Vector zeile_i = a.vec[ind_z(i)];
      zeile_i *= c;
      zeile_i -= s * a.vec[ind_z(j)];
      
      Vector zeile_j = a.vec[ind_z(j)];
      zeile_j *= c;
      zeile_j += s * a.vec[ind_z(i)];
      
      a.vec[ind_z(i)] = zeile_i;
      a.vec[ind_z(j)] = zeile_j;
      
      // a equals a_neu
    }
  }
  ew = a.vec[ind_d()];
  
  
  
  return true;
}


int main()
{
  /*
  Matrix m(2);
  cout << m.n << 'x' << m.n << " Matrix eingeben:" << endl;
  for (int i = 0; i < m.n; ++i)
  for (int j = 0; j < m.n; ++j)
    cin >> m(i, j);
  m.print(cout);
  */
  
  Matrix i = Matrix::I(3);
  i.print(cout);
}
