// compile with: c++ -std=c++17 -D_main -D_unittest

#include "unit_test_2.hpp"

unittest
{
    int x = 0;
    ++x;
    assert(x == 1, mention(x));
    ++x;
    // assert(x == 0, mention(x));
}
end_unittest

unittest
{
    int x = 1;
    int y = 0;
    assert(x + y > 0);
    assert(x + y == 1, mention(x), mention(y), mention(x + y));
    assert(x + y == 2, mention(x), mention(y), mention(x + y));
}
end_unittest
