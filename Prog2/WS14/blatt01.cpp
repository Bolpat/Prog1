/* Rudimentaerer Loesungsentwurf Aufgabe 1
   Programmieren II fuer Mathematiker WS 14/15    */

#include <iostream>
#include <complex>
#include <cmath>

using namespace std;

class Complex
{
private:
    double r, phi;
    
    Complex(double r, double p, bool reduce)
    : r(r), phi(p)
    {
        if (!reduce) return;
        const double TAU = 8.0*atan(1);
        const double PI  = 4.0*atan(1);
        if (r < 0.0)
        {
            phi += PI;
            r = -r;
        }
        while (phi >  PI) phi -= TAU;
        while (phi < -PI) phi += TAU;
    }
    
public:
    Complex(double re = 0.0, double im = 0.0)
    : r(sqrt(re*re + im*im)), phi(atan2(im, re))
    { }

    double re() const { return r * cos(phi); }

    double im() const { return r * sin(phi); }

    friend Complex operator+(Complex const & z1, Complex const & z2)
    {
        const double r   = sqrt( z1.r * z2.r + z2.r * z2.r  +  2.0 * z1.r * z2.r * cos(z1.phi - z2.phi));
        const double phi = atan2( z1.im() + z2.im() , z1.re() + z2.re() );
        return Complex(r, phi, false);
    }

    friend Complex operator-(Complex const & z1, Complex const & z2)
    {
        const double r   = sqrt(z1.r * z1.r + z2.r * z2.r  -  2.0 * z1.r * z2.r * cos(z1.phi - z2.phi));
        const double phi = atan2(z1.im() - z2.im() , z1.re() - z2.re());
        return Complex(r, phi, false);
    }
    
    friend Complex operator*(Complex const & z1, Complex const & z2)
    {
        return Complex(z1.r * z2.r, z1.phi + z2.phi, true);
    }
    
    friend Complex operator/(Complex const & z1, Complex const & z2)
    {
        return Complex(z1.r / z2.r, z1.phi - z2.phi, true);
    }
    
    friend ostream & operator<<(ostream & stream, Complex const & z)
    {
        return stream << '(' << z.re() << ',' << z.im() << ')';;
    }
    
    friend Complex exp (Complex const & z);
    friend Complex sqrt(Complex const & z);
    friend Complex log (Complex const & z);
};

Complex exp(Complex const & z)
{
    return Complex(exp(z.r * cos(z.phi)), z.r * sin(z.phi), true);
}

Complex sqrt(Complex const & z)
{
    return Complex(sqrt(z.r), z.phi/2.0, true);
}

Complex log(Complex const & z)
{
    return Complex(log(z.r), z.phi);
}

Complex pow(Complex const & z1, Complex const & z2)
{
    return exp(z2 * log(z1));
}

int main()
{
    double x1, y1, x2, y2;
    cout << "x1 y1 x2 y2: ";
    cin >> x1 >> y1 >> x2 >> y2;

    Complex         z1(x1, y1), z2(x2, y2);
    complex<double> w1(x1, y1), w2(x2, y2);

    cout << "(z1-z2)/(z1+z2) = " << (z1-z2)/(z1+z2) << endl
         << "                = " << (w1-w2)/(w1+w2) << endl;
    
    cout << "sqrt(z1*z2) = " << sqrt(z1*z2) << endl
         << "              " << sqrt(w1*w2) << endl;

    cout << "exp(z1) = " << exp(z1) << endl
         << "          " << exp(w1) << endl;

    cout << "log(z2) = " << log(z2) << endl
         << "          " << log(w2) << endl;
    
    cout << "pow(z1, z2) = " << pow(z1, z2) << endl
         << "              " << pow(w1, w2) << endl;
        
    return 0;
}
