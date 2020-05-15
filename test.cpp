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

//If we put the padding in the front, does it become transparent?
//Do we need to do msv_first false & reverse the results?
//For 1 byte, the value of msv_first doesn't matter.
//For 2 bytes, 0x0F0E, msv_first true gives  0x0F0E.
//                   & msv_first false gives 0x0E0F.
//The difficulty with base36 is that it is hard to break it up into multiple
//chunks if you want to be able to handle more data than you're willing to
//have in memory.
std::string base36_encode(std::string_view in)
{
    namespace bic = base_integer_conversion;
    using namespace boost::multiprecision;
    cpp_int encoder;
    import_bits(encoder, in.begin(), in.end(), 8);
    return bic::ntos(encoder, 36);
}

std::string base36_decode(std::string_view in)
{
    namespace bic = base_integer_conversion;
    using namespace boost::multiprecision;
    cpp_int decoder { bic::ston<cpp_int>(in, 36) };
    std::string out;
    export_bits(decoder, std::back_inserter(out), 8);
    return out;
}

//a      -> ME======
//ab     -> MFRA====
//abc    -> MFRGG===
//abcd   -> MFRGGZA=
//abcde  -> MFRGGZDF
//abcdef -> MFRGGZDFMY======

//I used this site to validate my output.
//https://cryptii.com/pipes/base32
std::string base32_encode(std::string_view in)
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

void test_base32(std::string_view in)
{
    //TODO: Round-trip & validation
    const auto out { base32_encode(in) };
    std::cout << "base32: " << in << " -> " << out << '\n';
}

void report(bool b)
{
    if (b) std::cout << "PASS\n";
    else std::cout << "FAIL\n";
}

void test_base36(std::string_view in)
{
    const auto out { base36_encode(in) };
    const auto back { base36_decode(out) };
    std::cout << "base36: " << in << " -> " << out << " -> " << back << ' ';
    report(back == in);
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

    std::vector<std::string> test_strings {
        "a", "ab", "abc", "abcd", "abcde", "abcdef"
    };

    for (const auto& s: test_strings) {
        test_base32(s);
    }

    for (const auto& s: test_strings) {
        test_base36(s);
    }

    test_count_digits();
}
