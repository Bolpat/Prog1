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

    template<typename T1, typename T2> friend auto operator+(checked<T1> lhs, checked<T2> rhs);
    template<typename T1, typename T2> friend auto operator-(checked<T1> lhs, checked<T2> rhs);
    template<typename T1, typename T2> friend auto operator*(checked<T1> lhs, checked<T2> rhs);
    template<typename T1, typename T2> friend auto operator/(checked<T1> lhs, checked<T2> rhs);
    template<typename T1, typename T2> friend auto operator%(checked<T1> lhs, checked<T2> rhs);

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
    if (rhs.value > T2(0) && std::numeric_limits<Common>::max() - rhs.value < lhs.value
     || rhs.value < T2(0) && std::numeric_limits<Common>::min() - rhs.value > lhs.value)
    {
        throw std::overflow_error("operator+: arithmetic overflow");
    }
    return checked<Common>(lhs.value + rhs.value);
}

template<typename T1, typename T2>
auto operator-(checked<T1> lhs, checked<T2> rhs)
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
auto operator*(checked<T1> lhs, checked<T2> rhs)
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
auto operator/(checked<T1> lhs, checked<T2> rhs)
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
auto operator%(checked<T1> lhs, checked<T2> rhs)
{
    if (rhs == T2(0)) throw std::domain_error("operator%: division by zero");
    using Common = decltype(lhs.value % rhs.value);
    return checked<Common>(lhs.value % rhs.value);
}

template<typename T>
checked<T> check(T value) { return checked<T>(value); }

}

/*
int main()
{
    // to verify the calls compile.
    checked_int::checked<long> a, b;
    auto p = a + b;
    auto s = a - b;
    auto t = a * b;
    auto d = a * b;
    auto m = a % b;
}
*/


using namespace std;
using namespace checked_int;

// for debugging.
#define READ(x) do { std::cerr << #x << ": "; std::cin >> (x); } while (0)
#define INSPECT(x) do { std::cerr << #x << " = " << (x) << std::endl; } while (0)
#define EXPECT(x, r) do { std::cerr << "is " << #x << " = " << (x) << " equal to " << (r) << " = " << #r << "?" << std::endl; } while (0)

#define INSPECT_RAT(x) do { std::cerr << #x << " = "; _inspect_rat(x); } while (0)
template<typename T> void _inspect_rat(rational<T> r) { std::cerr << r << " = " << static_cast<long double>(r) << std::endl; }

template <typename T>
class rational
{
  private:
    checked<T> p, q;
    // T p, q;

    static T gcd(T a, T b)
    {
        if (a < 0) return gcd(-check(a), b);
        if (b < 0) return gcd(a, -check(b));
        if (b == 0) return a;
        return gcd(b, a % b);
    }

  public:
    rational(T z = 0, T n = 1)
        : p(z), q(n)
    {
        if (q == 0) throw domain_error("denominator must not be zero");
        if (q < 0)
        {
            p = -check(p);
            q = -check(q);
        }
        T g = gcd(p, q);
        p /= g;
        q /= g;
    }

    explicit operator long double() const { return static_cast<long double>(p) / static_cast<long double>(q); }

    friend rational operator+(rational const & r) { return r; }
    friend rational operator-(rational r) { r.p = -r.p; return r; }

    rational& operator+=(rational const & r) { return *this = *this + r; }
    rational& operator-=(rational const & r) { return *this = *this - r; }
    rational& operator*=(rational const & r) { return *this = *this * r; }
    rational& operator/=(rational const & r) { return *this = *this / r; }
    rational& operator%=(rational const & r) { return *this = *this % r; }

