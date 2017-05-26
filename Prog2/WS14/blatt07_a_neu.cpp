// Programmentwurf zu Aufgabe 7a, WS 2014/2015
// Programmieren II fuer Mathematiker

// Includedateien
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

// #define DEBUG

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
  string         name_data,
                 name_comp;

public:
  LZW(string name_data = "ein", string name_comp = "aus")
    : name_data(name_data), name_comp(name_comp)
  {
    // Index anlegen und initialisieren

    for (int ch =  0, i = 0;   ch <= 255;   ++ch, ++i)
    {
      string str(1, (char)ch);
      stdtable.push_back(str);
    }
  }
  
  void komprimieren()
  {
    auto table = stdtable;
    map<string, int> index;
    
    int j = 0;
    for (auto it = stdtable.begin();  it != stdtable.end();  ++it, ++j)
    {
      index[*it] = j;
    }
    
    // Eingabedatei binaer zum Lesen oeffnen
    ifstream ein;
    ein.open(name_data, ifstream::binary);
    if (!ein.good()) return;
    
    // Ausgabedatei als Textdatei zum Schreiben oeffnen
    ofstream aus;
    aus.open(name_comp);
    
    // Kompressionsalgorithmus durchfuehren
    char ch;
    
    ein.read(&ch, 1);
    while (ein.good())
    {
      string sequenz(1, ch);
      
      decltype(index.find(sequenz)) pos, pos_;
    
      while ( (pos_ = index.find(sequenz)) != index.end() && !ein.eof() )
      {
        pos = pos_;
        ein.read(&ch, 1);
        sequenz.push_back(ch);
      }
      // pos->second() ist der Index der längsten Teilzeichenkette gerade noch in der Tabelle.
      // pos_ ist auf jeden Fall index.end()
      
      aus << pos->second << " ";
      
      if (table.size() < TABLEMAXSIZE)
      {
        index[sequenz] = table.size();
        table.push_back(sequenz);
        
        ch = *(sequenz.rbegin());
        sequenz.pop_back();
      }
    }

    // Ein- und Ausgabedateien schliessen
    ein.close();
    aus.close();
    // Groesse der Eingabedatei und geschaetzte Groesse der komprim. Datei ausgeben
  }
  
  void dekomprimieren()
  {
    ifstream ein;
    ofstream aus;
    
    ein.open(name_comp);
    aus.open(name_data);
    
    // stdtable bereits initialisiert.
    vector<string> table = stdtable;
    table.reserve(TABLEMAXSIZE);
    
    while (ein.good())
    {
      int i; ein >> i;
      
      aus << table[i];
      if (table.size() < TABLEMAXSIZE)
      {
        auto pos = ein.tellg();
        int j; ein >> j;
        string s = table[i];
        s.push_back(table.size() <= j ? table[j][0] : table[i][0]);
        // table.push_back(s); // does not work, but WHY??
        table.resize(table.size() + 1);
        *table.rbegin() = s;
        ein.seekg(pos, ein.beg);
      }
    }
    
    ein.close();
    aus.close();
  }
};

int main()
{
  string inpfile;
  
  cout << "KOMPRESSION" << endl << endl;
  
  cout << "Geben Sie einen Dateinamen ein:" << endl;
  cin >> inpfile;
  LZW lzwK(inpfile, inpfile + ".lzw");
  lzwK.komprimieren();
  
  cout << endl << endl;
  return 0;
  cout << "DEKOMPRESSION" << endl << endl;
  
  LZW lzwD("aus.dat", "comp.lzw");
  lzwD.dekomprimieren();
  
  return 0;
}
