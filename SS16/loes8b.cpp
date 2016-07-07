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

bool is_base64_part(char cv[4])
{
    return is_base64_char(cv[0])
        && is_base64_char(cv[1])
        && is_base64_char(cv[2])
        && is_base64_char(cv[3]);
}

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

int count_fin(char cv[4])
{
    if (cv[2] != '=') return 0;
    if (cv[3] != '=') return 1;
    return 2;
}

int main(int argc, char ** argv)
{
    if (argc < 3)
    {
        cerr << "Not enough arguments. Use " << argv[0] << " input-file-name output-file-name" << endl;
        return 1;
    }

    const char b[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    // Eingabestrom mit Datei verknuepfen und oeffnen
    ifstream ein(argv[1]);

    // Ausgabestrom mit Datei verknuepfen und offnen
    ofstream aus(argv[2]);
    
    char cv[4];
    while (ein.read(cv, 4), ein.gcount() > 0)
    {
        if (!(ein.gcount() == 4 && is_base64_part(cv)))
        {
            cerr << "Input file is not base64." << endl;
            return 1;
        }
        
        unsigned char out[] = { 0, 0, 0 };
        switch (count_fin(cv))
        {
        case 2:
            out[2] |= (base64_to_binary(cv[3]) & 0x3F);
            // fallthrough!
        case 1:
            out[2] |= (base64_to_binary(cv[2]) & 0x03) << 6;
            out[1] |= (base64_to_binary(cv[2]) & 0x3C) << 2;
            // fallthrough!
        case 0:
            out[1] |= (base64_to_binary(cv[1]) & 0x0F) << 4;
            out[0] |= (base64_to_binary(cv[0]) & 0x3F) << 2;
        }
        switch (count_fin(cv))
        {
        case 2:
            aus << out[0] << out[1];
            // fallthrough!
        default:
            aus << out[2];
        }
    }
    return 0;
}