#include <algorithm>
#include <cctype>
#include <exception>
#include <sstream>
#include <string>
#include <string_view>

namespace base_integer_conversion {

    //If I wanted to depend on boost, I could use make_function_iterator.
    struct counting_iterator {
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;

        size_t count{0};

        counting_iterator& operator=(const counting_iterator&)
        { return *this; }

        template <typename T>
        void operator=(const T&) {}

        counting_iterator& operator*() { return *this; }
        counting_iterator& operator++() { ++count; return *this; }
        counting_iterator& operator++(int) { ++count; return *this; }
    };

    //The converter class provides for custom digit sets,
    //which can support larger bases.
    struct converter {
        static converter& converter36();

        explicit converter(std::string_view digits): my_digits(digits) {}

        size_t max_base() const { return my_digits.size(); }

        template <typename N>
        size_t count_digits(N n) const { return count_digits(n, max_base()); }

        template <typename N>
        size_t count_digits(N n, const unsigned base) const;

        template <typename N, typename Range>
        N ston(Range s) const { return ston<N>(s, max_base()); }

        //TODO: Should do a move return if possible?
        template <typename N, typename Range>
        N ston(Range s, const unsigned base) const;

        template <typename N, typename First, typename Last>
        N ston(First first, Last last) const
        { return ston<N>(first, last, max_base()); }

        template <typename N, typename First, typename Last>
        N ston(First first, Last last, const unsigned base) const;

        template <typename N>
        std::string ntos(N n) const { return ntos(n, max_base()); }

        template <typename N>
        std::string ntos(N n, const unsigned base) const;

#if 0
        //Need way to disambiguate between this & (n, base) overload.
        template <typename N, typename Out>
        Out ntos(N n, Out out) const { return ntos(n, out, max_base()); }
#endif

        template <typename N, typename Out>
        Out ntos(N n, Out out, const unsigned base) const;

    private:
        template <typename N, typename Out>
        Out ntos_(N n, Out out, const unsigned base) const;

        const std::string my_digits {"0123456789abcdefghijklmnopqrstuvwxyz"};
    };

    converter& converter::converter36()
    {
        static converter base36{"0123456789abcdefghijklmnopqrstuvwxyz"};
        return base36;
    }

    template <typename N>
    size_t converter::count_digits(N n, const unsigned base) const
    {
        auto end { ntos_(n, counting_iterator{}, base) };
        return end.count;
    }

    template <typename N, typename First, typename Last>
    N converter::ston(First first, Last last, const unsigned base) const
    {
        //TODO: Don't allow leading '-' for unsigned types.
        if (first == last) {
            throw std::invalid_argument("Empty string not allowed");
        }

        if ((base < 2) or (base > max_base())) {
            std::ostringstream message;
            message << __func__ << ": bad base = " << base;
            throw std::invalid_argument(message.str());
        }

        auto cton = [&digits = this->my_digits, base](char c)
        {
            char lower = std::tolower(c);
            auto n = digits.find(lower);
            if (n >= base) {
                std::ostringstream message;
                message << __func__ << ": bad digit = " << c;
                throw std::invalid_argument(message.str());
            }
            return n;
        };

        bool negative{false};
        if ('+' == *first) {
            ++first;
        } else if ('-' == *first) {
            negative = true;
            ++first;
        }

        if (first == last) {
            throw std::invalid_argument("No digits found");
        }

        N n{0};
        for (; first != last; ++first) {
            n = (n * base) + cton(*first);
        }

        //TODO: Throw exception for negative string with unsigned integer
        if constexpr (std::numeric_limits<N>::is_signed) {
            if (negative) return -n;
        }
        return n;
    }

    template <typename N, typename Range>
    N converter::ston(Range s, const unsigned base) const
    {
        using std::begin, std::end; //Enable ADL
        return ston<N>(begin(s), end(s), base);
    }

    //TODO: It would be nice if this didn't have to allocate.
    template <typename N, typename Out>
    Out converter::ntos(N n, Out out, const unsigned base) const
    {
        std::string s { ntos_(n, base) };
        return std::copy(s.rbegin(), s.rend(), out);
    }

    template <typename N>
    std::string converter::ntos(N n, const unsigned base) const
    {
        std::string s;
        ntos_(n, std::back_inserter(s), base);
        std::reverse(s.begin(), s.end());
        return s;
    }

    template <typename N, typename Out>
    Out converter::ntos_(N n, Out out, const unsigned base) const
    {
        if ((base < 2) or (base > max_base())) {
            std::ostringstream message;
            message << __func__ << ": bad base = " << base;
            throw std::invalid_argument(message.str());
        }

        bool negative{false};
        if constexpr (std::numeric_limits<N>::is_signed) {
            if (n < 0) {
                negative = true;
                n = -n;
            }
        }

        for (; n > 0; n /= base) {
            size_t i { n % base };
            *out++ = my_digits[i];
        }

        if (negative) *out++ = '-';
        return out;
    }

    //TODO: ston that will infer base from prefix
    //TODO: ston that will skip leading whitespace
    //TODO: ntos that will add leading +
    //TODO: ntos that will add base prefix
    //TODO: ntos that will upcase
    //TODO: ntos overflow detection?
    //TODO: ston that will skip (specified?) separator characters?

    template <typename N>
    N ston(std::string_view s, const unsigned base = 10)
    {
        return converter::converter36().ston<N>(s, base);
    }

    template <typename N>
    std::string ntos(N n, const unsigned base = 10)
    {
        return converter::converter36().ntos(n, base);
    }
}