    friend rational operator+(rational const & s, rational const & t)
    {
        T g = gcd(s.q, t.q);
        T k = s.q / g * t.q;
        T p = s.p * (t.q / g) + t.p * (s.q / g);
        return rational(p, k);
    }
    friend rational operator-(rational const & s, rational const & t)
    {
        T g = gcd(s.q, t.q);
        T k = (s.q / g) * t.q;
        T p = s.p * (t.q / g) - t.p * (s.q / g);
        return rational(p, k);
    }
    friend rational operator*(rational const & s, rational const & t)
    {
        // strategy: swap denominators to reduce the fractions:
        //  (26/35) * (15/22) = (26/22) * (15/35) = (13/11) * (3/7)
        // the two resulting fractions not only have co-prime numerators and denominators,
        // the numerator of one is co-prime to the denominator of the other,
        //  (13/11) * (3/7) = (13*3)/(11*7)
        // therefore the numerator- and denominator-wise multiplication is optimal.
        rational a(s.p, t.q), b(t.p, s.q), result;
        result.p = a.p * b.p;
        result.q = a.q * b.q;
        return result;
    }
    friend rational operator/(rational const & s, rational const & t)
    {
        if (t == 0) throw invalid_argument("operator/: division by zero");
        return s * rational(t.q, t.p);
    }

    // note: integer division is intentional!
    friend T whole(rational const & r) { return r.p / r.q; }
    friend T div(rational const & s, rational const & t) { return whole(s / t); }
    friend rational operator%(rational const & s, rational const & t) { return s - div(s, t) * t; }

    /*
    // recursive version
    friend rational pow(rational b, int n)
    {
        if (n == 0) return 1;
        if (b == 0) return b;
        if (n < 0) return pow(1 / b, -checked<int>(n));
        if (n % 2 != 0)
        {
            return pow(b * b, n / 2) * b;
        }
        else
        {
            return pow(b * b, n / 2);
        }
    }
    */

    friend rational pow(rational b, int n)
    {
        if (n == 0) return 1;
        if (b == 0) return b;
        if (n < 0)
        {
            b = 1 / b;
            n = -checked<int>(n);
        }
        unsigned h = ~(~0u >> 1);
        while (!(n & h)) h >>= 1;
        rational result = b;
        while (h >>= 1)
        {
            result *= result;
            if (n & h) result *= b;
        }
        return result;
    }

    friend bool operator==(rational const & s, rational const & t) { return s.p == t.p && s.q == t.q; }
    friend bool operator!=(rational const & s, rational const & t) { return !(s == t); }
    friend bool operator> (rational const & s, rational const & t) { return t < s; }
    friend bool operator<=(rational const & s, rational const & t) { return s == t || s < t; }
    friend bool operator>=(rational const & s, rational const & t) { return s == t || s > t; }
    friend bool operator< (rational const & s, rational const & t)
    {
        // trivial criteria
        if (s.p <= 0 && 0 <= t.p) return s != t; // makes comparisons to 0 fast.
        if (s.p <= t.p && s.q >= t.q) return s != t;

        // We'll just use the fact that s.p/s.q < t.p/t.q iff s.p * t.q < t.p * s.q
        // as denominators are always positive.
        // For optimization, we use the fact that for (s.p/s.q) < (t.p/t.q), we can
        // divide both sides by (gcd(s.p, t.p)/gcd(s.q, t.q)) easily:
        //      15/22 < 35/26  using / (5/2)
        //  <=>  5/11 <  7/13  using cross multiply, i.e. * (11*13)
        //  <=>  5*13 <  7*11
        //  <=>  65   <  77
        auto gp = gcd(s.p, t.p), gq = gcd(t.q, s.q);
        return (s.p / gp) * (t.q / gq) < (t.p / gp) * (s.q / gq);
    }

    friend ostream& operator<<(ostream& stream, rational const & r)
    {
        if (r.q == 1) return stream << r.p;
        return stream << reinterpret_cast<ostringstream&>(ostringstream() << r.p << '/' << r.q).str();
    }
    friend istream& operator>>(istream& stream, rational & r)
    {
        T p;
        if (!(stream >> p)) return stream;
        if (stream.peek() != '/')
        {
            r = rational(p);
            return stream;
        }
        stream.get();
        T q;
        stream >> q;
        r = rational(p, q);
        return stream;
    }
};

template<typename T>
rational<T> power_series(rational<T> const & r, int n)
{
    rational<T> result = 0;
    if (n < 0) return result;
    // using non-inclusive < n (instead of <= n), results are correct even for for n == INT_MAX.
    for (int k = 1; k < n; ++k) result += k * pow(r, k);
    return result + (n * pow(r, n));
}

