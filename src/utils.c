#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/utils.h"

CHLNPW_CONSTEXPR CHLNPW_NOTHROW bool is_privileged() CHLNPW_NOEXCEPT
{
    return geteuid() == 0UL;
}

CHLNPW_NORETURN
CHLNPW_CONSTEXPR CHLNPW_NOTHROW void func_err_exit(const char *func, const char *msg) CHLNPW_NOEXCEPT
{
    fprintf(stderr, "%s:%s\n", func, msg);
    exit(EXIT_FAILURE);
}

CHLNPW_NORETURN
CHLNPW_CONSTEXPR CHLNPW_NOTHROW void err_exit(const char *msg) CHLNPW_NOEXCEPT
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}