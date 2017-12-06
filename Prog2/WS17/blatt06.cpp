#include <iostream>
#include <iomanip>
#include <list>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <exception>

using namespace std;

class Polynom
{
    struct Monom
    {
        long a; /// coefficient
        unsigned d; /// degree

        constexpr Monom(long a = 0, unsigned d = 0)
        : a(a), d(d) { }

        constexpr Monom operator-() const
        {
            return Monom(-a, d);
        }
        constexpr friend Monom operator*(Monom const & m, Monom const & n)
        {
            return Monom(m.a * n.a, m.d + n.d);
        }
        
        friend ostream & operator<<(ostream & stream, Monom const & m)
        {
            if (m.a == 0) return stream << '0';
            if (m.a == -1) stream << '-';
            else if (m.a !=  1 || m.d == 0) stream << m.a;
            if (m.d > 0) stream << 'x';
            if (m.d > 1) stream << '^' << m.d;
            return stream;
        }
    };

    list<Monom> monoms;
    // Invariants:
    //  is sorted by .d
    //  does not contain two elements with same .d
    //  does not contain monom with .a == 0

public:
    Polynom(long a, unsigned d = 0)
    : monoms(a == 0 ? 0 : 1, Monom(a, d))
    { }

    Polynom(Monom const & monom = Monom())
    : monoms(monom.a == 0 ? 0 : 1, monom)
    { }

    friend Polynom operator-(Polynom p) // copy p
    {
        for (auto & monom : p.monoms) monom = -monom;
        return p;
    }
    friend Polynom operator+(Polynom p, Polynom q) // copy p, q
    {
        p.monoms.merge(q.monoms, [](Monom m, Monom n) { return m.d < n.d; });

        if (p.monoms.size() < 2) return p;

        auto it1 = p.monoms.begin();
        auto it2 = it1;  ++it2;
        for (; it2 != p.monoms.end(); ++it1, ++it2)
        if (it1->d == it2->d)
        {
            it1->a += it2->a;
            it2->a = 0;
        }

        p.monoms.remove_if([](Monom m) { return m.a == 0; });

        return p;
    }

    inline friend Polynom operator-(const Polynom & p, const Polynom & q)
    {
        return p + (-q);
    }

    friend Polynom operator*(Polynom const & p, Polynom const & q)
    {
        Polynom result;
        for (auto const & m : p.monoms)
        for (auto const & n : q.monoms)
            result += m * n;
        return result;
    }

    // optimized add for monoms used in operator*
    Polynom & operator+=(Monom m)
    {
        // place m in a list to track it with an iterator
        list<Monom> temp = { m };
        auto it = temp.begin(); // track m with iterator
        monoms.merge(temp, [](Monom m, Monom n) { return m.d < n.d; });
        // merge will place m behind equivalent monoms.
        // the invariants of the list enforce:
        // if there is a monom equivalent to m, it is directly before it.

        // if m is the first in the merged list, no equivalent monoms exist.
        if (it-- == monoms.begin()) return *this;
        // if the predecessor of m is equivalent to it ...
        if (it->d == m.d)
        {
            // ... add the elements, and if the sum equals 0 ...
            if ((it->a += m.a) == 0)
                monoms.erase(it); // ... remove it from the list which puts the iterator to the next element ...
            else
                ++it; // ... otherwise put iterator to the next element which is m manually ...
            monoms.erase(it); // ... and erase it.
        }
        return *this;
    }

    /// binäre exponentiation
    friend Polynom pow(Polynom p, unsigned n)
    {
        if (n == 0) return Polynom(1);
        unsigned h = compl(compl(0u) >> 1);
        while ((n & h) == 0) h >>= 1;
        Polynom result = p;
        while (h >>= 1)
        {
            result *= result;
            if (n & h) result *= p;
        }
        return result;
    }

    Polynom & operator+=(Polynom p) { return *this = *this + p; }
    Polynom & operator-=(Polynom p) { return *this = *this - p; }
    Polynom & operator*=(Polynom p) { return *this = *this * p; }

    friend ostream & operator<<(ostream & stream, Polynom const & p)
    {
        if (p.monoms.size() == 0) return stream << '0';
        
        auto it = p.monoms.crbegin();
        stream << *it++;
        for (; it != p.monoms.crend(); ++it)
        {
            if (it->a > 0) stream << '+';
            stream << *it;
        }
        return stream;
    }
};

class parse_error : public std::runtime_error
{
    size_t pos_;
public:
    parse_error(string message, size_t pos = 0)
    : std::runtime_error(message), pos_(pos)
    { }

    size_t pos() const { return pos_; }
};

class Expression
{
private:
    size_t size;
    istringstream ein;

    /// expression interpretation from ein, i.e. a + and - separated sequence of terms
    Polynom expression()
    {
        Polynom sum;
        switch (ein.peek())
        {
            case '+': ein.get(); // fallthrough
            default: sum =  term(); break;
            case '-': ein.get(); sum = -term(); break;
        }

        while (ein.peek() == '+' || ein.peek() == '-')
        switch (ein.peek())
        {
            case '+': ein.get(); sum += term(); break;
            case '-': ein.get(); sum -= term(); break;
        }

        return sum;
    }

    /// term interpretation from ein, i.e. a * spearated sequence of factors
    Polynom term()
    {
        Polynom prod = factor();
        while (ein.peek() == '*')
        {
            ein.get();
            prod *= factor();
        }
        return prod;
    }

    /// factor interpretation from ein, i.e. an unsigned integer
    Polynom factor()
    {
        if (ein.peek() == '(')
        {
            ein.get();
            Polynom result = expression();
            expect(')');
            return result;
        }

        if (isdigit(ein.peek()))
        {
            long result;
            ein >> result;
            return result;
        }

        if (ein.peek() == 'x')
        {
            ein.get();
            return Polynom(1, 1);
        }

        if (ein.eof())
            throw parse_error("unexpected end of expression");
        throw parse_error("unexpected character");
    }
    
    void expect(char c) // not const because of peek()
    {
        string message = "'.' expected";
        message[1] = c; // replace . with c.
        expect(c, message);
    }

    void expect(char c, string message) // not const because of peek()
    {
        if (ein.peek() != c) throw parse_error(message, pos());
        ein.get();
    }

public:
    Expression(string s) : size(s.size()), ein(s) { }

    friend Polynom eval(Expression & expr)
    {
        Polynom result = expr.expression();
        if (!expr.ein.eof())
            throw parse_error("unexpected characters at the end of the expression", expr.pos());
        return result;
    }
    size_t pos() // not const because of peek()
    {
        return ein.eof() ? size : static_cast<size_t>(ein.tellg());
    }
};

int main()
{
    string s;
    cout << "Arithmetical Expression: ";
    cin >> s;
    Expression expr = s;
    try
    {
        Polynom result = eval(expr);
        cout << "Result: " << result << endl;
    }
    catch (runtime_error& error)
    {
        cout << "Error:" << endl;
        cout << s << endl;
        for (int i = 0; i < expr.pos(); ++i) cout << ' ';
        cout << "^-- " << error.what() << endl;
    }
}
