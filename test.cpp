//TODO: Do validation tests

#include <cstdint>
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpessimizing-move"
#include <boost/multiprecision/cpp_int.hpp>
#pragma GCC diagnostic pop

#include "bic.hpp"

template <typename Int>
void test(Int n)
{
    namespace bic = base_integer_conversion;
    std::cout << n << " -> ";
    auto s { bic::ntos(n, 36) };
    std::cout << s << " -> ";
    n = bic::ston<Int>(s, 36);
    std::cout << n << '\n';
}

/*
 * test = $74 65 73 74
 * 0111 0100 0110 0101 0111 0011 0111 0100
 * base 36 is between 5 & 6 bits per digit
 */
std::string base36_encode(std::string_view in)
{
    namespace bic = base_integer_conversion;
    using namespace boost::multiprecision;
    cpp_int encoder;
    import_bits(encoder, in.begin(), in.end(), 8);
    return bic::ntos(encoder, 36);
}

//a      -> ME======
//ab     -> MFRA====
//abc    -> MFRGG===
//abcd   -> MFRGGZA=
//abcde  -> MFRGGZDF
//abcdef -> MFRGGZDFMY======

//I used this site to validate my output.
//https://cryptii.com/pipes/base32
std::string base32_encode(std::string in)
{
    namespace bic = base_integer_conversion;
    using namespace boost::multiprecision;
    cpp_int encoder;
    import_bits(encoder, in.begin(), in.end(), 8);
    auto padding = 5 - ((in.size() * 8) % 5);
    if ((padding > 0) && (padding < 5)) encoder <<= padding;
    //The RFC 4648 digit set:
    bic::converter converter{"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"};
    std::string out { converter.ntos(encoder) };
    padding = 8 - (out.size() % 8);
    if ((padding > 0) and (padding < 8)) out += std::string(padding, '=');
    return out;
}

void report(bool b)
{
    if (b) std::cout << "PASS\n";
    else std::cout << "FAIL\n";
}

void test_count_digits()
{
    namespace bic = base_integer_conversion;
    auto base36 { bic::converter::converter36() };
    report(2 == base36.count_digits(36));
    report(3 == base36.count_digits(123, 10));
}

int main(int argc, const char** argv)
{
    int plain_int { INT_MAX / 2 };
    test(plain_int);

    unsigned plain_unsigned { UINT_MAX / 2 };
    test(plain_unsigned);

    using namespace boost::multiprecision;

    cpp_int big_int { INTMAX_MAX };
    big_int *= 2;
    test(big_int);

    auto kibi_uint { std::numeric_limits<uint1024_t>::max() };
    test(kibi_uint);

    auto checked { std::numeric_limits<checked_int1024_t>::max() };
    test(checked);

    char to_encode[] { "test" };
    std::string encoded { base36_encode(to_encode) };
    std::cout << to_encode << " -> " << encoded << '\n';
    std::cout << to_encode << " -> " << base32_encode(to_encode) << '\n';
    std::cout << "testy" << " -> " << base32_encode("testy") << '\n';
    std::cout << base32_encode("a") << '\n';
    std::cout << base32_encode("ab") << '\n';
    std::cout << base32_encode("abc") << '\n';
    std::cout << base32_encode("abcd") << '\n';
    std::cout << base32_encode("abcde") << '\n';
    std::cout << base32_encode("abcdef") << '\n';

    test_count_digits();
}
