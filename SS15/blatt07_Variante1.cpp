#include <iostream>
#include <iomanip>

#include <cmath>

#include <vector>


using namespace std;

/** Solves the system A x = y with cholesky decomposition.
 * 
 * When finished with true, the value of x lies in y (the former value destroyed),
 * and the upper matrix L lies in A (the former value destroyed).
 * If the trace of A is negative, false will be returned, A and y unchanged.
 * If A is not positive definite, A may have undefined state; y unchanged.
 * On success, the function returns true.
 */
bool cholesky(vector< vector<double> > & A, // in/out: the matrix being changed to L
              vector<double>           & y, // in/out: the vector being changed to x
              double                   eps) // in:     the precision of determine zero.
{
    unsigned const n = A.size(); // alias for smart access to size.
    
    double trace = A[0][0]; // calculate trace (first addend here)
    for (unsigned i = 1; i < n; ++i) trace += A[i][i]; // sum up trace
    if (trace <= 0.0) return false; // if  trace <= 0, A is not positive definite.
    eps *= trace; // Use  eps * trace  as the new zero check.
    
    // L and A indeed share the same data.
    // A will be overwritten and become L.
    // To follow the algorithm, it is concise to have two names.
    vector< vector<double> > & L = A;
    
    // Calculate L, overwrite A.
    // j runs over the column indices of the column being calculated L;
    //    it is not an error that it is used on the row component sometimes on left sides.
    //    This usage follows from the definition of the matrix product  A = L x L*.
    for (unsigned j = 0; j < n; ++j)
    {
        // L[j][j] = A[j][j] already because A = L, so unnecessary to assign.
        // k runs over already calculated values in 
        for (unsigned k = 0; k < j; ++k) A[j][j] -= L[j][k] * L[j][k];
        // if the radicand is < 0 or (which implies that) < eps, we conclude,
        // that the matrix is not positive definite.
        if (L[j][j] < eps) return false;
        // now that we know L[j][j] > 0 for sure, take the quare root.
        L[j][j] = sqrt(L[j][j]);
        
        // i runs over the row indices of cells under the j-j-Cell in the jth-column.
        for (unsigned i = j + 1; i < n; ++i)
        {
            for (unsigned k = 0; k < j; ++k) A[i][j] -= L[i][k] * L[j][k];
            // L[i][j] is A[i][j], therefore L[i][j] = A[i][j] / L[j][j] can be simplified
            L[i][j] /= L[j][j];
        }
    }
    
    // Forward substitution
    // Alike L and A, we use concise aliases for y here...
    vector<double> & z = y;
    for (unsigned i = 0; i < n; ++i)
    {
        // as z = y, z[i] = y[i] - (...) can be simplified.
        for (unsigned k = 0; k < i; ++k) z[i] -= L[i][k] * z[k];
        z[i] /= L[i][i];
    }
    
    // Back substitution
    // alias y here again (z is y, remember?)
    vector<double> & x = z;
    // Here i runs backwards. As it is unsigned, we cannot check "while >= 0"
    // because this is always the case. Therefore we have to check the value
    // before subtraction. If it is zero BEFORE, we have to stop.
    for (unsigned i = n; i-- > 0;)
    {
        // as x = z, x[i] = z[i] - (...) can be simplified.
        for (unsigned k = i + 1; k < n; ++k) x[i] -= L[k][i] * x[k];
        x[i] /= L[i][i];
    }
    
    // It's done when it's done... (R)
    return true;
}

int main()
{
    cout << "Lösen des Gleichungssystems Ax = y." << endl << endl;
    
    // Read some values.
    unsigned n;
    cout << "Es wird eine symmetrische n*n-Matrix eingelesen werden.\n"
            "n: " << flush;
    cin >> n;
    
    unsigned w;
    cout << "Wie viele Ziffern hat die längste Zahl (für schöne Formatierung)?\n"
            "w: " << flush;
    cin >> w;
    
    // Matrix A als Spaltenvektor mit n Zeilenvektoren (je Länge 0) definieren.
    vector<vector<double> > A(n);
    // Den i-ten Zeilenvektor auf Länge i+1 setzen.
    for (unsigned i = 0; i < n; ++i) A[i].resize(i + 1);
    
    // Nun ist A eine "Matrix" der Form
    // 0
    // 0 0
    // : :. 0
    // 0  ...  0
    // Da A symmetrisch sein soll, ist der fehlende Teil auch nicht wichtig.
    // Es ist darauf zu achten, dass auf die fehlenden Einträge nicht zugegriffen
    // wird. (Mit fortgeschrittenen Techniken könnte der Zugriff umgeleitet werden.)
    cout << "Bitte symmetrische Matrix zeilenweise eingeben.\n"
            "Bekannte Werte weden sofort eingesetzt.\n"
            "Rechtsbündige Eingabe ist am übersichtlichsten.\n"
            "Matrix:"
         << endl;
    
    // Es werden in jeder Zeile die bekannten Einträge ausgegeben. Der Benutzer
    // ergänzt dann die übrigen. Die Eingabe erfolgt gespiegelt, d. h. die im obigen
    // Kommentar fehlenden Komponenten + Diagonale werden vom Benutzer eingegeben.
    for (unsigned i = 0; i < n; ++i)
    {
        unsigned j = 0;
        for (; j < i; ++j) cout << setw(w) << A[i][j] << ' ';   // bekannte Werte ausgeben
        for (; j < n; ++j) cin  >>            A[j][i];          // unbekannte Werte einlesen
    }
    
    cout << "Vektoren x und y als Zeilenvektor, Komponenten  mit Leerzeichen getrennt, eingeben." << endl;
    
    // Vektor y als Nullvektor der Länge n definieren.
    vector<double> y(n);
    
    // y einlesen.
    cout << "y = " << flush;
    for (unsigned i = 0; i < n; ++i) cin >> y[i];
    
    // A und y nochmal zur Kontrolle ausgeben.
    cout << "Es wird das folgende Gleichungssystems gelöst:\n" << endl;
    unsigned wn = (n * w + n) / 2 - 1; // halbe Breite der Matrix
    cout << setw(wn + 1)         << right << 'A'
         << setw(wn + 4 + w / 2) << right << 'y' << endl;
    for (unsigned i = 0; i < n; ++i)
    {
        unsigned j = 0;
        for (; j <= i; ++j) cout << setw(w) << A[i][j] << ' ';
        for (; j <  n; ++j) cout << setw(w) << A[j][i] << ' ';
        cout << "| " << setw(w) << y[i] << '\n';
    }
    cout << '\n' << endl;
    
    // Aliase bilden.
    vector< vector<double> > & L = A;
    vector<double> & x = y;
    
    // Berechnen. Funktionswert gibt an, ob die Berechnung erfolgreich war.
    if (cholesky(L, x, 1.0e-10))
    {
        cout << "Die Lösung konnte bestimmt werden:\n"
                "x = ";
        for (unsigned i = 0; i < n; ++i) cout << x[i] << ' ';
        cout << endl;
        
//         cout << "Die zugehörige Dreicksmatrx L ist:\n";
//         
//         for (unsigned i = 0; i < n; ++i)
//         {
//             unsigned j = 0;
//             for (; j <= i; ++j) cout << setw(w) << L[i][j] << ' ';
//             for (; j <  n; ++j) cout << setw(w) << 0.0     << ' ';
//             cout << '\n';
//         }
    }
    else
    {
        cout << "Es konnte keine Lösung bestimmt werden.\n"
                "Womöglich ist die angegebene Matrix singulär oder nicht positiv definit."
             << endl;
    }
}