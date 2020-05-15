#include <cstdint>
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpessimizing-move"
#include <boost/multiprecision/cpp_int.hpp>
#pragma GCC diagnostic pop

#include <boost/type_index.hpp>

#include "bisc.hpp"

const std::vector<std::pair<std::string, std::string>> base32_test_data {
    { "a",      "ME======" },
    { "ab",     "MFRA====" },
    { "abc",    "MFRGG===" },
    { "abcd",   "MFRGGZA=" },
    { "abcde",  "MFRGGZDF" },
    { "abcdef", "MFRGGZDFMY======" },
};

template <typename Int>
void test(Int n)
{
    auto s { bisc::ntos     (n, 36) };
    Int  m { bisc::ston<Int>(s, 36) };
    std::cout << (n == m? "PASS": "FAIL") << ": ntos/ston<"
        << boost::typeindex::type_id<Int>().pretty_name() << ">: "
        << n << " -> \"" << s << "\" -> " << m << '\n';
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
    using namespace boost::multiprecision;
    cpp_int encoder;
    import_bits(encoder, in.begin(), in.end(), 8);
    return bisc::ntos(encoder, 36);
}

std::string base36_decode(std::string_view in)
{
    using namespace boost::multiprecision;
    cpp_int decoder { bisc::ston<cpp_int>(in, 36) };
    std::string out;
    export_bits(decoder, std::back_inserter(out), 8);
    return out;
}

//I used this site to validate my output.
//https://cryptii.com/pipes/base32
//Could operate in chunks of 5 input bytes.
std::string base32_encode(std::string_view in)
{
    using namespace boost::multiprecision;
    //Import bits
    cpp_int encoder;
    import_bits(encoder, in.begin(), in.end(), 8);
    //Bit shift
    auto padding = 5 - ((in.size() * 8) % 5);
    if ((padding > 0) && (padding < 5)) encoder <<= padding;
    //ntos
    //The RFC 4648 digit set:
    bisc::converter converter{"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"};
    std::string out { converter.ntos(encoder) };
    //Padding
    padding = 8 - (out.size() % 8);
    if ((padding > 0) and (padding < 8)) out += std::string(padding, '=');
    return out;
}

//Could operator in chunks of 8 input bytes.
std::string base32_decode(std::string_view in)
{
    using namespace boost::multiprecision;
    //Remove padding
    auto last { in.find_last_not_of('=') };
    if (in.npos == last) return "";
    auto padding { in.size() - last - 1};
    in.remove_suffix(padding);
    //ston
    //The RFC 4648 digit set:
    bisc::converter converter{"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"};
    cpp_int decoder { converter.ston<cpp_int>(in) };
    //Bit shift
    switch (padding) {
        case 0: /* no-op */    break;
        case 1: decoder >>= 3; break;
        case 3: decoder >>= 1; break;
        case 4: decoder >>= 4; break;
        case 6: decoder >>= 2; break;
        default: return "error";
    }
    //Export bits
    std::string out;
    export_bits(decoder, std::back_inserter(out), 8);
    return out;
}

void test_base32(std::string_view in, std::string_view expected)
{
    const auto out  { base32_encode(in)  };
    const auto back { base32_decode(out) };
    bool ok { out == expected };
    if (ok) ok = { in == back };
    std::cout << (ok? "PASS": "FAIL") << ": base32: "
        << in << " -> " << out << " -> " << back << '\n';
}

void test_base36(std::string_view in)
{
    const auto out { base36_encode(in) };
    const auto back { base36_decode(out) };
    std::cout << (back == in? "PASS": "FAIL") << ": base36: "
        << in << " -> " << out << " -> " << back << '\n';
}

void test_count_digits()
{
    auto base36 { bisc::converter::converter36() };
    auto report = [&base36](int n, int base, size_t expected_count)
    {
        auto count {
            (36 == base)
                ? base36.count_digits(n)
                : base36.count_digits(n, base)
        };
        std::cout << (count == expected_count? "PASS": "FAIL")
            << ": base36.count_digits(" << n << ", " << base << ") -> "
            << count << " (expected " << expected_count << ")\n";
    };
    report(36, 36, 2);
    report(123, 10, 3);
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

    for (const auto& pair: base32_test_data) {
        test_base32(pair.first, pair.second);
    }

    std::vector<std::string> test_strings {
        "a", "ab", "abc", "abcd", "abcde", "abcdef"
    };

    for (const auto& s: test_strings) {
        test_base36(s);
    }

    test_count_digits();

    bisc::converter converter("ABCDEFGH");
    std::uint64_t n = converter.ston<std::uint64_t>(std::string_view("ACEG"));
    std::cout << n << '\n';
}
