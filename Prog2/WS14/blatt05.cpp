// Loesungsentwurf zu Aufgabe 5, WS 2014/2015
// Programmieren II fuer Mathematiker

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class dieder
{
private:
    int i;

public:
    dieder(int i = 0)
    : i(i)
    {
        if (!(0 <= i && i < 10)) throw invalid_argument("value must be between 0 and 9");
    }

    dieder inv() const
    {
        static const int inverse[] = { 0, 4, 3, 2, 1, 5, 6, 7, 8, 9 };
        return dieder(inverse[i]);
    }

    dieder operator*(dieder d) const
    {
        const int & j = d.i;

        if (i <= 4)
        {
            if (j <= 4) return dieder((i + j) % 5);
            else        return dieder(5 + (i + j) % 5);
        }
        else
        {
            if (j <= 4) return dieder(5 + (i - j) % 5);
            else        return dieder(i - j < 0  ?  5 + i - j  :  i - j);
        }
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
        if (static_cast<int>(di * dj) == 0)
        {
            cout << static_cast<int>(dj) << ", ";
        }
    }
    cout << endl;
}

/* Berechnet pi^k(i)) fuer k >= 1 */
int perm(int i, int k)
{
    static const int pi[] = { 1, 5, 7, 6, 2, 8, 3, 0, 9, 4 };
    while (k-- > 0) i = pi[i];
    return i;
}

int _main()
{
    invtable();
}

int main()
{
    /* Eingabe lesen, auf String speichern */
    string inp;
    cout << "Geben Sie einen Code ein." << endl;
    cin >> inp;

    vector<int> digits;
    digits.reserve(inp.size());

    /* Eingabestring in Ziffernkette umwandeln */
    for (unsigned k = 0; k < inp.size(); ++k)
    {
        int j = 0;
        switch (inp[k])
        {
            case '9': case 'z': case 'Z': ++j;
            case '8': case 'y': case 'Y': ++j;
            case '7': case 'u': case 'U': ++j;
            case '6': case 's': case 'S': ++j;
            case '5': case 'n': case 'N': ++j;
            case '4': case 'l': case 'L': ++j;
            case '3': case 'k': case 'K': ++j;
            case '2': case 'g': case 'G': ++j;
            case '1': case 'd': case 'D': ++j;
            case '0': case 'a': case 'A':
                digits.push_back(perm(j, k));
                break;
            default: throw string("Character ivalid");
        }
    }

    /* Produkt in Diedergruppe berechnen */
    dieder prod;
    for (unsigned i = 0; i < digits.size(); ++i)
    {
        prod *= dieder(digits[i]);
    }

    /* Ausgabe in Grossbuchstaben mit angehaengter Pruefziffer */
    for (unsigned i = 0; i < inp.size(); ++i)
    {
        if ('a' <= inp[i] && inp[i] <= 'z')
            cout << static_cast<char>(inp[i] - 'a' + 'A');
        else
            cout << inp[i];
    }
    cout << prod.inv() << endl;
    return 0;
}