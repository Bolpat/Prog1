/* Loesungsentwurf zu Aufgabe 8a, SS 2016
   Programmieren I fuer Mathematiker         

   Aenderung: read+gcount statt readsome verwendet */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char ** argv)
{
    if (argc < 3)
    {
        cerr << "Not enough arguments. Use " << argv[0] << " input-file-name output-file-name" << endl;
        return 1;
    }

    const char b[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    // Eingabestrom mit Datei verknuepfen und oeffnen
    ifstream ein(argv[1], ios::binary);

    // Ausgabestrom mit Datei verknuepfen und offnen
    ofstream aus(argv[2]);

    // Umwandlung binaer->base64 durchführen
    int blocks = 19;
    char cv[3];   // zur Benutzung von read
    while (ein.read(cv, 3), ein.gcount() > 0)
    {
        unsigned int u[] = { 0, 0, 0, 0 };
        switch (ein.gcount())
        {
          case 3:
            u[3] |= (cv[2] & 0x3F) << 0;
            u[2] |= (cv[2] & 0xC0) >> 6;
            // fallthrough!
          case 2:
            u[2] |= (cv[1] & 0x0F) << 2;
            u[1] |= (cv[1] & 0xF0) >> 4;
            // fallthrough!
          case 1:
            u[1] |= (cv[0] & 0x03) << 4;
            u[0] |= (cv[0] & 0xFC) >> 2;
        }
        char out[] = "====";
        switch (ein.gcount())
        {
          case 3:
            out[3] = b[u[3]];
            // fallthrough!
          case 2:
            out[2] = b[u[2]];
            // fallthrough!
          case 1:
            out[1] = b[u[1]];
            out[0] = b[u[0]];
        }
        aus << out;
        if (--blocks == 0)
        {
            aus << endl;
            blocks = 19;
        }
    }
    if (blocks != 0) aus << endl;
    return 0;
}
