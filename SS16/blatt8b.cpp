/* Loesungsentwurf zu Aufgabe 8a, SS 2016
   Programmieren I fuer Mathematiker         

   Aenderung: read+gcount statt readsome verwendet */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

bool is_base64_char(char c)
{
    return 'A' <= c && c <= 'Z'
        || 'a' <= c && c <= 'z'
        || '0' <= c && c <= '9'
        || c == '+' || c == '/'
        || c == '=';
}

/// Base64-Alphabet --> { 0, ..., 63 }
unsigned char base64_to_binary(char c)
{
    if ('A' <= c && c <= 'Z') return  0 + (c - 'A');
    if ('a' <= c && c <= 'z') return 26 + (c - 'a');
    if ('0' <= c && c <= '9') return 52 + (c - '0');
    if (c == '+')             return 62;
    if (c == '/')             return 63;
cerr << "incorrect character: " << (char)c << endl;
    return 0xFF;
}

/// Zaehlt die informationstragenden Zeichen in cv. Das sind alle auszer '='.
int count_valid(char cv[4])
{
    if (cv[3] != '=') return 4;
    if (cv[2] != '=') return 3;
    return 2;
}

/// Liest Base64-Zeichen in buf. buf muss n Zeichen aufnehmen koennen.
/// Nicht-Base64-Zeichen werden uebersprungen.
/// Die Funktion gibt zurueck, wie viele Zeichen in buf geschrieben wurden.
int read_base64(istream & i, char * buf, int n)
{
    int k = 0;
    while (k < n && (i >> *buf) && is_base64_char(*buf))
    {
        ++k;
        ++buf;
    }
    return k;
}

int main(int argc, char ** argv)
{
    if (argc < 3)
    {
        cerr << "Not enough arguments. Use " << argv[0] << " input-file-name output-file-name" << endl;
        return 1;
    }
    
    // Eingabestrom mit Datei verknuepfen und oeffnen
    ifstream ein(argv[1]);
    if (!ein)
    {
        cerr << "Kann Datei '" << argv[1] << "' nicht oeffnen" << endl;
        return 1;
    }

    // Ausgabestrom mit Datei verknuepfen und oeffnen
    ofstream aus(argv[2], ios::binary);
    if (!aus)
    {
        cerr << "Kann Datei '" << argv[1] << "' nicht schreiben" << endl;
        return 1;
    }
    
    // Dekodieren
    char cv[4];
    int count = 0;
    while ((count = read_base64(ein, cv, 4)) == 4)
    {
        unsigned char out[] = { 0, 0, 0 };
        switch (count_valid(cv))
        {
          case 4:
            out[2] |= (base64_to_binary(cv[3]) & 0x3F) >> 0;
            // fallthrough!
          case 3:
            out[2] |= (base64_to_binary(cv[2]) & 0x03) << 6;
            out[1] |= (base64_to_binary(cv[2]) & 0x3C) >> 2;
            // fallthrough!
          default:
            out[1] |= (base64_to_binary(cv[1]) & 0x0F) << 4;
            out[0] |= (base64_to_binary(cv[1]) & 0x30) >> 4;
            out[0] |= (base64_to_binary(cv[0]) & 0x3F) << 2;
        }
        aus.write(reinterpret_cast<char *>(out), count_valid(cv) - 1);
    }
    if (count != 0)
    {
        cerr << "Unvollstaendige Base64-datei" << endl;
        return 1;
    }
    return 0;
}