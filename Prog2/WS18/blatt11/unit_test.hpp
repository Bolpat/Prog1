
// this lets us use __LINE__ in error messages properly
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// this lets us create unique names of all sorts
#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define UNIQUE_NAME(base) PP_CAT(base, __COUNTER__)

#ifdef _inspect
    #include <iostream>
    
    #define inspect(x) std::cerr << #x << ": " << (x) << std::endl;
#endif

// if -D_unittest is used while compiling,
// unit tests will be executed.
#ifdef _unittest
    #define unittest(stmts) \
        run_unittest UNIQUE_NAME(unit_test_)([]{ stmts });
    #define unittest_n(stmts, ...) \
        run_unittest UNIQUE_NAME(unit_test_)([]{ stmts }, __VA_ARGS__);
#else
    #define unittest(stmts)
#endif

#define assert(cond)                                                      \
    do                                                                    \
    {                                                                     \
        if (!cond)                                                        \
            throw assert_error{"Unittest failure (" __FILE__ ": " TOSTRING(__LINE__) "): (" #cond ") is false"}; \
    }                                                                     \
    while (false)

struct assert_error { const char *msg; };

// if -D_main is used while compiling,
// an empty main will be inserted.
// Note it will always return non-zero.
#ifdef _main
    int main() { return 1; }
#endif

#include <iostream>

template <typename T>
struct name_t
{
    const char* name;
    T value;
    
    friend std::ostream &operator<<(std::ostream &stream, const name_t &self)
    {
        return stream << self.name << " = " << self.value;
    }

    friend std::ostream &&operator<<(std::ostream &&stream, const name_t &self)
    {
        return std::move(stream << self);
    }
};

#define name(x) name_t<decltype(x)>{ #x, (x) }

struct run_unittest
{
    template <typename Functional, typename... Ts>
    run_unittest(Functional functional, const name_t<Ts> &&... names)
    try
    {
        functional();
    }
    catch (assert_error err)
    {
        std::cerr << err.msg << "\nValues included are: ";
#if __cpp
        (std::cerr << ... << names);
#else
        print_names(names...);
#endif 
        std::cerr << std::endl;
    }
    
#if !(__cpp_fold_expressions >= 201603L)
    void print_names(std::ostream &stream) { }
    
    template <typename T, typename... Ts>
    void print_names(std::ostream &stream, const name_t<T> &this_name, const name_t<Ts> &... names)
    {
        stream << this_name;
        print_names(stream, names...);
    }
#endif
};
