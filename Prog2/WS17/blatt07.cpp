#include <iostream>
#include <iomanip>
#include <vector>
#include <complex>
#include <fstream>
#include <string>

using namespace std;

using Complex = complex<double>;

// Beispiele
Complex f1 (Complex z) { }

Complex f1s(Complex z) { }

using CFunc = Complex(Complex);

bool newtonverfahren(Complex z0, Complex& z, CFunc f, CFunc fs) { }

class Farbe {
private:
    char r,g,b;
public:
    Farbe() {}  
    Farbe(char r_, char g_, char b_) {}
    friend ostream& operator<<(ostream& stream, Farbe farbe) { }
};


class Konvergenz{
private:
    CFunc fp;
    CFunc fderivp;
    double a,b,c,d,D;
    int M,N;
    string fname;
    ofstream aus;
    vector<Farbe> farbtabelle;
    vector<Complex> nullstellen;
    vector<int> haeufigkeiten;
 
public:
    Konvergenz(CFunc fp, CFunc fderivp, double a, double b, double c, double d, int M, int N, double D, string fname) 
        : fp(fp), fderivp(fderivp), a(a), b(b), c(c), d(d), M(M), N(N), D(D), fname(fname)
    {
        // 8 Farben definieren
        farbtabelle =
            {
                {   0,   0,   0 },
                { 255,   0,   0 },
                {   0, 255,   0 },
                {   0,   0, 255 },
                {   0, 255, 255 },
                { 255,   0, 255 },
                { 255, 255,   0 },
                { 255, 255, 255 }
            };
        // Ausgabedatei oeffnen und Header schreiben
        aus(fname);
    }

    void analysieren() {
        // Gitter abarbeiten
        
        // Nullstellen, Haeufigkeiten und Name der Ausgabedatei ausgeben
    }
};


int main() 
{ 
    double a,b,c,d,D;
    int M,N;
    
    // Gitterdaten einlesen
    
    Konvergenz konvergenz1(f1,f1s,a,b,c,d,M,N,D,"aus_f1.ppm");
    konvergenz1.analysieren();

    return 0;
}