#ifndef _UTILS_H_
#define _UTILS_H_

#include "config.h"

__BEGIN_DECLS

CHLNPW_NOTHROW bool is_privileged() CHLNPW_NOEXCEPT;

CHLNPW_NORETURN
CHLNPW_NOTHROW void func_err_exit(const char *func, const char *msg) CHLNPW_NOEXCEPT;

CHLNPW_NORETURN
CHLNPW_NOTHROW void err_exit(const char *msg) CHLNPW_NOEXCEPT;

__END_DECLS

#endif