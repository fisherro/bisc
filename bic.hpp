#include <algorithm>
#include <cctype>
#include <exception>
#include <sstream>
#include <string>
#include <string_view>

namespace base_integer_conversion {
    //TODO: ston that will infer base from prefix
    //TODO: ston that will skip leading whitespace
    //TODO: ntos that will add leading +
    //TODO: ntos that will add base prefix
    //TODO: ntos that will upcase
    //TODO: ntos overflow detection?
    //TODO: ston that will skip (specified?) separator characters?

    template <typename N> N ston(std::string_view s, const unsigned base = 10)
    {
        //TODO: Don't allow leading '-' for unsigned types.
        if (s.size() < 1) {
            throw std::invalid_argument("No digits found");
        }

        if ((base < 2) or (base > 36)) {
            std::ostringstream message;
            message << __func__ << ": bad base = " << base;
            throw std::invalid_argument(message.str());
        }

        auto cton = [base](char c)
        {
            std::string_view lookup{"0123456789abcdefghijklmnopqrstuvwxyz"};
            char lower = std::tolower(c);
            auto n = lookup.find(lower);
            if (n >= base) {
                std::ostringstream message;
                message << __func__ << ": bad digit = " << c;
                throw std::invalid_argument(message.str());
            }
            return n;
        };

        bool negative{false};
        if ('+' == s[0]) {
            s.remove_prefix(1);
        } else if ('-' == s[0]) {
            negative = true;
            s.remove_prefix(1);
        }

        if (s.size() < 1) {
            throw std::invalid_argument("No digits found");
        }

        N n{0};
        for (char c: s) {
            n = (n * base) + cton(c);
        }

        if (negative) return -n;
        return n;
    }

    template <typename N> std::string ntos(N n, const unsigned base = 10)
    {
        if ((base < 2) or (base > 36)) {
            std::ostringstream message;
            message << __func__ << ": bad base = " << base;
            throw std::invalid_argument(message.str());
        }

        bool negative{false};
        if (n < 0) {
            negative = true;
            n = -n;
        }

        std::string_view lookup{"0123456789abcdefghijklmnopqrstuvwxyz"};
        std::string s;
        for (; n > 0; n /= base) {
            size_t i { n % base };
            s.push_back(lookup[i]);
        }

        if (negative) s.push_back('-');
        std::reverse(s.begin(), s.end());
        return s;
    }
}
