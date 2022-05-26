#ifndef _UTILS_H_
#define _UTILS_H_

#include "config.h"
#include <string.h>
#include <errno.h>

#define SYSCALL_ERR_CHECK(expression, function)   \
    if ((expression) < 0)                         \
    {                                             \
        func_err_exit(function, strerror(errno)); \
    }

__BEGIN_DECLS

CHLNPW_NOTHROW bool is_privileged() CHLNPW_NOEXCEPT;

CHLNPW_NORETURN
CHLNPW_NOTHROW void func_err_exit(const char *func, const char *msg) CHLNPW_NOEXCEPT;

CHLNPW_NORETURN
CHLNPW_NOTHROW void err_exit(const char *msg) CHLNPW_NOEXCEPT;

CHLNPW_NORETURN
CHLNPW_NOTHROW void print_usage_info(int exit_status) CHLNPW_NOEXCEPT;

__END_DECLS

#endif