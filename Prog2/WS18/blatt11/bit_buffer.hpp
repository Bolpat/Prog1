
#include <cstdint>
#include <iostream>

// Can be used with obit_buffer and ibit_buffer to inspect their buffer values.
// An asterisk marks the position of the internal cursor.
// For an obit_buffer, the next inserted element is being inserted after the cursor.
// For an ibit_buffer, the next returned element is the one after the cursor.
// Elements are printed in binary, separated by a space or asterisk.
//
// Usage:
//  print_buffer(std::cout, bit_buf);
// Returns:
//  The stdf::ostream reference given as the first parameter.
template <typename bit_buffer>
std::ostream &print_buffer(std::ostream &stream, const bit_buffer &bit_buf)
{
    // for (std::size_t i = 0u; i < bit_buf.words_read * bit_buf.bits; ++i)
    for (std::size_t i = 0u; i < bit_buf.buffer_bits; ++i)
    {
        stream << (bit_buf.buffer[i / 8u] & (1u << (i % 8u)) ? '1' : '0');
        if (i == bit_buf.bit_index - 1)
        {
            stream << '*';
        }
        else if (i % bit_buf.bits == bit_buf.bits - 1)
        {
            stream << ' ';
        }
    }
    return stream << std::endl;
}

// Note:
//  The number of bits for obit_buffer can be smaller than 8,
//  the number of bits for ibit_buffer must be at least 8.
//  This is due to reading ambiguities for smaller bit counts.

// An obit_buffer has two member functions: put and flush.
// put takes two parameters: a std::ostream reference and a value of any unsigned integer type.
// flush takes one parameter: a std::ostream reference.
// Both functions return the std::ostream reference.
//
// The obit_buffer object holds an internal buffer.
// When flushed or, when the internal buffer is full, before putting a value,
// the contents are copied to the referenced std::ostream.
// For put, if the internal buffer is not full, the referenced std::ostream is not changed.
// The function put can be used with any number of values -- variadic (C++17 or later) or with an std::initializer_list;
// this is equivalent to calling the function separately with the same std::ostream reference.
//
// The reference can be an lvalue reference or an rvalue reference.
//
// Usage:
//      std::ostream o_str;
//      obit_buffer<bits> buf_variable;
//
//      unsigned value;
//      buf_variable.put(o_str, value);
//
//      unsigned value1, value2;
//      buf_variable.put(o_str, value1, value2); // (*)
//      buf_variable.put(o_str, { value1, value2 }); // (**)
//
// The lines with (*) and (**) are equivalent to
//
//      buf_variable.put(o_str, value1), // Note: comma intentional!
//      buf_variable.put(o_str, value2);
//
// Example:
//      std::ostringstream o_str;
//      obit_buffer<12> o_buf;
//      o_buf.put(o_str, 0b1100'1001'0011, 0b0111'1010'1110);
//      // internal buffer full
//      o_buf.flush(o_str);
//      std::assert(o_str.str().size() == 3);
template <std::size_t _bits>
class obit_buffer
{
    static_assert(1 <= _bits, "bits must be at least 1");
    static_assert(_bits <= 8u * sizeof(std::uintmax_t),
        "bits must be less than the number of bits in std::uintmax_t");

    constexpr static std::size_t bits = _bits;
    // the size of the buffer.
    constexpr static std::size_t buffer_size =
        bits % 8 == 0 ? bits / 8 :
        bits % 4 == 0 ? bits / 4 :
        bits % 2 == 0 ? bits / 2 :
        bits;
    constexpr static std::size_t buffer_bits = 8u * buffer_size;
    // the index of the bit to write next.
    std::size_t bit_index = 0;
    // the buffer to hold the values: an array of buffer_size 8-bit values.
    std::uint8_t buffer[buffer_size] = { 0 };

    // for interoperability with ibit_buffer and print_buffer.
    constexpr static std::size_t words_read = buffer_bits / bits;

