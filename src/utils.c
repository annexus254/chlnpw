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

CHLNPW_NORETURN
CHLNPW_NOTHROW void print_usage_info(int exit_status) CHLNPW_NOEXCEPT
{
    fprintf(stdout, "\t\t\t\t\t\tCHLNPW\nA small software utility that enables one to quickly view all the user"
                    " accounts present in a given unix system, and optionally also view and modify the corresponding hashes associated with those accounts.\n"
                    "\nUSAGE: chlnpw [OPTIONS] [PATH] [USERNAME] [PASSWORD] [ALGORITHM] [SALT]\n"
                    "OPTIONS:\n\t-h: Display this help information.\n\t"
                    "-b: Create a backup of PATH in the same directory as the chlnpw executable.\n\t"
                    "-p: Display all user accounts and their corresponding hashes.\n\t"
                    "-u: Display all user accounts.\n\t"
                    "-v: Toggle verbosity and display some more useful information.\n\t"
                    "-w: Write hash of PASSWORD to the Database. If no salt is provided then one is generated automatically(recommended)\n"
                    "\nNote that the USERNAME provided should be one that is present in the User's Database. If you are unsure of the USERNAME, first run this program with the -u option first.\n"
                    "ALGORITHM provided should be one of the following: yescrypt , gost-yescrypt , bsdicrypt , sunmd5 , md5crypt , sha256crypt , sha512crypt , nt , descrypt , scrypt , bcrypt , sha1crypt\n"
                    "Ensure that you enter the name of the chosen algorithm exactly as indicated above otherwise the program will not work.\n");
    exit(exit_status);
}