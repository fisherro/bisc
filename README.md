# Why?

## String to integer

Say you want to convert an string to an arbitrary integer type using an arbitrary base in generic code.

`Std::stoi` and friends work for bases from 2 to 36, but they only work with the built-in types, and they aren't easy to use in generic code as the type of the result is encoded in the name of the functions.

`Std::from_chars` works for bases from 2 to 36 and is easy to use in generic code, but it still only works for the built-in types.

`Boost::lexical_cast` is great for converting between various integer types and strings. It works with any type that is streamable. But it only works for base 10.

## Integer to string

Now what if you want to convert an integer of arbitrary type to a string using an arbitrary base?

`Std::printf` and friends only work for built-in types and bases 8, 10, and 16.

`Std::ostream` works with any streamable type but only bases 8, 10, and 16.

`Std::format` only works for bases 2, 8, 10, and 16.

`Std::to_chars` supports arbitrary bases but only works with built-in types.

# This library

## Example

Presenting `ston` and `ntos`:

```cpp
namespace bic = base_integer_conversion;
using Int = boost::multiprecision::cpp_int;

Int n = Int(INTMAX_MAX);
n *= 2;
std::string s { bic::ntos(n, 36) };
std::cout << s << '\n' << bic::ston<Int>(s, 36) << '\n';
```

Note that these functions don't do any of the whitespace skipping or prefix handling that some others do. (Maybe convenience wrapper functions will be provided for that kind of thing.)

## ston

```cpp
#include <bic.hpp>
template <typename N>
N ston(std::string_view s, const unsigned base = 10);
```

Parses a string as an integer.

`N`: An integer type that should be used for the result.

`s`: A string to convert to an integer. It may start with a `+` or `-` character. After that, it should only contain valid digits of the given base.

`base`: The base must be between 2 and 36 inclusive. The digits expected will be the first `base` characters from `0123456789abcdefghijklmnopqrstuvwxyz`. Uppercase versions of the digits will also be accepted.

An exception will be thrown if `s` contains no valid digits.

An exception will be thrown if `s` contains any invalid digits (except for any leading `+` or `-`).

An exception will be thrown if an invalid `base` is specified.

## ntos

```cpp
#include <bic.hpp>
template <typename N>
std::string ntos(N n, const unsigned base = 10);
```

Renders an integer as a string.

`n`: The integer to render.

`base`: The base must be between 2 and 36 inclusive. The digits used will be the first `base` characters from `0123456789abcdefghijklmnopqrstuvwxyz`.

An exception will be thrown if an invalid `base` is specified.

For negative integers, the string will begin with `-`. For positive integers, the string will _not_ begin with `+`.
