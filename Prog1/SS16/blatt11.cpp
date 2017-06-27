#include <iostream>
#include <fstream>
#include <sstream>

#include <limits>

using namespace std;

int fehler(string arg, int exitcode)
{
    // Meldung ausgeben und mit exitcode abbrechen:
    switch (exitcode)
    {
    case 1:
    case 3:
        cerr <<
            "Falsche Argumente."                                                 "\n"
            "Verwendung: " << arg << " [m]-[n] [file]"                           "\n"
            "wobei die Zeilen m bis n einer Textdatei ausgegeben werden. "       "\n"
            "Hierbei duerfen m oder n fehlen, wobei dann "                       "\n"
            "m = 0 bzw. n = Anzahl Zeilen ist."                                  "\n"
            "Es wird nullbasiert gezaehlt und m ist inklusiv, n exklusiv."       "\n"
            "Falls m > n werden die Argumente vertauscht interpretiert."         "\n"
            "Falls der Dateiname fehlt, wird von der Standardeingabe gelesen."   "\n"
            "\n";
        break;
    case 2:
        cerr << "Kann die Datei '" << arg << "' nicht oeffnen."                  "\n"
        "\n";
        break;
    default:
        cerr << "Unbekannter Fehler."                                            "\n"
        "\n";
    }
    cerr << "Fehlercode: " << exitcode << "\n";
    return exitcode;
}


int main(int argc, char *argv[]) 
{
    // Argumentzahl ueberpruefen, ggf. abbrechen
    if (argc < 2) return fehler(argv[0], 3);
    if (argc > 3) return fehler(argv[0], 3);
    if (argv[1][0] == '\0') return fehler(argv[0], 3);

    // Stringstream zu argv[1] definieren
    istringstream iss(argv[1]);

    // m bestimmen:
    // Ein Zeichen vorausschauen, ggf. von Stringstream lesen bzw. auf 1 setzen
    unsigned m = 0;
    if (iss.peek() != '-' && !(iss >> m)) return fehler(argv[0], 1);
    
    // Ueberpruefen, ob '-' vorhanden, andernfalls Fehlerabbruch
    if (iss.peek() != '-' || !iss.ignore(1)) return fehler(argv[0], 1);

    // n bestimmen:
    // Ein Zeichen vorausschauen, ggf. von Stringstream lesen bzw. auf groesste Integerzahl setzen
    unsigned n = numeric_limits<unsigned>::max();
    // Ein Zeichen lesen, falls danach Stringstromzustand nicht eof Fehlerabbruch
    if (!(iss >> n).eof()) return fehler(argv[0], 1);
    
    // Datei mit Namen argv[2] oeffnen, ggf. Fehlerbehandlung
    ifstream file;
    if (argc == 3 && (file.open(argv[2]), !file)) return fehler(argv[2], 2);
    istream & inp = argc == 3 ? file : cin;

    // Falls m > n: Nichts ausgeben.
    if (m > n) return 0;

    // Datei mit getline lesen, Zeilen zaehlen und nur Zeilen im gewuenschten Bereich ausgeben
    string line;
    while (m-- > 0 && getline(inp, line)) --n;
    while (n-- > 0 && getline(inp, line)) cout << line << endl;
}
