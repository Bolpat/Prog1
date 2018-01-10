// Programmentwurf zu Aufgabe 9, WS 2017/2018
// Programmieren II fuer (Wirtschafts-)Mathematiker

#include <valarray>

using namespace std;

template <class T> class myslice_array
{
private:
    slice s;
    valarray<T> & a;
public:
    myslice_array<T> (valarray<T> & a, int i0, int n, int h)
    : s(slice(i0,n,h)), a(a)
    { }
    T& operator[] (int j)
    {
        return a[s.start()+j*s.stride()];
    }
    // Typumwandlungsoperator: myslice_array<T> -> valarray<T>
 
    // Zuweisungsoperator:     myslice_array<T> = valarray<T>

    // Zuweisungsoperator:     myslice_array<T> = T

};

template <class T> class mycslice_array
{
private:
    slice s;
    const valarray<T> & a;
public:
    mycslice_array<T> (const valarray<T> & a, int i0, int n, int h)
    : s(slice(i0,n,h)), a(a)
    { }
    T operator[] (int j) const
    {
        return a[s.start()+j*s.stride()];
    }
    // Typumwandlungsoperator: mycslice_array<T> -> valarray<T>
};

template <class T> class matrix
{
private: 
    int m,n;
    valarray<T> a;
public:
    matrix<T> (int m = 0, int n = 0) : m(m), n(n), a(m*n) { }
    size_t nzeilen()  const { return m; }
    size_t nspalten() const { return n; }

    myslice_array<T> operator[] (int i)
    {
        return myslice_array<T>(a,i*n,n,1);
    }
    mycslice_array<T> operator[] (int i) const
    {
        return mycslice_array<T>(a,i*n,n,1);
    }
    myslice_array<T> zeile(int i)
    {
        return myslice_array<T>(a,i*n,n,1);
    }
    mycslice_array<T> zeile(int i) const
    {
        return mycslice_array<T>(a,i*n,n,1);
    }
    myslice_array<T> spalte(int j)
    {
        return myslice_array<T>(a,j,n,n);
    }
    mycslice_array<T> spalte(int j) const
    {
        return mycslice_array<T>(a,j,n,n);
    }
    // myslice_array<T> diagonale() { }   

    // mycslice_array<T> diagonale() const{ }

    // Zuweisungsoperator:  matrix<T> = T

};


typedef valarray<double> Vektor;
typedef   matrix<double> Matrix;

bool jacobi(Matrix a, Vektor& ew, Matrix& ev, int maxzykl, double eps)
{


}



int main()
{
  // Vorabtest:
  // 
  // n einlesen
  // Matrix e(n,n);
  // e = 0.0;
  // e.diagonale() = 1.0; 
}

