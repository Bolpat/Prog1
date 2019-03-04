#include <sstream>

#include "unit_test_2.hpp"
#include "bit_buffer.hpp"

template <std::size_t bits>
void unit_test_template(std::size_t run_times, std::initializer_list<std::size_t> values)
{
    std::ostringstream ostr;
    bound_obit_buffer<bits> bit_obuf(ostr);
    for (std::size_t i = 0u; i < run_times; ++i)
    for (std::size_t value : values)
    {
        bit_obuf << value;
    }
    bit_obuf.flush();

    std::istringstream istr(ostr.str());
    bound_ibit_buffer<bits> bit_ibuf(istr);
    for (std::size_t i = 0u; i < run_times; ++i)
    for (std::size_t value : values)
    {
        std::uint32_t n;
        bit_ibuf >> n;
        assert(n == value, mention(n), mention(value));
    }
}

unittest
{
    for (std::size_t run_times = 1u; run_times < 5; ++run_times)
    {
        unit_test_template<12>(run_times, { 0b1111'1111'1111, 0b1010'0000'0101, 0b0111'1001'1110 });
        unit_test_template<14>(run_times, { 0b11011'0110'11011, 0b01110'1001'01110, 0b10110'0000'01101 });
    }
}
end_unittest

unittest
{
    std::ostringstream ostr1, ostr2;
    obit_buffer<10> obit_buf;
    obit_buf.put(ostr1, 0b10101'01010, 0b01010'10101);
    obit_buf.put(ostr1, { 0b11100'11000 });
    obit_buf.flush(ostr1);
    obit_buf.put(ostr2, 0b10101'01010, 0b01010'10101, 0b11100'11000);
    obit_buf.flush(ostr2);
    assert(ostr1.str() == ostr2.str(), mention(ostr1.str()), mention(ostr2.str()));
}
end_unittest
