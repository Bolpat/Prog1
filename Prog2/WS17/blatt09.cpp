#include <iostream>
#include <iomanip>
#include <valarray>
#include <cmath>
#include <algorithm>

using namespace std;

template <typename T>
struct myslice_array
{
private:
    slice s;
    valarray<T> & a;
public:
    myslice_array<T> (valarray<T> & a, int i0, int n, int h)
    : s(i0, n, h), a(a)
    { }
    
    // Indexoperator (den std::slice_array nicht unterstützt)
    T & operator[](int j)
    {
        return a[s.start() + j * s.stride()];
    }
    
    // Typumwandlungsoperator: myslice_array<T> -> valarray<T>
    operator valarray<T>()
    {
        return a[s];
    }

    // Zuweisungsoperator:     myslice_array<T> = valarray<T>
    myslice_array & operator=(const valarray<T> & va)
    {
        a[s] = va;
        return *this;
    }

    // Zuweisungsoperator:     myslice_array<T> = T
    myslice_array & operator=(T t)
    {
        a[s] = t;
        return *this;
    }
    
    // Einzig gebrauchter Operator: *
    friend valarray<T> operator*(T t, myslice_array msa)
    {
        return static_cast<valarray<T>>(msa) * t;
    }
};

template <typename T>
struct mycslice_array
{
private:
    slice s;
    const valarray<T> & a;
public:
    mycslice_array<T> (const valarray<T> & a, int i0, int n, int h)
    : s(i0, n, h), a(a)
    { }
    
    T operator[](int j)
    {
        return a[s.start() + j * s.stride()];
    }
    
    // Typumwandlungsoperator: mycslice_array<T> -> valarray<T>
    operator valarray<T>()
    {
        return a[s];
    }
    
    // Einzig gebrauchter Operator: *
    friend valarray<T> operator*(T t, mycslice_array msa)
    {
        return static_cast<valarray<T>>(msa) * t;
    }
};

template <typename T>
struct matrix
{
private: 
    int m,n;
    valarray<T> a;
public:
    matrix<T> (int m = 0, int n = 0) : m(m), n(n), a(m * n) { }
    size_t rows_count()  const { return m; }
    size_t cols_count() const { return n; }

    // Macht den Zugriff auf Matrix-Elemente mit matrix[i][j] möglich
    myslice_array<T> operator[](int i)
    {
        return myslice_array<T>(a, i * n, n, 1);
    }
    
    mycslice_array<T> operator[](int i) const
    {
        return mycslice_array<T>(a, i * n, n, 1);
    }
    
    myslice_array<T> row(int i)
    {
        return myslice_array<T>(a, i * n, n, 1);
    }
    
    mycslice_array<T> row(int i) const
    {
        return mycslice_array<T>(a, i * n, n, 1);
    }
    
    myslice_array<T> col(int j)
    {
        return myslice_array<T>(a, j, m, n);
    }
    
    mycslice_array<T> col(int j) const
    {
        return mycslice_array<T>(a, j, m, n);
    }
    
    myslice_array<T> diag()
    {
        return myslice_array<T>(a, 0, min(m, n), n+1);
    }

    mycslice_array<T> diag() const
    {
        return mycslice_array<T>(a, 0, min(m, n), n+1);
    }

    matrix & operator=(T t)
    {
        a = t;
        return *this;
    }
};


typedef valarray<double> Vector;
typedef   matrix<double> Matrix;


void print_matrix(ostream & stream, const Matrix & m, int width)
{
    for (size_t i = 0; i < m.rows_count(); ++i)
    {
        for (size_t j = 0; j < m.cols_count(); ++j)
        {
            stream << setw(width) << m[i][j] << ' ';
        }
        stream << '\n';
    }
}

int jacobi(Matrix a, Vector & ew, Matrix & ev, int maxzykl, double eps)
{
    // Prüfe, ob a eine quadratische Matrix ist.
    if (a.rows_count() != a.cols_count()) return -3;
    size_t n = a.rows_count();
    
    // Prüfe, ob a eine symmetrische Matrix ist.
    for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < i; ++j)
    if (a[i][j] != a[j][i])
        return -2;
    
    double N = 0.0;
    for (size_t i =  0;  i < n; ++i)
    for (size_t j = i+1; j < n; ++j)
        N += 2*abs(a[i][j])*abs(a[i][j]);
    const double NA = N;
    
cerr << "N: " << N << '\n';
    
    Matrix u(n, n); // n*n-Nullmatrix
    u.diag() = 1.0; // n*n-Identitätsmatrix
    
    while (maxzykl-- > 0)
    {
print_matrix(cerr, u, 15);
cerr << '\n';
        
        for (size_t i =  0;  i < n; ++i)
        for (size_t j = i+1; j < n; ++j)
        {
            // Konvergenz?
            if (N <= eps * NA)
            {
                ew = a.diag();
                ev = move(u);
                return 0;
            }
            N -= abs(a[i][j]) * abs(a[i][j]) * 2.0;
cerr << "N: " << N << '\n';
            
            double T = (a[j][j] - a[i][i]) / 2.0 / a[i][j];
            double t = (T < 0 ? -1.0 : +1.0) / (abs(T) + sqrt(1.0 + T*T));
            // // Note: c = 1 / sqrt(1 + t*t) and s = t*c is faster but has less precision in s.
            // //       This way precision is favored over speed.
            // double c = sqrt(1.0 + t*t);
            // double s = t / c;
            // c = 1.0 / c;
            double c = 1.0 / sqrt(1.0 + t*t);
            double s = c * t;
            
            a.col(i) = c * a.col(i) - s * a.col(j);
            a.col(j) = s * a.col(i) + c * a.col(j);
            a.row(i) = c * a.row(i) - s * a.row(j);
            a.row(j) = s * a.row(i) + c * a.row(j);
            
            u.col(i) = c * u.col(i) - s * u.col(j);
            u.col(j) = s * u.col(i) + c * u.col(j);
        }
    }
    // maxzykl Iterationen haben nicht gereicht: Abbruch!
    return -1;
}

ostream & operator<<(ostream & stream, const Vector & v)
{
    stream << '[';
    if (v.size() > 0)
    {
        stream << v[0];
        for (size_t i = 1; i < v.size(); ++i)
            stream << ',' << v[i];
    }
    return stream << ']';
}

/*
int main()
{
    size_t m, n;
    cin >> m >> n;
    Matrix a(m, n);
    for (size_t i = 0; i < m; ++i)
    for (size_t j = 0; j < n; ++j)
        cin >> a[i][j];
    
    for (size_t i = 0; i < m; ++i)
    {
        Vector r = a.row(i);
        cout << r << endl;
    }
    cout << endl;
    for (size_t j = 0; j < n; ++j)
    {
        Vector c = a.col(j);
        cout << c << endl;
    }
}
*/

int main()
{
    // Vorabtest:
    // size_t n;
    // cout << "n: ";
    // cin >> n;

    // Matrix e(n, n);
    // e = 0.0;
    // e.diag() = 1.0;
    
    size_t m, n;
    cin >> m >> n;
    Matrix a(m, n);
    for (size_t i = 0; i < m; ++i)
    for (size_t j = 0; j < n; ++j)
        cin >> a[i][j];
    
    Vector ew;
    Matrix ev;
    if (int f = jacobi(a, ew, ev, 3, 1e-5); f < 0)
    {
        cout << "Fehler. " << f << endl;
        return 1;
    }
    for (size_t j = 0; j < n; ++j)
    {
        cout << "Eigenwert:   " << ew[j] << '\n';
        cout << "Eigenvektor: " << ev.col(j) << endl;
    }
    
    return 0;
}