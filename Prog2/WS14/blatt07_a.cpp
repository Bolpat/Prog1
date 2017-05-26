// Programmentwurf zu Aufgabe 7a, WS 2014/2015
// Programmieren II fuer Mathematiker

// Includedateien
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#define DEBUG

using namespace std;

const unsigned TABLEMAXSIZE = 1 << 12;

template <typename T>
string vec2str(vector<T> const & vec)
{
  ostringstream res("[");
  res << '[';
  if (!vec.empty())
  {
    auto it = vec.begin();
    res << *it;
    while (++it != vec.end())
    {
      res << ", " << *it;
    }
  }
  
  res << ']';
  
  return res.str();
}

template <typename I, typename T>
string map2str(map<I, T> const & m)
{
  ostringstream res;
  res << '[';
  if (!m.empty())
  {
    auto it = m.begin();
    res << '(' << it->first << ", " << it->second << ')';
    while (++it != m.end())
    {
      res << ", " << '(' << it->first << ", " << it->second << ')';
    }
  }
  res << ']';
  return res.str();
}

class LZW {
private:
  vector<string> stdtable;
  string         name_data, name_comp;

public:
  LZW(string name_data = "ein", string name_comp = "aus")
    : name_data(name_data), name_comp(name_comp)
  {
    // Index anlegen und initialisieren
    int i = 0;
#if false // defined DEBUG
    for (int ch = 32, i = 0;   ch <= 126;   ++ch, ++i)
#else
    for (int ch =  0, i = 0;   ch <= 255;   ++ch, ++i)
#endif
    {
      cout << "i = " << i << " ch = " << (char)ch << endl;
      string str(1, (char)ch);
      stdtable.push_back(str);
    }
    vec2str(stdtable);
    cout << endl;
  }
  
  void komprimieren()
  {
#if defined DEBUG
    cout << "Kopiere stdtable" << endl;
#endif     
    auto table = stdtable;
    map<string, int> index;
    
    int j = 0;
    for (auto it = stdtable.begin();  it != stdtable.end();  ++it, ++j)
    {
      index[*it] = j;
    }
    
#if defined DEBUG
    ostringstream ausgabe;
#endif
    
    // Eingabedatei binaer zum Lesen oeffnen
    ifstream ein;
    ein.open(name_data);
    
    // Ausgabedatei als Textdatei zum Schreiben oeffnen
    ofstream aus;
    aus.open(name_comp);
    
    // Kompressionsalgorithmus durchfuehren (Schleife!)
    char   ch;

    ein.read(&ch, 1);
    while (!ein.eof())
    {
#if defined DEBUG
      cout << "Lese " << ch << endl;
#endif
      
      string sequenz(1, ch);
      
#if defined DEBUG
      cout << "Sequenz = " << sequenz << endl;
#endif
      decltype(index.find(sequenz)) pos, pos_;
     
      while ( (pos_ = index.find(sequenz)) != index.end() && !ein.eof())
      {
#if defined DEBUG
        cout << sequenz << " bereits in table -> weiter" << endl;
#endif
        pos = pos_;
        ein.read(&ch, 1);
        
#if defined DEBUG
        cout << "Lese zusätzlich " << ch << endl;
#endif
        
        sequenz.push_back(ch);
        
#if defined DEBUG
        cout << "Sequenz = " << sequenz << endl;
#endif
      }
#if defined DEBUG
      cout << sequenz << " nicht in table" << endl;
      cout << "Schreibe " << pos->second << " (Index von " << pos->first << ") in Datei." << endl;
#endif
      // pos->second() ist der Index der längsten Teilzeichenkette gerade noch in der Tabelle.
      aus << pos->second << " ";
      
#if defined DEBUG
      ausgabe << pos->second << " ";
#endif
      
      if (table.size() < TABLEMAXSIZE)
      {
        index[sequenz] = table.size();
        table.push_back(sequenz);
        
#if defined DEBUG
        cout << "Fuege hinzu: " << sequenz << endl;
#endif
        
        ch = *(sequenz.rbegin());
        sequenz.pop_back();
      }
#if defined DEBUG
      else
      {
        cout << "Tabelle voll!" << endl;
      }
#endif

#if defined DEBUG
      cout << vec2str(table) << endl;
      cout << map2str(index) << endl;
      //cout << i << endl;
#endif
    }

    // Ein- und Ausgabedateien schliessen
    ein.close();
    aus.close();
    
#if defined DEBUG
    cout << "Ausgabe: " << ausgabe.str() << endl;
#endif
    // Groesse der Eingabedatei und geschaetzte Groesse der komprim. Datei ausgeben
  }
  
  void dekomprimieren()
  {
    ifstream ein;
    ein.open(name_comp);
    ofstream aus;
    aus.open(name_data);
    
#if defined DEBUG
    ostringstream ausgabe;
#endif
    
    // stdtable bereits initialisiert.
    vector<string> table = stdtable;
    table.reserve(TABLEMAXSIZE);
    
    while (ein.good())
    {
      int i; ein >> i;

#if defined DEBUG
      cout << vec2str(table) << endl;
      cout << "Index: " << i << endl;
      cout << "Schreibe " << table[i] << endl;
      
      ausgabe << table[i];
#endif
      
      aus << table[i];
      if (table.size() < TABLEMAXSIZE)
      {
        auto pos = ein.tellg();
        int j; ein >> j;
#if defined DEBUG
        cout << "Betrachte Index " << j << endl;
#endif
        string s = table[i];
        
#if defined DEBUG
//#if true
        if (j < table.size())
        {
          cout << j << " bereits in table mit Wert " << table[j] << endl;
          s.push_back(table[j][0]);
        }
        else
        {
          cout << j << " noch nicht in table." << endl;
          s.push_back(table[i][0]);
        }
#else
        s.push_back(table.size() <= j ? table[j][0] : table[i][0]);
#endif
        // table.push_back(s); // does not work, but WHY??
        table.resize(table.size() + 1);
        *table.rbegin() = s;
        
#if defined DEBUG
        cout << "Füge " << *(table.rbegin()) << " zu table hinzu" << endl;
#endif
        
        ein.seekg(pos, ein.beg);
      }
    }
    
#if defined DEBUG
    cout << ausgabe.str() << endl;
#endif
    
    ein.close();
    aus.close();
  }
};

int main()
{
#if defined DEBUG
  cout << "DEBUG" << endl;
#endif
  string inpfile;
  
  cout << "KOMPRESSION" << endl << endl;
  
  cout << "Geben Sie einen Dateinamen ein:" << endl;
  cin >> inpfile;
  LZW lzwK(inpfile, inpfile + ".lzw");
  lzwK.komprimieren();
  
  cout << endl << endl;
  
  //return 0;
  
  cout << "DEKOMPRESSION" << endl << endl;
  cin >> inpfile;
  string outfile(inpfile, inpfile.size()-4);
  if (outfile != ".lzw")
  {
      cout << ".lzw-Datei erwartet." << endl;
      return 0;
  }
  LZW lzwD(inpfile, outfile + "_");
  lzwD.dekomprimieren();
  
  return 0;
}