    friend
    std::ostream &print_buffer<obit_buffer>(std::ostream &, const obit_buffer &);

public:
    std::ostream &put(std::ostream &stream, std::uintmax_t value)
    {
        // bit_mask == 11...1100...00 with bits-many zeros
        constexpr std::uintmax_t bit_mask = ~((uintmax_t(1u) << uintmax_t(bits)) - uintmax_t(1u));
        if (value & bit_mask)
        {
            throw std::invalid_argument("obit_buffer.put: value is too large");
        }

        if (bit_index == 8u * buffer_size)
        {
            stream.write(reinterpret_cast<char*>(&buffer[0]), buffer_size);
            bit_index = 0;
            for (auto& byte : buffer) byte = 0;
        }

        for (std::size_t i = 0u; i < bits; ++i)
        {
            if (value & (1u << i))
                buffer[(bit_index + i) / 8u] |= 1u << ((bit_index + i) % 8u);
        }
        bit_index += bits;
        return stream;
    }

    std::ostream &&put(std::ostream &&stream, std::uintmax_t value)
    {
        return std::move(put(stream, value));
    }

    std::ostream &put(std::ostream &stream, std::initializer_list<std::uintmax_t> values)
    {
        for (std::uintmax_t value : values) put(stream, value);
        return stream;
    }

    std::ostream &&put(std::ostream &&stream, std::initializer_list<std::uintmax_t> values)
    {
        for (std::uintmax_t value : values) put(stream, value);
        return std::move(stream);
    }

#if __cpp_fold_expressions >= 201603L
    template <typename... Integers>
    std::ostream &put(std::ostream &stream, Integers... values)
    {
        (put(stream, static_cast<std::uintmax_t>(values)),...);
        return stream;
        // return (put(stream, values), ..., stream);
    }

    // template <typename... Integers>
    // std::ostream &&put(std::ostream &&stream, Integers... values)
    // {
        // return (put(stream, values), ..., std::move(stream));
    // }
#endif

    std::ostream &flush(std::ostream &stream)
    {
        const size_t n = (bit_index + 7u) / 8u;
        stream.write(reinterpret_cast<char*>(&buffer[0]), n);
        bit_index = 0;
        for (auto &byte : buffer) byte = 0;
        return stream;
    }

    std::ostream &&flush(std::ostream &&stream)
    {
        return std::move(flush(stream));
    }
};


// An ibit_buffer has one member function: get.
// get takes two parameters: a std::istream reference and a reference to any unsigned integer type.
// It returns the std::istream reference.
//
// The ibit_buffer object holds an internal buffer.
// When the internal buffer is empty while trying to extract a value,
// the buffer is reinitialized .
// For put, if the internal buffer is not full, the referenced std::ostream is not changed.
// The function put can be used with any number of values -- variadic (C++17 or later) or with an std::initializer_list;
// this is equivalent to calling the function separately with the same std::ostream reference.
//
// The reference can be an lvalue reference or an rvalue reference.
//
// Usage:
//      std::istream i_str;
//      ibit_buffer<bits> buf_variable;
//
//      unsigned value;
//      buf_variable.get(i_str, value);
//
//      unsigned value1, value2;
//      buf_variable.get(i_str, value1, value2); // (*)
//      buf_variable.get(i_str, { value1, value2 }); // (*)
//
// The lines with (*) and (**) is equivalent to
//
//      buf_variable.get(i_str, value1), // Note: comma intentional!
//      buf_variable.get(i_str, value2);
template <std::size_t _bits>
class ibit_buffer
{
    static_assert(8u <= _bits && _bits <= 8u * sizeof(std::uintmax_t),
        "bits must be between 8 and the number of bits in std::uintmax_t");

