// Loesungsentwurf zu Aufgabe 5, WS 2014/2015
// Programmieren II fuer Mathematiker

#include <iostream>
#include <vector>
#include <string>

/* Weitere Includedateien */



using namespace std;

class dieder
{
private: 
    int i;
    
public:
    dieder(int _i = 0): i(_i) { }
    
    dieder inv() const
    {
        static const int inverse[] = { 0,  4, 3, 2, 1,  5,  6, 7, 8, 9 };
        return dieder(inverse[i]);
    }
    
    dieder operator*(dieder d) const
    {
        const int & j = d.i;
        
        if (i <= 4)
            if (j <= 4) return dieder((i+j) % 5);
            else        return dieder(5 + ((i+j) % 5));
            
        else // 5 <= i
            if (j <= 4) return dieder(5 + ((i-j) % 5));
            else        return dieder(i-j < 0  ?  5+i-j  :  i-j);
    }
    
    dieder & operator*=(dieder d) { return *this = *this * d; }
    
    bool operator==(dieder d) const { return i == d.i; }
    
    operator int() const { return i; }
};

// Wurde verwendet um die Tabelle mit den Inversen zu bestimmen
void invtable()
{
    for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 10; ++j)
    {
        dieder di(i), dj(j);
        if ((int)(di*dj) == 0) cout << (int)dj << ", ";
    }
    cout << endl;
}

/* Berechnet pi^k(i)) fuer k >= 1 */
int perm(int i, int k)
{
    static const int pi[] = { 1, 5, 7, 6, 2, 8, 3, 0, 9, 4 };
    for (; k > 0; --k) i = pi[i];
    return i;
}

int main()
{
    invtable();
}

int _main()
{
    string inp;
    vector<int> digits;
    
    /* Eingabe lesen, auf String speichern */
    cout << "Geben Sie einen Code ein." << endl;
    cin >> inp;
    digits.reserve(inp.size());
    
    /* Eingabestring in Ziffernkette umwandeln */
    for (unsigned k = 0; k < inp.size(); ++k)
    {
        if (isdigit(inp[k])) 
        {
            digits.push_back(perm(inp[k] - '0', k));
        }
        else
        {
            int j = 0;
            switch (inp[k])
            {
                case 'z': case 'Z': ++j;
                case 'y': case 'Y': ++j;
                case 'u': case 'U': ++j;
                case 's': case 'S': ++j;
                case 'n': case 'N': ++j;
                case 'l': case 'L': ++j;
                case 'k': case 'K': ++j;
                case 'g': case 'G': ++j;
                case 'd': case 'D': ++j;
                case 'a': case 'A': digits.push_back(perm(j, k));
                break;
                default: throw string("Character ivalid");
            }
        }
    }
    
    /* Produkt in Diedergruppe berechnen */
    dieder d;
    for (unsigned i = 0; i < digits.size(); ++i)
    {
        d *= dieder(digits[i]);
    }
    
    /* Ausgabe in Grossbuchstaben mit angehaengter Pruefziffer */
    
    for (unsigned i = 0; i < inp.size(); ++i)
    {
        cout << (char)( ('a' <= inp[i] && inp[i] <= 'z') ? inp[i] - 32
        : inp[i] );
    }
    cout << d.inv();
    cout << endl;
    return 0;
}