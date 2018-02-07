// Loesungsvorschlag zu Aufgabe 11a+b, WS 2017/2018
// Programmieren II fuer (Wirtschafts-)Mathematiker

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#ifdef DEBUG
#define DEBUG_LOG(expression) expression
#else
#define DEBUG_LOG(expression) {}
#endif

using namespace std;

class LZW {
private:
    // compression index
    map<string, size_t> index;
    // decompression table
    vector<string> tabelle;
    
    ifstream ein;
    ofstream aus;
    
    string name_ein, name_aus;
    
public:
    LZW(string name_ein = "ein", string name_aus = "aus")
    : name_ein(name_ein), name_aus(name_aus), tabelle(256)
    {
        // Index initialisieren (fuer Kompression)
        for (size_t i = 0; i < 256; ++i) index[string(1, static_cast<char>(i))] = i;

        // Tabelle initialisieren (fuer Dekompression)
        for (size_t i = 0; i < 256; ++i) tabelle[i] = static_cast<char>(i);
    }
    
    void komprimieren()
    {
        
        // ios::in is always set for ifstream objects (even if explicitly not set in argument mode).
        // Note that even though ifstream is an input stream, its internal filebuf object may be set to also support output operations
        // See http://www.cplusplus.com/reference/fstream/ifstream/open/
        ein.open(name_ein.c_str(), ios::binary | ios::in);
        aus.open(name_aus.c_str());
        
        string w;
        map<string,size_t>::iterator pos;
        char z;
        int i, write_count = 0, read_count = 0;
        ein >> noskipws;
        while (ein >> z)
        {
            read_count += 1;
            w += z;
DEBUG_LOG(cerr << "z '" << z << "'\n");
DEBUG_LOG(cerr << "w '" << w << "'\n");
            if ((pos = index.find(w)) != index.end()) 
            {
DEBUG_LOG(cerr << "'" << w << "' in table; index is " << pos->second << '\n');
                // w in Tabelle
                // Index merken
                i = pos->second;  // etwas unschoen
                // continue;
            }
            else
            {
DEBUG_LOG(cerr << "'" << w << "' not in table\n");
                // w nicht in Tabelle
                aus << setw(5) << i;   // Textausgabe
DEBUG_LOG(cerr << "OUTPUT " << i << "\n");
                if (++write_count % 16 == 0) aus << endl;
                if (index.size() < 4096)
                {
DEBUG_LOG(cerr << "add '" << w << "' to table as index " << index.size() << '\n');
                    int nind = index.size();
                    index[w] = nind;
                }
                // Zuruecksetzen fuer Neustart
                //ein.unget();
                //read_count -= 1;
                // TODO: make backup of i
                w = z;
            }
        }
        // Falls EOF restlichen Index ausgeben
        if (w != "")
        {
            aus << setw(5) << i << endl; 
            write_count++;
        }
        
        ein.close();
        aus.close();
        
        cerr << "Anzahl der gelesenen Zeichen: " << read_count << endl;
        cerr << "Geschaetzte Groesse der komprimierten Datei: " << (write_count + write_count / 2) << endl;
    }

    void dekomprimieren() {
        ein.open(name_ein.c_str());
        aus.open(name_aus.c_str(), ios::binary | ios::trunc);
        
        size_t i;
        string w = "";
        while (ein >> i)
        {
            unsigned j;
        ruecksprung:
            aus << tabelle[i];
            if ( (tabelle.size() < 4096) && (ein >> j) )
            {
                if (j < tabelle.size() )
                {
                    // W(j) belegt
                    w = tabelle[i] + tabelle[j].substr(0, 1);
                }
                else
                {
                    // W(j) nicht belegt
                    w = tabelle[i]+tabelle[i].substr(0, 1);
                }
                tabelle.push_back(w);
                i = j;
                goto ruecksprung;
            }
        }
        ein.close();
        aus.close();
    }
};


int main()
{
    string name_ein;
    cout << "Name der zu komprimierenden Datei: ";
    cin >> name_ein;
    
    LZW lzw(name_ein, name_ein + ".ind");
    lzw.komprimieren();
    
    // LZW lzw2(name_ein + ".ind", name_ein + "2");
    // cout << "Name der Indexdatei: " << (name_ein + ".ind") << endl;
    // lzw2.dekomprimieren();
    // cout << "Ruecktransformierte Datei: " << (name_ein + "2") << endl;
    
    return 0;
}
