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
    constexpr checked(T value = T()) : value(value) { }
    constexpr operator T() const { return value; }

    checked operator-() const
    {
        if (std::numeric_limits<T>::is_signed && std::numeric_limits<T>::min() + std::numeric_limits<T>::max() < T(0))
        if (value == std::numeric_limits<T>::min())
        {
            std::overflow_error("operator-: arithemtic overflow on negation");
        }
        return checked(-value);
    }

    template<typename S> checked& operator+=(S rhs) { value = *this + rhs; return *this; }
    template<typename S> checked& operator-=(S rhs) { value = *this - rhs; return *this; }
    template<typename S> checked& operator*=(S rhs) { value = *this * rhs; return *this; }
    template<typename S> checked& operator/=(S rhs) { value = *this / rhs; return *this; }
    template<typename S> checked& operator%=(S rhs) { value = *this % rhs; return *this; }

    template<typename T1, typename T2>
    friend auto operator+(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value + rhs.value)>;
    template<typename T1, typename T2>
    friend auto operator-(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value - rhs.value)>;
    template<typename T1, typename T2>
    friend auto operator*(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value * rhs.value)>;
    template<typename T1, typename T2>
    friend auto operator/(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value / rhs.value)>;
    template<typename T1, typename T2>
    friend auto operator%(checked<T1> lhs, checked<T2> rhs) -> checked<decltype(lhs.value % rhs.value)>;

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

template<typename T1, typename T2>
auto operator+(checked<T1> lhs, checked<T2> rhs)
{
    using Common = decltype(lhs.value + rhs.value);
    if (rhs.value > T1(0) && std::numeric_limits<Common>::max() - rhs.value < lhs.value
     || rhs.value < T1(0) && std::numeric_limits<Common>::min() - rhs.value > lhs.value)
    {
        throw std::overflow_error("operator+: arithemtic overflow");
    }
    return checked<Common>(lhs.value + rhs.value);
}

template<typename T1, typename T2>
auto operator-(checked<T1> lhs, checked<T2> rhs)
{
    using Common = decltype(lhs.value - rhs.value);
    if (rhs.value < T1(0) && std::numeric_limits<Common>::max() + rhs.value < lhs.value
     || rhs.value > T1(0) && std::numeric_limits<Common>::min() + rhs.value > lhs.value)
    {
        throw std::overflow_error("operator-: arithemtic overflow");
    }
    return checked<Common>(lhs.value - rhs.value);
}

template<typename T1, typename T2>
auto operator*(checked<T1> lhs, checked<T2> rhs)
{
    using Common = decltype(lhs.value * rhs.value);
    if (std::numeric_limits<Common>::is_signed && std::numeric_limits<Common>::min() + std::numeric_limits<Common>::max() < Common(0) && (
            lhs.value == T1(-1) && rhs.value == std::numeric_limits<Common>::min()
         || rhs.value == T2(-1) && lhs.value == std::numeric_limits<Common>::min()))
    {
        throw std::overflow_error("operator*: arithemtic overflow on negation");
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
        throw std::overflow_error("operator*: arithemtic overflow");
    }
    return checked<Common>(lhs.value * rhs.value);
}

template<typename T1, typename T2>
auto operator/(checked<T1> lhs, checked<T2> rhs)
{
    if (rhs == T2(0)) throw std::domain_error("operator/: division by zero");
    using Common = decltype(lhs.value / rhs.value);
    if (std::numeric_limits<Common>::is_signed && std::numeric_limits<Common>::min() + std::numeric_limits<Common>::max() < Common(0) &&
        std::numeric_limits<T2>::is_signed && rhs == T2(-1) && lhs.value == std::numeric_limits<Common>::min())
    {
        throw std::overflow_error("operator/: arithemtic overflow on negation");
    }
    return checked<Common>(lhs.value / rhs.value);
}

template<typename T1, typename T2>
auto operator%(checked<T1> lhs, checked<T2> rhs)
{
    if (rhs == T2(0)) throw std::domain_error("operator%: division by zero");
    using Common = decltype(lhs.value % rhs.value);
    return checked<Common>(lhs.value % rhs.value);
}

}

using namespace std;
using namespace checked_int;

class rational
{
  private:
    checked<long> p, q;
    // T p, q;

    static long ggt(long a, long b)
    {
        throw logic_error("not implemented");
    }

  public:
    rational(long z = 0, long n = 1)
        : p(z), q(n)
    {
        if (q == 0) throw domain_error("denominator must not be zero");
        if (q < 0)
        {
            throw logic_error("not implemented");
        }
        throw logic_error("not implemented");
    }

    explicit operator long double() const { throw logic_error("not implemented"); }

    friend rational operator+(rational r) { throw logic_error("not implemented"); }
    friend rational operator-(rational r) { throw logic_error("not implemented"); }

    rational& operator+=(rational r) { throw logic_error("not implemented"); }
    rational& operator-=(rational r) { throw logic_error("not implemented"); }
    rational& operator*=(rational r) { throw logic_error("not implemented"); }
    rational& operator/=(rational r) { throw logic_error("not implemented"); }
    rational& operator%=(rational r) { throw logic_error("not implemented"); }

    friend rational operator+(rational s, rational t)
    {
        throw logic_error("not implemented");
    }
    friend rational operator-(rational s, rational t)
    {
        throw logic_error("not implemented");
    }
    friend rational operator*(rational s, rational t)
    {
        throw logic_error("not implemented");
    }
    friend rational operator/(rational s, rational t)
    {
        throw logic_error("not implemented");
    }

    friend rational operator%(rational s, rational t)
    {
        throw logic_error("not implemented");
    }

    friend rational pow(rational b, int n)
    {
        throw logic_error("not implemented");
    }

    friend bool operator==(rational s, rational t) { throw logic_error("not implemented"); }
    friend bool operator!=(rational s, rational t) { throw logic_error("not implemented"); }
    friend bool operator> (rational s, rational t) { throw logic_error("not implemented"); }
    friend bool operator<=(rational s, rational t) { throw logic_error("not implemented"); }
    friend bool operator>=(rational s, rational t) { throw logic_error("not implemented"); }
    friend bool operator< (rational s, rational t)
    {
        // trivial criteria
        if (s.q == t.q) return s.p < t.p;
        if (s.p == t.q) return s.q > t.q;
        if (s.p <= t.p && s.q <= t.q) return s != t;

        throw logic_error("not implemented");
    }

    friend ostream& operator<<(ostream& stream, rational const & r)
    {
        throw logic_error("not implemented");
    }
    friend istream& operator>>(istream& stream, rational& r)
    {
        throw logic_error("not implemented");
    }
};

rational power_series(rational const & r, int n)
{
    throw logic_error("not implemented");
}

rational continued_fraction(vector<long> const & a, vector<long> const & b)
{
    if (b.size() == 0u) throw invalid_argument("continued_fraction: vector of coefficients must not be empty");
    if (a.size() != b.size() - 1u) throw invalid_argument("continued_fraction: vector sizes mismatch");

    throw logic_error("not implemented");
}

rational continued_fraction(vector<long> const & b)
{
    if (b.size() == 0u) throw invalid_argument("continued_fraction: vector of coefficients must not be empty");
    throw logic_error("not implemented");
}

int main()
{
    cout << "-- (a) and (b) ----------" << endl;
    for (rational r : { rational(2,3), rational(-10,7) })
    {
        const int n = 8;
        throw logic_error("not implemented");
    }

    cout << "-- (c) ------------------" << endl;
    throw logic_error("not implemented");
    
    cout << "-- (d) ------------------" << endl;
    throw logic_error("not implemented");
}
