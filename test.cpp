#include <cstdint>
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpessimizing-move"
#include <boost/multiprecision/cpp_int.hpp>
#pragma GCC diagnostic pop

#include "bic.hpp"

int main(int argc, const char** argv)
{
    namespace bic = base_integer_conversion;
    using Int = boost::multiprecision::cpp_int;

    Int n { INTMAX_MAX };
    n *= 2;
    auto s { bic::ntos(n, 36) };
    std::cout << n << '\n' << s << '\n' << bic::ston<Int>(s, 36) << '\n';
}