    constexpr static std::size_t bits = _bits;
    // the size of the buffer.
    constexpr static std::size_t buffer_size =
        bits % 8 == 0 ? bits / 8 :
        bits % 4 == 0 ? bits / 4 :
        bits % 2 == 0 ? bits / 2 :
        bits;
    constexpr static std::size_t buffer_bits = 8u * buffer_size;
    // the index of the bit to write next.
    std::size_t bit_index = buffer_size * 8u;
    // the buffer to hold the values: an array of buffer_size 8-bit values.
    std::uint8_t buffer[buffer_size] = { 0 };

    // could be avoided by adjusting an unaligned read to the end of the buffer.
    std::size_t words_read = 0;

    friend
    std::ostream &print_buffer<ibit_buffer>(std::ostream &, const ibit_buffer &);

public:
    template <typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
    std::istream &get(std::istream &stream, Integer &value)
    {
        static_assert(sizeof(Integer) * 8u >= bits,
            "ibit_buffer.get: sizeof(Integer) must be large enough to hold the given number of bits");

        // empty buffer?
        if (bit_index == 8u * buffer_size)
        {
            stream.read(reinterpret_cast<char*>(&buffer[0]), buffer_size);
            bit_index = 0;
            words_read = stream.gcount() * 8u / bits;
        }

        // read to end?
        if (bit_index == words_read * bits)
        {
            stream.setstate(std::ios_base::failbit);
            return stream;
        }

        value = 0u;
        for (std::size_t i = 0u; i < bits; ++i)
        {
            if (buffer[(bit_index + i) / 8u] & (1u << ((bit_index + i) % 8u)))
                value |= 1u << i;
        }
        bit_index += bits;
        return stream;
    }

    template <typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
    std::istream &get(std::istream &stream, std::initializer_list<Integer &> values)
    {
        for (Integer &value : values) get(stream, value);
        return stream;
    }

    template <typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
    std::istream &&get(std::istream &&stream, Integer &value)
    {
        return std::move(get(stream, value));
    }

    template <typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
    std::istream &&get(std::istream &&stream, std::initializer_list<Integer &> values)
    {
        for (Integer &value : values) get(stream, value);
        return std::move(stream);
    }

#if __cpp_fold_expressions >= 201603L
    template <typename... Integers, typename = std::enable_if_t<(std::is_integral<Integers>::value && ...)>>
    std::istream &get(std::istream &stream, Integers &... values)
    {
        return (get(stream, values), ..., stream);
    }

    template <typename... Integers, typename = std::enable_if_t<(std::is_integral<Integers>::value && ...)>>
    std::istream &&get(std::istream &&stream, Integers &... values)
    {
        return (get(stream, values), ..., std::move(stream));
    }
#endif
};

// The classes bound_obit_buffer and bound_ibit_buffer bind a std::ostream or std::istream and a obit_buffer or ibit_buffer together,
// and make the streaming operators available.
// The internal stream is intentionally a public member variable.
template <size_t bits>
class bound_obit_buffer
{
    obit_buffer<bits> buffer;

public:
    std::ostream &stream;

    bound_obit_buffer(std::ostream& stream) : stream(stream) { }

    friend bound_obit_buffer  &operator<<(bound_obit_buffer  &self, std::uintmax_t value) { self.buffer.put(self.stream, value); return self; }
    friend bound_obit_buffer &&operator<<(bound_obit_buffer &&self, std::uintmax_t value) { return std::move(self << value); }

    void flush() { buffer.flush(stream); }
};

template <size_t bits>
class bound_ibit_buffer
{
    ibit_buffer<bits> buffer;

public:
    std::istream &stream;

    bound_ibit_buffer(std::istream& stream) : stream(stream) { }

    template <typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
    friend bound_ibit_buffer<bits>  &operator>>(bound_ibit_buffer<bits>  &self, Integer &value) { self.buffer.get(self.stream, value); return self; }
    template <typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
    friend bound_ibit_buffer<bits> &&operator>>(bound_ibit_buffer<bits> &&self, std::uintmax_t &value) { return std::move(self >> value); }
};
