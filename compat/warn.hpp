#pragma once

#include "macros.hpp"

#include <sstream>
#include <iostream>
#include <locale>
#include <utility>

#include <string>

namespace warn_detail {
template<typename t> using basic_string_stream = std::basic_ostringstream<t, std::char_traits<t>, std::allocator<t>>;
using string_stream = basic_string_stream<wchar_t>;

force_inline void do_warn(string_stream&) {}

template<typename x, typename... xs>
force_inline void do_warn(string_stream& acc, const x& datum, const xs&... rest)
{
    acc << datum;
    if (sizeof...(rest) > 0u)
        acc << L' ';
    do_warn(acc, rest...);
}

template<typename... xs>
never_inline void warn_(const char* file, int line, const char* level, const xs&... seq)
{
    using namespace warn_detail;
    string_stream stream;

    do_warn(stream, seq...);

    std::wcerr << L'[' << level << L' '
               << file << L':' << line
               << L"] "
               << std::boolalpha
               << stream.str()
               << L'\n';
    std::wcerr.flush();
}

} // ns warn_detail

#define otr_impl_warn_base(level, ...) \
    (warn_detail::warn_(__FILE__, __LINE__, (level), __VA_ARGS__))

#define warn(...) \
    otr_impl_warn_base("WARN", __VA_ARGS__)

#define debug(...) \
    otr_impl_warn_base("DEBUG", __VA_ARGS__)

#define crit(...) \
    otr_impl_warn_base("CRIT", __VA_ARGS__)

#if 0
#include <utility>

#define fatal(...)                                  \
    do                                              \
    {                                               \
        otr_impl_warn_base("CRIT", __VA_ARGS__);    \
        *(volatile int*)0 = 0;                      \
        std::quick_exit(255);                       \
    } while (0)
#endif
