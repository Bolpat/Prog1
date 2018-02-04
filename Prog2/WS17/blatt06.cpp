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
        long a;
        unsigned d;

        constexpr Monom(long a = 0, unsigned d = 0)
        : a(a), d(d)
        { }

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
// #ifdef _unicode_exponents
            // static const wchar_t powers[] = L"\x2070\x00B9\x00B2\x00B3\x2074\x2075\x2076\x2077\x2078\x2079";
            // if (m.d > 1) stream << powers[m.d];
// #else
            if (m.d > 1) stream << '^' << m.d;
// #endif
            return stream;
        }
    };

    list<Monom> monoms;
    // Invariants:
    //  is sorted by .d
    //  does not contain two elements with same .d
    //  does not contain monom with .a == 0

public:
    Polynom(long a = 0, unsigned d = 0)
    : monoms(a == 0 ? 0 : 1, Monom(a, d))
    { }

    Polynom(Monom const & monom)
    : monoms(monom.a == 0 ? 0 : 1, monom)
    { }

    friend Polynom operator-(Polynom p) // copy p
    {
        for (auto & monom : p.monoms) monom = -monom;
        return p;
    }

    // iterates the list once
    friend Polynom operator+(Polynom p, Polynom q) // copy p, q
    {
        p.monoms.merge(q.monoms, [](Monom m, Monom n) { return m.d < n.d; });

        if (p.monoms.size() < 2) return p;

        auto it = p.monoms.begin();
        do
        {
            auto prev = it++;
            if (prev->d == it->d)
            {
                if ((it->a += prev->a) != 0) ++it;
                else it = p.monoms.erase(it);
                p.monoms.erase(prev);
            }
        }
        while (it != p.monoms.end());

        return p;
    }

    // iterates the list twice
    /*
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
            it2->a = 0; // mark second for reasure
        }

        p.monoms.remove_if([](Monom m) { return m.a == 0; });

        return p;
    }
    */

    friend Polynom operator-(const Polynom & p, const Polynom & q)
    {
        return p + (-q);
    }

    friend Polynom operator*(Polynom const & p, Polynom const & q)
    {
        Polynom result;
        for (auto const & m : p.monoms)
        for (auto const & n : q.monoms)
            result += m * n; // optimized
        return result;
    }

    /// binary exponentiation
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

        // if m is the first in the merged list, no equivalent monoms exist, and we're done.
        if (it-- == monoms.begin()) return *this;
        // if the predecessor of m is equivalent to it ...
        if (it->d == m.d)
        {
            // ... add the elements together, and if that sum equals 0 ...
            if ((it->a += m.a) == 0)
                it = monoms.erase(it); // ... remove it from the list, what increments the iterator to the next element ...
            else
                ++it; // ... otherwise increment iterator to the next element manually ...
            monoms.erase(it); // ... and erase it.
        }
        return *this;
    }

    Polynom & operator+=(Polynom p) { return *this = *this + p; }
    Polynom & operator-=(Polynom p) { return *this = *this - p; }
    Polynom & operator*=(Polynom p) { return *this = *this * p; }

    friend ostream & operator<<(ostream & stream, Polynom const & p)
    {
        if (p.monoms.size() == 0) return stream << '0';

        auto it = p.monoms.crbegin();
        stream << *it;
        for (++it; it != p.monoms.crend(); ++it)
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

    /// extracts character if it is one of the mentioned
    /// returns ein.peek() in any case.
#if __cplusplus > 201402L
    template <typename ... Ts>
    char maybe(Ts... should_be)
    {
        char result = ein.peek();
        // C++17 Fold Expression
        if ( (... || (result == should_be)) ) ein.get();
        return result;
    }
#else
    char maybe(char should_be)
    {
        char result = ein.peek();
        if (result == should_be) ein.get();
        return result;
    }
    char maybe(char should_be1, char should_be2)
    {
        char result = ein.peek();
        if (result == should_be1 || result == should_be2) ein.get();
        return result;
    }
#endif
    /// if ein.peek() != expected_char, throw exception with appropriate message
    void expect(char expected_char) // not const because of peek()
    {
        string message = "'_' expected";
        message[1] = expected_char; // replace _ with c.
        expect(expected_char, message);
    }

    /// if ein.peek() != expected_char, throw exception with given message
    void expect(char expected_char, string message) // not const because of peek()
    {
        if (ein.peek() != expected_char) throw parse_error(message, pos());
        ein.get();
    }

    /// expression interpretation from ein, i.e. a + and - separated sequence of terms
    Polynom expression()
    {
        Polynom sum;
        switch (maybe('+', '-'))
        {
            case '+': // fallthrough
            default: sum = term(); break;
            case '-': sum = -term();
        }

        for (;;)
        switch (maybe('+', '-'))
        {
            case '+': sum += term(); break;
            case '-': sum -= term(); break;
            default: return sum;
        }
    }

    /// term interpretation from ein, i.e. a * spearated sequence of factors
    Polynom term()
    {
        Polynom prod = factor();
        for (;;)
        switch (maybe('*'))
        {
            case '*': prod *= factor(); break;
            default : return prod;
        }
    }

    /// factor interpretation from ein, i.e. an operand optionally followed by '^' and non-negative integer
    Polynom factor()
    {
        Polynom result = operand();
        if (maybe('^') != '^') return result;
        unsigned n;
        ein >> n;
        return pow(result, n);
    }

    /// operand interpretation from ein, i.e. an unsigned integer
    Polynom operand()
    {
        if (maybe('(') == '(')
        {
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

        if (maybe('x') == 'x') return Polynom(1, 1);

        throw parse_error(ein.eof()
            ? "unexpected end of expression"
            : "unexpected character");
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
    catch (parse_error& error)
    {
        cout << "Error:" << endl;
        cout << s << endl;
        for (unsigned i = 0; i < expr.pos(); ++i) cout << ' ';
        cout << "^-- " << error.what() << endl;
    }
}
