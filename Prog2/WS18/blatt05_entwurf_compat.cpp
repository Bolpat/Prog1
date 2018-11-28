#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <vector>

namespace checked_int
{

template <typename T>
class checked
{
    T value;
  public:
    checked(T value = T()) : value(value) { }
    operator T() const { return value; }

    checked operator+() const { return *this; }
    checked operator-() const
    {
        if (std::numeric_limits<T>::is_signed && std::numeric_limits<T>::min() + std::numeric_limits<T>::max() < T(0))
        if (value == std::numeric_limits<T>::min())
        {
            throw std::overflow_error("operator-: arithmetic overflow on negation");
        }
        return checked(-value);
    }

    template<typename S> checked& operator+=(S rhs) { value = *this + rhs; return *this; }
    template<typename S> checked& operator-=(S rhs) { value = *this - rhs; return *this; }
    template<typename S> checked& operator*=(S rhs) { value = *this * rhs; return *this; }
    template<typename S> checked& operator/=(S rhs) { value = *this / rhs; return *this; }
    template<typename S> checked& operator%=(S rhs) { value = *this % rhs; return *this; }

    template<typename T1, typename T2>
    friend auto operator+(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value + rhs.value)>
    {
        using Common = decltype(lhs.value + rhs.value);
        if (rhs.value > T2(0) && std::numeric_limits<Common>::max() - rhs.value < lhs.value
            || rhs.value < T2(0) && std::numeric_limits<Common>::min() - rhs.value > lhs.value)
        {
            throw std::overflow_error("operator+: arithmetic overflow");
        }
        return checked<Common>(lhs.value + rhs.value);
    }
    
    template<typename T1, typename T2>
    friend auto operator-(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value - rhs.value)>
    {
        using Common = decltype(lhs.value - rhs.value);
        if (rhs.value < T2(0) && std::numeric_limits<Common>::max() + rhs.value < lhs.value
            || rhs.value > T2(0) && std::numeric_limits<Common>::min() + rhs.value > lhs.value)
        {
            throw std::overflow_error("operator-: arithmetic overflow");
        }
        return checked<Common>(lhs.value - rhs.value);
    }
    
    template<typename T1, typename T2>
    friend auto operator*(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value * rhs.value)>
    {
        using Common = decltype(lhs.value * rhs.value);
        if (std::numeric_limits<Common>::is_signed && std::numeric_limits<Common>::min() + std::numeric_limits<Common>::max() < Common(0) && (
            lhs.value == T1(-1) && rhs.value == std::numeric_limits<Common>::min()
            || rhs.value == T2(-1) && lhs.value == std::numeric_limits<Common>::min()))
        {
            throw std::overflow_error("operator*: arithmetic overflow on negation");
        }
        if (rhs < 0 && lhs < 0)
        {
            rhs = -rhs;
            lhs = -lhs;
        }
        // one of rhs and lhs >= 0; for == 0, nothing to check; for < 0, the other must be >= 0, so if > 0, divide max by it.
        if (rhs > 0 && (
            lhs.value > std::numeric_limits<Common>::max() / rhs.value
            || lhs.value < std::numeric_limits<Common>::min() / rhs.value)
            || lhs > 0 && (
                rhs.value > std::numeric_limits<Common>::max() / lhs.value
                || rhs.value < std::numeric_limits<Common>::min() / lhs.value)
        )
        {
            throw std::overflow_error("operator*: arithmetic overflow");
        }
        return checked<Common>(lhs.value * rhs.value);
    }
    
    template<typename T1, typename T2>
    friend auto operator/(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value / rhs.value)>
    {
        if (rhs == T2(0)) throw std::domain_error("operator/: division by zero");
        using Common = decltype(lhs.value / rhs.value);
        if (std::numeric_limits<Common>::is_signed && std::numeric_limits<Common>::min() + std::numeric_limits<Common>::max() < Common(0) &&
            std::numeric_limits<T2>::is_signed && rhs == T2(-1) && lhs.value == std::numeric_limits<Common>::min())
        {
            throw std::overflow_error("operator/: arithmetic overflow on negation");
        }
        return checked<Common>(lhs.value / rhs.value);
    }
    
