
// this lets us use __LINE__ in error messages properly
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// this lets us create unique names of all sorts
#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define UNIQUE_NAME(base) PP_CAT(base, __COUNTER__)

// if -D_main is used while compiling,
// an empty main will be inserted.
// Note it will always return non-zero.
#ifdef _main
    int main() { return 1; }
#endif

// if -D_inspect is used while compiling,
// inspect(x) can be used anywhere to
// print an expression with its value.
#ifdef _inspect
    #include <iostream>
    
    #define inspect(x) std::cerr << #x << ": " << (x) << std::endl;
#else
    #define inspect(x)
#endif

// if -D_unittest is used while compiling,
// unit tests will be executed.
#ifdef _unittest
    #define _assert
    #define unittest \
        run_unittest UNIQUE_NAME(unittest_){[]
    #define end_unittest };
#endif

#ifdef _assert
#include <iostream>

// mention creates an object of named_expr in the intended way:
// the name part holds a string resembling the expression,
// the value part holds the expression.
#define mention(x) named_expr{ #x, (x) }

// A named_expr holds a named value.
template <typename T>
struct named_expr
{
    const char *name;
    T value;
    
    named_expr(const char *name, T &value) : name(name), value(value) { }
    named_expr(const char *name, T &&value) : name(name), value(std::move(value)) { }
    
    friend std::ostream &operator<<(std::ostream &stream, const named_expr &self)
    {
        return stream << self.name << " = " << self.value;
    }

    friend std::ostream &&operator<<(std::ostream &&stream, const named_expr &self)
    {
        return std::move(stream << self);
    }
};

// assert errors will usually hold a list of mentioned expressions to help
// understanding what went wrong and why the assert condition is false.
// assert_error is a template which inherits from this.
struct assert_error_base
{
    const char *msg;
    assert_error_base(const char *msg) : msg(msg) { }
    virtual void print(std::ostream &stream) const = 0;
};

#include <tuple>

// inherits the msg from assert_error_base and amends it with a custom list of named expressions.
// 
template <typename... Ts>
struct assert_error : public assert_error_base
{
    std::tuple<named_expr<Ts>...> named_expr_tuple;
    
    explicit assert_error(const char *msg, const named_expr<Ts> &... names)
    : assert_error_base(msg), named_expr_tuple(names...)
    { }

    void print(std::ostream &stream) const
    {
        stream << msg;
        if constexpr (sizeof...(Ts) > 0)
        {
            stream << "; relevant binding" << (sizeof...(Ts) == 1 ? "" : "s") << ": ";
            std::apply([&stream](auto first_named_expr, auto ...named_exprs)
            {
                stream << first_named_expr;
#if __cpp_fold_expressions >= 201603L
                ((stream << ", " << named_exprs), ...);
#else
                std::tuple{ (stream << ", " << named_exprs)... };
#endif
            }, named_expr_tuple);
        }
        stream << std::endl;
    }
};

// basically executes a unit test.
template <typename Functional>
struct run_unittest
{
    run_unittest(const Functional &functional)
    {
        try
        {
            functional();
        }
        catch (const assert_error_base &err)
        {
            err.print(std::cerr);
        }
        catch (const std::exception &exc)
        {
            std::cerr << "Unittest failure: an exception was thrown." << '\n';
            std::cerr << "Message: " << exc.what() << '\n';
        }
        catch (const char *msg)
        {
            std::cerr << "Unittest failure. Message: " << msg << '\n';
        }
        catch (const std::string &msg)
        {
            std::cerr << "Unittest failure. Message: " << msg << '\n';
        }
        catch (...)
        {
            std::cerr << "Unittest failure.\n";
        }
    }
};

// Usage:
//  assert(condition);
//  assert(condition, mention(variable), mention(expression));
#define assert(cond, ...) \
    if (!(cond)) do       \
    throw assert_error {  \
        "Unittest failure (" __FILE__ ": " TOSTRING(__LINE__) "): (" #cond ") is false", \
        __VA_ARGS__       \
        };                \
    while (false)

#endif