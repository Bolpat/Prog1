// compile with -std=c++11

// Programmentwurf zu Aufgabe 10, WS 2014/2015
// Programmieren II fuer Mathematiker

// Include- Dateien

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Ort
{
    double phi, psi;
};

constexpr
double rad(double deg) { return deg * (3.1415926535 / 180.0); }

typedef vector<Ort> Kurve;

struct Erde
{
    vector<Kurve> breitenkreise;
    vector<Kurve> laengenkreise;
    vector<Kurve> kuestenlinien;

    Erde(string dateiname = "data.mod")
    {
      ifstream data(dateiname);

      // Breitenkreise erzeugen und speichern
      breitenkreise.reserve(11);
      Kurve k;
      for (int psi = -(90-15); psi <= (90-15); psi += 15)
      {
        k.reserve(360);
        for (int phi = 0; phi < 360; ++)
        {
          k.push_back(Ort(rad(psi),rad(phi));
        }
        breitenkreise.push_back(move(k));
      }
      
      // Laengenkreise erzeugen und speichern
      laengenkreise.reserve(12);
      for (int phi = -(90-15); phi <= (90-15); psi += 15)
      {
        k.reserve(360);
        
      }
      
      // Kuestenlinien einlesen und speichern
      
    }
};


class Ansicht
{
private:
  double R, D, phi, psi;
  double xc, yc, zc, xm, ym, zm;
  double bildbreite;
  double a, b, c, p, q, s, t;
  double rad;  // Radius des Kugelumrisses
  double skal; // Skalierungsfaktor  rad*skal= bildbreite/2
  
  ofstream svgdatei;

public:
  Ansicht(double R_, double D_, double phi_, double psi_, Erde erde, string dateiname="erde.svg") :
    R(R_),D(D_),phi(phi_),psi(psi_)
    {
    // Abbildungsparameter berechnen

    // SVG-Datei oeffnen, SVG-Prolog schreiben

    // Kugelumriss zeichnen (Skalierung!)
      
    // Breitenkreise zeichnen

    // Laengenkreise zeichnen

    // Kuestenlinien zeichnen

    // SVG-Epilog schreiben
    
  }

  void kugelkoord_kartesisch(double r, double phi, double psi,
                            double & x, double & y, double & z)
  {
    
  }

  void zentrproj(double x,double y, double z, double & u, double & v)
  {
    
  }
  
  void ebene_bild(double u, double v, double & bx, double & by)
  {
    
  }

  void kurve_zeichnen(Kurve kurve)
  {
    
    // Ortskoordinaten (Laenge/Breite) in Bildkoordinaten umrechnen
    // und Strecken mit lineto zeichnen
    // Clipping beruecksichtigen (ggf. statt lineto moveto)
    
  }

};




int main()
{
  double R = 1.0,
         D = 5.0,
         phi,
         psi;
  // Betrachterstandort, Kugelradius einlesen
  

  Erde erde("data.mod");
  Ansicht ansicht(R,D,phi,psi,erde,"erde.svg");

  return 0;

}

