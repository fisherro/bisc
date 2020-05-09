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
}
