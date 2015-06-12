#include <iostream>
#include <iomanip>

#include <cerrno>
#include <cstring>

using namespace std;



/// Konstante, die die Zahl 10...0 beschreibt.
unsigned const HI_BIT = compl (compl 0u >> 1);


/// Schreibt die Bits von u auf die Standardausgabe.
void printbits(unsigned u)
{
    for (unsigned testBit = HI_BIT; testBit != 0u; testBit >>= 1)
        cout << (u & testBit ? '1' : '0');
}

/// Addiert a und b.
unsigned add_1a(unsigned a, unsigned b)
{
    unsigned h = a ^ b, // Halbaddition
             u = a & b; // Prä-Übertrag
    if (u == 0u) return h; // Trivialen Fall abfangen.
    if (u & HI_BIT)     // Auf Überlauf testen (Prä-Übertrag hat ganz vorne eine 1)
    {
        errno = ERANGE;
        return 0u;
    }
    // Mit Endrekursion Ergebnis auf trivialen Fall reduzieren.
    return add_1a(h, u << 1);
}

/// Addiert a und b.
unsigned add_1b(unsigned a, unsigned b)
{
  begin:
    unsigned h = a ^ b, // Halbaddition
             u = a & b; // Prä-Übertrag
    if (u == 0u) return h; // Trivialen Fall abfangen.
    if (u & HI_BIT)     // Auf Überlauf testen (Prä-Übertrag hat ganz hinten eine 1)
    {
        errno = ERANGE;
        return 0u;
    }
    
    a = h;              // |
    b = u << 1;         //  > Endrekursion aufgelöst.
    goto begin;         // |
}

/// Addiert a und b.
unsigned add_2(unsigned a, unsigned b)
{
    while (b != 0)
    {
        unsigned h = a ^ b,      // Berechnen...
                 u = a & b;      // ...wie oben.
        if (u & HI_BIT) // Fehlerbehandlung wie oben.
        {
            errno = ERANGE;
            return 0u;
        }
        a = h;          // Für den nächsten Schritt a und b...
        b = u << 1;     // ...auf die entsprechenden Werte setzen.
    }
    return a;
}


/// Multipliziert a und b.
unsigned mult(unsigned a, unsigned b)
{
    unsigned sum = (a & 1u) ? b : 0u;  // Summe initialisieren.
    
    // a nach rechts schieben, d. h. nächstes Bit verarbeiten.
    while ((a >>= 1) != 0u)
    {
        if (b & HI_BIT) // Da a != 0 gilt, muss dies einen Überlauf darstellen.
        {
            errno = ERANGE;
            return 0u;
        }
        b <<= 1;        // b nach links schieben, d. h. verdoppeln.
        
        if (a & 1u)     // Falls das unterste Bit von a gesetzt ist...
        {               // ...b zur Summe hinzuaddieren.
            sum = add_2(sum, b);
            if (errno) return 0u; // Evtl. auftretenden Fehler fangen und propagieren.
        }
    }
    return sum;
}


int main()
{
    // a und b einlesen.
    cout << "a, b (hexadezimal): " << flush;
    unsigned a, b;
    cin >> hex >> a >> b;
    
    // Multiplikation mit Fehlerbehandlung durchführen
    errno = 0;
    unsigned m = mult(a, b);
    
    switch (errno)
    {
      case 0: // Fehlerfreier Aufruf.
        cout << "Dezimal:\n"     << dec
             << "mult(a, b)  =  " << m       << '\n'
             << "a * b       =  " << (a * b) << '\n' << endl;
        cout << "Hexadezimal:\n" << hex;
        cout << "mult(a, b)  =  " << m       << '\n'
             << "a * b       =  " << (a * b) << '\n' << endl;
        break;
        
      case ERANGE: // Wertebereichsfehler, d. h. Überlauf. Damit ist zu rechnen.
        cout << "Überlauf!" << endl;
        break;
        
      default: // bei anderen Fehlern können wir a-priori keine Ursache angeben.
        cout << "Unerwartete Ausnahme. Details: " << strerror(errno) << endl;
    }
}

