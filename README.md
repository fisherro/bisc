# Bisc

Bisc is a header-only C++ library for converting arbitrary integer types to strings and back using any base.

Bisc might stand for something like "Base Integer String Conversions". Or maybe not.

## Why?

### String to integer

Say you want to convert an string to an arbitrary integer type using an arbitrary base in generic code.

`Std::stoi` and friends work for bases from 2 to 36, but they only work with the built-in types, and they aren't easy to use in generic code as the type of the result is encoded in the name of the functions.

`Std::from_chars` works for bases from 2 to 36 and is easy to use in generic code, but it still only works for the built-in types.

`Boost::lexical_cast` is great for converting between various integer types and strings. It works with any type that is streamable. But it only works for base 10.

### Integer to string

Now what if you want to convert an integer of arbitrary type to a string using an arbitrary base?

`Std::printf` and friends only work for built-in types and bases 8, 10, and 16.

`Std::ostream` works with any streamable type but only bases 8, 10, and 16.

`Std::format` only works for bases 2, 8, 10, and 16.

`Std::to_chars` supports arbitrary bases but only works with built-in types.

## This library

### Example

Presenting `bisc::ston` and `bisc::ntos`:

```cpp
using Int = boost::multiprecision::cpp_int;

Int n = Int(INTMAX_MAX);
n *= 2;
std::string s { bisc::ntos(n, 36) };
std::cout << s << '\n' << bisc::ston<Int>(s, 36) << '\n';
```

Note that these functions don't do any of the whitespace skipping or prefix handling that some others do. (Maybe convenience wrapper functions will be provided for that kind of thing.)

### bisc::ston

```cpp
#include <bisc.hpp>
template <typename N>
N bisc::ston(std::string_view s, const unsigned base = 10);
```

Parses a string as an integer.

`N`: An integer type that should be used for the result.

`s`: A string to convert to an integer. It may start with a `+` or `-` character. After that, it should only contain valid digits of the given base.

`base`: The base must be between 2 and 36 inclusive. The digits expected will be the first `base` characters from `0123456789abcdefghijklmnopqrstuvwxyz`. Uppercase versions of the digits will also be accepted.

An exception will be thrown if `s` contains no valid digits.

An exception will be thrown if `s` contains any invalid digits (except for any leading `+` or `-`).

An exception will be thrown if an invalid `base` is specified.

### bisc::ntos

```cpp
#include <bisc.hpp>
template <typename N>
std::string bisc::ntos(N n, const unsigned base = 10);
```

Renders an integer as a string.

`n`: The integer to render.

`base`: The base must be between 2 and 36 inclusive. The digits used will be the first `base` characters from `0123456789abcdefghijklmnopqrstuvwxyz`.

An exception will be thrown if an invalid `base` is specified.

For negative integers, the string will begin with `-`. For positive integers, the string will _not_ begin with `+`.

### bisc::converter

The `bisc::converter` class provides the same functionality as `bisc::ston` and `bisc::ntos` but using a user-defined set of digits. It can also support bases greater than 36, and it provides iterator-based overloads as well.

```cpp
bisc::converter::converter(std::string_view digits);
```

A `bisc::converter` is constructed with the digit set to use.

```cpp
bisc::converter& bisc::converter::coverter36();
```

The static `converter36` member function will return the converter that is used for `bisc::ston` and `bisc::ntos`.

```cpp
size_t max_base() const;
```

The `max_base` member function returns the maximum base which the converter supports. This is equal to the size of the digit set.

```cpp
size_t bisc::converter::count_digits(auto n) const;
size_t bisc::converter::count_digits(auto n, const unsigned base) const;
```

The `count_digits` member function tells you how many digits the given number will be when converted to a string. If a base isn't given, the converter's max base is used.

```cpp
auto bisc::converter::ston(auto s) const;
auto bisc::converter::ston(auto s, const unsigned base) const;
```

These two `ston` overloads convert a range of `char` to an integer. The integer type will need to be provided as a template parameter. If no base is provided, the converter's max base is used.

```cpp
//Example:
bisc::converter converter("ABCDEFGH");
std::uint64_t n = converter.ston<std::uint64_t>(UINT64_MAX);
```

```cpp
auto bisc::converter::ston(auto first, auto last, const unsigned base) const;
```

This overload of `ston` takes a pair of iterators to a range of `char`. Otherwise, it is the same as the `ston` overloads above.

```cpp
std::string bisc::converter::ntos(auto n) const;
std::string bisc::converter::ntos(auto n, const unsigned base) const;
```

These two overloads of `ntos` convert the given integer into a `std::string`. If no base is provided, the max base of the converter is used.

```cpp
auto bisc::converter::ntos(auto n, auto out, const unsigned base) const;
```

This overload of `ntos` writes the `char`s generated to the given output iterator. It returns an iterator one past the last `char` written.