    template<typename T1, typename T2>
    friend auto operator%(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value % rhs.value)>
    {
        if (rhs == T2(0)) throw std::domain_error("operator%: division by zero");
        using Common = decltype(lhs.value % rhs.value);
        return checked<Common>(lhs.value % rhs.value);
    }

    friend std::ostream& operator<<(std::ostream& stream, checked   c) { return stream << c.value; }
    friend std::istream& operator>>(std::istream& stream, checked & c) { return stream >> c.value; }
};

template<typename T1, typename T2> bool operator==(checked<T1> lhs, checked<T2> rhs) { return static_cast<T1>(lhs) == static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator==(checked<T1> lhs,         T2  rhs) { return static_cast<T1>(lhs) ==                 rhs ; }
template<typename T1, typename T2> bool operator==(        T1  lhs, checked<T2> rhs) { return                 lhs  == static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator!=(checked<T1> lhs, checked<T2> rhs) { return static_cast<T1>(lhs) != static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator!=(checked<T1> lhs,         T2  rhs) { return static_cast<T1>(lhs) !=                 rhs ; }
template<typename T1, typename T2> bool operator!=(        T1  lhs, checked<T2> rhs) { return                  lhs != static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator< (checked<T1> lhs, checked<T2> rhs) { return static_cast<T1>(lhs) <  static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator< (checked<T1> lhs,         T2  rhs) { return static_cast<T1>(lhs) <                  rhs ; }
template<typename T1, typename T2> bool operator< (        T1  lhs, checked<T2> rhs) { return                 lhs  <  static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator> (checked<T1> lhs, checked<T2> rhs) { return static_cast<T1>(lhs) >  static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator> (checked<T1> lhs,         T2  rhs) { return static_cast<T1>(lhs) >                  rhs ; }
template<typename T1, typename T2> bool operator> (        T1  lhs, checked<T2> rhs) { return                  lhs >  static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator<=(checked<T1> lhs, checked<T2> rhs) { return static_cast<T1>(lhs) <= static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator<=(checked<T1> lhs,         T2  rhs) { return static_cast<T1>(lhs) <=                 rhs ; }
template<typename T1, typename T2> bool operator<=(        T1  lhs, checked<T2> rhs) { return                 lhs  <= static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator>=(checked<T1> lhs, checked<T2> rhs) { return static_cast<T1>(lhs) >= static_cast<T2>(rhs); }
template<typename T1, typename T2> bool operator>=(checked<T1> lhs,         T2  rhs) { return static_cast<T1>(lhs) >=                 rhs ; }
template<typename T1, typename T2> bool operator>=(        T1  lhs, checked<T2> rhs) { return                  lhs >= static_cast<T2>(rhs); }

template<typename T1, typename T2> auto operator+(        T1  lhs, checked<T2> rhs) { return checked<T1>(lhs) +             rhs ; }
template<typename T1, typename T2> auto operator+(checked<T1> lhs,         T2  rhs) { return             lhs  + checked<T2>(rhs); }
template<typename T1, typename T2> auto operator-(        T1  lhs, checked<T2> rhs) { return checked<T1>(lhs) -             rhs ; }
template<typename T1, typename T2> auto operator-(checked<T1> lhs,         T2  rhs) { return             lhs  - checked<T2>(rhs); }
template<typename T1, typename T2> auto operator*(        T1  lhs, checked<T2> rhs) { return checked<T1>(lhs) *             rhs ; }
template<typename T1, typename T2> auto operator*(checked<T1> lhs,         T2  rhs) { return             lhs  * checked<T2>(rhs); }
template<typename T1, typename T2> auto operator/(        T1  lhs, checked<T2> rhs) { return checked<T1>(lhs) /             rhs ; }
template<typename T1, typename T2> auto operator/(checked<T1> lhs,         T2  rhs) { return             lhs  / checked<T2>(rhs); }
template<typename T1, typename T2> auto operator%(        T1  lhs, checked<T2> rhs) { return checked<T1>(lhs) %             rhs ; }
template<typename T1, typename T2> auto operator%(checked<T1> lhs,         T2  rhs) { return             lhs  % checked<T2>(rhs); }

template<typename T>
checked<T> check(T value) { return checked<T>(value); }

}

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

struct todo_missing : public std::logic_error { todo_missing(const char * p) : std::logic_error(p) { } };

#define TODO() throw todo_missing("something not implemented in line " STRINGIZE(__LINE__) ".")


// ***** BEGIN HERE *****

using namespace std;
using namespace checked_int;

class rational
{
  private:
    checked<long> p, q;

    // greatest common divisor
    static long gcd(long a, long b)
    {
        if (a < 0) TODO();
        if (b < 0) TODO();
        if (b == 0) TODO();
        TODO(); // the main case
    }

  public:
    rational(long z = 0, long n = 1)
        : p(z), q(n)
    {
        if (q == 0) throw domain_error("denominator must not be zero");
        if (q < 0)
        {
            TODO();
        }
        TODO();
    }

    explicit operator long double() const { TODO(); }

    friend rational operator+(rational r) { TODO(); }
    friend rational operator-(rational r) { TODO(); }

    rational& operator+=(rational r) { return *this = *this + r; }
    rational& operator-=(rational r) { return *this = *this - r; }
    rational& operator*=(rational r) { return *this = *this * r; }
    rational& operator/=(rational r) { return *this = *this / r; }

    friend rational operator+(rational s, rational t)
    {
        TODO();
    }
    friend rational operator-(rational s, rational t)
    {
        TODO();
    }
    friend rational operator*(rational s, rational t)
    {
        TODO();
    }
    friend rational operator/(rational s, rational t)
    {
        if (t == 0) throw invalid_argument("operator/: division by zero");
        TODO();
    }

    friend rational pow(rational b, int n)
    {
        if (n == 0) return 1;
        if (b == 0) return b;
        if (n < 0)
        {
            TODO();
        }
        TODO();
    }

    friend bool operator==(rational s, rational t) { return s.p == t.p && s.q == t.q; }
    friend bool operator!=(rational s, rational t) { return !(s == t); }
    friend bool operator> (rational s, rational t) { return t < s; }
    friend bool operator<=(rational s, rational t) { return s == t || s < t; }
    friend bool operator>=(rational s, rational t) { return s == t || s > t; }
    friend bool operator< (rational s, rational t)
    {
        // trivial criteria
        if (s.p <= 0 && 0 <= t.p) return s != t; // makes comparisons to 0 fast.
        if (s.p <= t.p && s.q >= t.q) return s != t;

        TODO();
    }

    friend ostream& operator<<(ostream& stream, rational const & r)
    {
        TODO();
    }
    friend istream& operator>>(istream& stream, rational & r)
    {
        TODO();
    }
};

rational power_series(rational const & r, int n)
{
    TODO();
}

rational continued_fraction(vector<long> const & a, vector<long> const & b)
{
    if (b.size() == 0u) throw invalid_argument("continued_fraction: vector of coefficients must not be empty");
    if (a.size() != b.size() - 1u) throw invalid_argument("continued_fraction: vector sizes mismatch");

    TODO();
}

// version where the a[i] default to ones.
rational continued_fraction(vector<long> const & b)
{
    if (b.size() == 0u) throw invalid_argument("continued_fraction: vector of coefficients must not be empty");
    return continued_fraction(vector<long>(b.size() - 1, 1), b);
}

// for debugging vectors of int; prints [v0, v1, .., vn]
// comment in if you need it.
/*
ostream& operator<<(ostream& stream, vector<int> const & v)
{
    stream << '[';
    if (!v.empty())
    {
        stream << v[0];
        for (vector<int>::size_type i = 1; i < v.size(); ++i)
            stream << ", " << v[i];
    }
    stream << ']';
    return stream;
}
*/


int main()
{
    try
    {
        cout << "\n-- (a) and (b) ----------" << endl;
        for (rational r : { rational(2, 3), rational(-10, 7) })
        {
            const int n = 8;
            // TODO
        }

        cout << "\n-- (c) ------------------" << endl;
        // TODO

        cout << "\n-- (d) ------------------" << endl;
        // TODO
    }
    catch (todo_missing const & e)
    {
        cerr << e.what() << endl;
    }
}
