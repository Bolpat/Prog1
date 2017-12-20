// Programmentwurf zu Aufgabe 8, WS 2017/2018
// Programmieren II fuer (Wirtschafts-)Mathematiker

// Include- Dateien

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Ort {
    double phi, psi;
};

using Kurve = vector<Ort>;

class Ansicht;

class Erde {
  private:
    vector<Kurve> breitenkreise;
    vector<Kurve> laengenkreise;
    vector<Kurve> kuestenlinien;

  public:
    Erde(string dateiname="data.mod") {

      // Breitenkreise erzeugen und speichern

      // Laengenkreise erzeugen und speichern

      // Kuestenlinien einlesen und speichern

    }
   
    friend class Ansicht;
};


class Ansicht {
  private:
    double R,D,phi,psi;
    double xc,yc,zc,xm,ym,zm;
    double bildbreite;
    double a,b,c,p,q,s,t;
    double rad;  // Radius des Kugelumrisses
    double skal; // Skalierungsfaktor  rad*skal= bildbreite/2
    ofstream svgdatei;

    void kugelkoord_kartesisch(double r, double phi, double psi,
                  double& x, double& y, double& z) {

    }

    void zentrproj(double x,double y, double z, double& u, double& v) {

    }
    
    void ebene_bild(double u, double v, double& bx, double& by) {

    }

    void kurve_zeichnen(Kurve kurve) {

      // Ortskoordinaten (Laenge/Breite) in Bildkoordinaten umrechnen
      // und Strecken mit lineto zeichnen
      // Clipping beruecksichtigen (ggf. statt lineto moveto)

    }

  public:
    Ansicht(double R, double D, double phi, double psi, Erde erde, string dateiname = "erde.svg")
    : R(R),D(D),phi(phi),psi(psi)
    {
      // Abbildungsparameter berechnen

      // SVG-Datei oeffnen, SVG-Prolog schreiben

      // Kugelumriss zeichnen (Skalierung!)
        
      // Breitenkreise zeichnen

      // Laengenkreise zeichnen

      // Kuestenlinien zeichnen

      // SVG-Epilog schreiben
      
    }
};




int main()
{
  double R,D,phi,psi;

  // Betrachterstandort, Kugelradius einlesen

  Erde erde("data.mod");
  Ansicht ansicht(R,D,phi,psi,erde,"erde.svg");

  return 0;

}

