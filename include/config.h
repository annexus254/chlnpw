#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __unix__
#define _XOPEN_SOURCE 700 // Enable SUSv4 features
#else
#error "This program is only meant to be compiled and run on UNIX systems"
#endif

#ifndef __BEGIN_DECLS
#ifdef __cplusplus
#define __BEGIN_DECLS \
    extern "C"        \
    {
#else
#define __BEGIN_DECLS
#endif
#endif

#ifndef __END_DECLS
#ifdef __cplusplus
#define __END_DECLS }
#else
#define __END_DECLS
#endif
#endif

#ifdef __cplusplus
#define CHLNPW_CONSTEXPR constexpr
#else
#define CHLNPW_CONSTEXPR
#endif

#ifdef __cplusplus
#define CHLNPW_NOEXCEPT noexcept
#else
#define CHLNPW_NOEXCEPT
#endif

#ifdef __cplusplus
#define CHLNPW_NOTHROW
#else
#ifdef __GNUC__
#define CHLNPW_NOTHROW __attribute__((nothrow))
#else
#define CHLNPW_NOTHROW
#endif
#endif

#ifndef __cplusplus
#ifdef __has_include
#if __has_include(<stdbool.h>)
#include <stdbool.h>
#else
typedef enum
{
    false,
    true
} bool
#endif
#else
typedef enum
{
    false,
    true
} bool
#endif
#endif

#ifdef __cplusplus
#if __cplusplus >= 200809L
#define CHLNPW_NORETURN [[noreturn]]
#else
#define CHLNPW_NORETURN
#warning "Compile this program using C++11 or greater to enable some important features"
#endif
#else
#ifdef __GNUC__
#define CHLNPW_NORETURN __attribute__((noreturn))
#else
#define CHLNPW_NORETURN
#endif
#endif

#ifndef BUFSIZ
#define BUFSIZ 8192
#endif

#endif //_CONFIG_H_