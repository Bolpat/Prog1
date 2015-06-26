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
 */
bool cholesky(vector< vector<double> > & A, // in/out: the matrix being changed to L
              vector<double> & y,           // in/out: the vector being changed to x
              double eps)                   // in:     the precision of determine zero.
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

#include <fstream>

int main(int, char ** argv)
{
    if (argv[1] == NULL)
    {
        cout << "Verwendung: " << argv[0] << " <Dateiname>" << endl;
        return 0;
    }
    ifstream in(argv[1]);
    
    if (not in) { cout << "Fehler beim öffnen!" << endl; return -1; }
    
    int n; if (not(in >> n)) { cout << "Fehler beim lesen!" << endl; return -1; }
    int w; if (not(in >> w)) { cout << "Fehler beim lesen!" << endl; return -1; }
    
    vector< vector<double> > A(n);
    vector<double> y(n);
    
    for (int i = 0; i < n; ++i)
        A[i].resize(i + 1);
    
    for (int i = 0; i <  n; ++i)
    {
        for (int j = 0; j <= i; ++j)
            if (not (in >> A[i][j])) { cout << "Fehler beim lesen!" << endl; return -1; }
        if (not (in >> y[i])) { cout << "Fehler beim lesen!" << endl; return -1; }
    }
    
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
    
    
    cout << "Lösung:\n"
            "x = ";
    if (cholesky(A, y, 1e-10))
    {
        for (int i = 0; i < n; ++i) cout << y[i] << ' ';
        cout << endl;
    }
    else
    {
        cout << "Fehler bei der Berechnung." << endl;
    }
    return 0;
}