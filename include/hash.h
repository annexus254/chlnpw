#ifndef __HASH_H_
#define __HASH_H_

#include "config.h"

#ifdef __has_include
#if __has_include(<crypt.h>)
#include <crypt.h>
#else
#error "Please install libcrypt or libxcrypt in your system"
#endif
#else
#include <crypt.h>
#endif

typedef struct
{
    int result_code;
    char hash[CRYPT_OUTPUT_SIZE];
} hash_result;

__BEGIN_DECLS

CHLNPW_NOTHROW
hash_result *
gen_hash(const char *algorithm, const char *pwd, const char *salt) CHLNPW_NOEXCEPT;

__END_DECLS

#endif