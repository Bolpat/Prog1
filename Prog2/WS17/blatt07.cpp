#include <iostream>
#include <iomanip>
#include <vector>
#include <complex>
#include <fstream>
#include <string>

using namespace std;

static const complex<double> I = complex<double>(0.0, 1.0);

// Examples
complex<double> f1 (complex<double> z)
{
    return z*z*z - 1.0;
}
complex<double> f1d(complex<double> z)
{
    return 3.0*(z*z);
}

complex<double> f2 (complex<double> z)
{
    return (z + 3.0)*(z*z) - I; // z^3 + 3z^2 + i
}
complex<double> f2d(complex<double> z)
{
    return (3.0*z + 6.0)*z;
}

complex<double> f3 (complex<double> z)
{
    return ((((z - 2.0*I)*z - 13.0)*z + 14.0*I)*z + 24.0)*z - 1.0;
}
complex<double> f3d(complex<double> z)
{
    return (((5.0*z + 8.0*I)*z - 39.0)*z + 28.0*I)*z + 24.0;
}

complex<double> f4 (complex<double> z)
{
    return exp(z) * (z*z*z - 1.0);
}
complex<double> f4d(complex<double> z)
{
    return f4(z) + 3.0*(z*z) * exp(z);
}

/// Performs Newton-Raphson method to find the zero of an analytic function.
/// Returns if convergence is sufficiant.
bool newton
(
    complex<double> z0,  /// the starting point
    complex<double> & z, /// the zero (being calculated)
    complex<double>(*f )(complex<double>),  /// the function
    complex<double>(*fd)(complex<double>), /// the derivative of the function
    double epsilon, /// the epsilon used to determine convergence
    unsigned itmax  /// the iteration bound
)
{
    z = z0;
    do
    {
        z0 = z;
        z = z - f(z) / fd(z);
        if (itmax-- == 0) return false;
    }
    while (abs(z - z0) > epsilon*abs(z0));
    return true;
}

class Color
{
    unsigned char r, g, b;
public:
    Color() = default;
    Color(unsigned char r, unsigned char g, unsigned char b)
        : r(r), g(g), b(b)
    { }

    friend ostream& operator<<(ostream& stream, Color color)
    {
        return stream.put(color.r).put(color.g).put(color.b);
    }
};


class Convergence
{
    complex<double>(*func )(complex<double>);
    complex<double>(*deriv)(complex<double>);
    double
        l_bound,
        r_bound,
        b_bound,
        t_bound,
        conv_epsilon,
        dist_epsilon;
    unsigned
        v_resulution,
        h_resulution;
    string
        fname;
    ofstream
        out;
    vector<Color>
        color_table =
        {
            { 255,   0,   0 },
            {   0, 255,   0 },
            {   0,   0, 255 },
            {   0, 255, 255 },
            { 255,   0, 255 },
            { 255, 255,   0 },
            {   0,   0,   0 },
            { 255, 255, 255 }
        };
    vector<complex<double>>
        zeros;
    vector<unsigned>
        zero_counts;
    unsigned
        divergent_count = 0;

public:
    Convergence
    (
        complex<double>(*func )(complex<double>), /// the function
        complex<double>(*deriv)(complex<double>), /// derivative of func
        double l_bound, /// left bound
        double r_bound, /// right bound
        double b_bound, /// bottom bound
        double t_bound, /// top bound
        unsigned v_resulution, /// the number of rows in the grid
        unsigned h_resulution, /// the number of columns in the grid
        double conv_epsilon, /// the epsilon used to determine convergence
        double dist_epsilon, /// the epsilon used to determine equality of zeros
        string fname /// the name of the file to be written to
    )
        : func(func)
        , deriv(deriv)
        , l_bound(l_bound)
        , r_bound(r_bound)
        , b_bound(b_bound)
        , t_bound(t_bound)
        , v_resulution(v_resulution)
        , h_resulution(h_resulution)
        , conv_epsilon(conv_epsilon)
        , dist_epsilon(dist_epsilon)
        , fname(fname)
        , out(fname)
    {
        if (!out)
        {
            throw ios_base::failure("Could not open file '" + fname + "'.");
        }
        out << "P6\n";
        out << v_resulution << ' ' << h_resulution << '\n';
        out << "255\n";
    }

    void analyze()
    {
        for (unsigned j = 0; j < v_resulution; ++j)
        for (unsigned i = 0; i < h_resulution; ++i)
        {
            complex<double> z0
                {
                    l_bound + i*(r_bound - l_bound)/v_resulution,
                    t_bound - j*(t_bound - b_bound)/h_resulution
                };
            if (newton(z0, z0, func, deriv, conv_epsilon, 200))
            {
                // convergence
                bool found = false;
                // z0 became the zero
                unsigned k = 0;
                for (auto z : zeros) if (abs(z - z0) < dist_epsilon)
                {
                    found = true;
                    zero_counts[k++] += 1;
                    out << color_table.at(k);
                    break; // cannot trivially continue the loop over i
                }
                if (!found) // didn't find a zero
                {
                    out << color_table.at(zeros.size());
                    zeros.push_back(z0);
                    zero_counts.push_back(1);
                }
            }
            else
            {
                // divergence
                out << Color(255, 255, 255);
                divergent_count += 1;
            }
        }

        cout << fname << ":\n";
        cout << "Found convergence to:\n";
        cout.precision(3);
        unsigned k = 0;
        for (auto z : zeros)
        {
            cout << "    " << setw(8) << z << ", " << zero_counts[k++] << " times\n";
        }
        cout << "Found divergence " << divergent_count << endl;
    }
};

int main(int argc, char** argv)
{
    double l_bound, r_bound, b_bound, t_bound;
    cout << "left, right, lower and upper bound: ";
    cin >> l_bound >> r_bound >> b_bound >> t_bound;

    unsigned v_resulution, h_resulution;
    cout << "vertical and horizontal resolution: ";
    cin >> v_resulution >> h_resulution;
    
    string file_name;
    cout << "File to be written to: ";
    cin >> file_name;

    complex<double>(*f )(complex<double>);
    complex<double>(*fd)(complex<double>);
    unsigned i;
    cout << "Function to analyze: ";
    cin >> i;
    switch (i)
    {
        case 1: f = f1; fd = f1d; break;
        case 2: f = f2; fd = f2d; break;
        case 3: f = f3; fd = f3d; break;
        case 4: f = f4; fd = f4d; break;
    }
    
    Convergence c(f, fd, l_bound, r_bound, b_bound, t_bound, v_resulution, h_resulution, 1e-12, 0.001, file_name);
    c.analyze();

    return 0;
}