template<typename T>
rational<T> continued_fraction(vector<T> const & a, vector<T> const & b)
{
    if (b.size() == 0u) throw invalid_argument("continued_fraction: vector of coefficients must not be empty");
    if (a.size() != b.size() - 1u) throw invalid_argument("continued_fraction: vector sizes mismatch");

    rational<T> result = b.back();
    for
    (
        auto a_it = a.crbegin(), b_it = b.crbegin() + 1;
        a_it != a.crend() && b_it != b.crend();
        ++a_it, ++b_it
    ) {
        result = *b_it + *a_it / result;
    }
    return result;
}

template<typename T>
rational<T> continued_fraction(vector<T> const & b)
{
    if (b.size() == 0u) throw invalid_argument("continued_fraction: vector of coefficients must not be empty");
    return continued_fraction(vector<T>(b.size() - 1, 1), b);
}

// testing arithmetic
/*
int main()
{
    try
    {
        rational<long> s, t;
        READ(s);
        // if (s == 0) return 0;
        // READ(t);
        // INSPECT(s);
        // INSPECT(t);
        // INSPECT(s + t);
        // EXPECT(s + t - t, s);
        // EXPECT(s + t - s, t);
        // INSPECT(s - t);
        // EXPECT(s - t + t, s);
        // EXPECT(-(s - t) + s, t);
        // INSPECT(s * t);
        // if (t != 0)
        // {
        //     EXPECT(s * t / t, s);
        // }
        // EXPECT(s * t / s, t);
        // if (t != 0)
        // {
        //     INSPECT(s / t);
        //     EXPECT(s / t * t, s);
        //     EXPECT(1/(s / t) * s, t);
        // }
        // INSPECT(pow(s, 5));
        // EXPECT(pow(s, 5) / pow(s, 4), s);

        using rat = rational<long>;
        // rat s(3, 4), t(1, 2);
        // INSPECT_RAT(s % t);
        for (rat a : { rat(+10, 3), rat(-10, 3) })
        for (rat b : { rat(+3, 2), rat(-3, 2) })
        {
            INSPECT(a);
            INSPECT(b);
            INSPECT(div(a, b));
            INSPECT(a % b);
            EXPECT(a - (div(a, b) * b + (a % b)), 0);
        }
    }
    catch (overflow_error e)
    {
        cout << e.what() << endl;
    }
    return 0;
}
*/

// for debugging vectors
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

long double diff(long double a, long double b) { return a < b ? b - a : a - b; }

int main()
{
    using T = int_least64_t;
    using rat = rational<T>;
    cout << "\n-- (a) and (b) ----------" << endl;
    for (rat r : { rat(2, 3), rat(-10, 7) })
    {
        const int n = 8;
        rat ps = power_series(r, n);
        cout << "sum[k = 1 .. " << n << "] k*(" << r << ")^k = " << ps << " = " << static_cast<long double>(ps) << endl;

        INSPECT_RAT((n * pow(r, n + 2) - (n + 1) * pow(r, n + 1) + r) / pow(r - 1, 2));
    }

    cout << "\n-- (c) ------------------" << endl;
    using vec = vector<T>;
    INSPECT_RAT(continued_fraction(vec{ 3, 7, 15, 1, 292 }));

    cout << "\n-- (d) ------------------" << endl;
    const long double pi = 3.141592653589793238462643383279L;
    for (int n = 1; n < 25; ++n)
    try
    {
        vector<T> a(n, 4);
        vector<T> b(n + 1, 0);
        for (int i = 1; i < n; ++i)
        {
            a[i] = i * i;
            b[i] = 2 * i - 1;
        }
        b[n] = 2 * n - 1;

        auto r = continued_fraction(a, b);
        long double lr = static_cast<long double>(r);
        cout << "for n = " << setw(2) << n << ": ";
        cout << "continued_fraction(a, b) = " << setw(19) << fixed << setprecision(numeric_limits<long double>::digits10) << lr << " (diff = " << diff(lr, pi) << ")";
        cout << " = " << r << endl;
    }
    catch (overflow_error)
    {
        cout << "for n = " << setw(2) << n << ": *** overflow ***" << endl;
    }
}
