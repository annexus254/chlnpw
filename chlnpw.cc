#include "include/chlnpw.h"

#include <getopt.h>
#include <pwd.h>
#include <shadow.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>

int main(int argc, char **argv, char **envp)
{
    bool list_users, verbose, list_pwds;
    int option;
    std::string algorithm, hash, salt, misc;

    list_users = verbose = list_pwds = false;

    while ((option = getopt(argc, argv, "uvp")) > 0)
    {
        switch (option)
        {
        case 'u':
            list_users = true;
            break;
        case 'v':
            verbose = true;
            break;
        case 'p':
            list_pwds = true;
            break;
        default:
            exit(EXIT_FAILURE);
            break;
        }
    }

    if (list_users && list_pwds)
    {
        std::cout << std::left;
        if (verbose)
        {
        }
        else
        {
        }
    }
    else if (list_pwds)
    {
        if (!is_privileged())
            err_exit("Root privileges needed in order to access the shadow file");

        std::cout << std::left;
        if (verbose)
        {
            std::cout << std::setw(20) << "User"
                      << std::setw(15) << "Algorithm"
                      << std::setw(35) << "Salt"
                      << std::setw(88) << "Hash"
                      << std::setw(20) << "Misc"
                      << '\n'
                      << std::setw(20) << "----"
                      << std::setw(15) << "--------"
                      << std::setw(35) << "----"
                      << std::setw(88) << "----"
                      << std::setw(20) << "----"
                      << '\n';
        }
        else
        {
        }

        for (spwd *ent = getspent(); ent != NULL; ent = getspent())
        {
            is_processed = false;
            algorithm = get_algorithm(ent->sp_pwdp);
            hash = get_hash(algorithm, ent->sp_pwdp);
            salt = get_salt(algorithm, ent->sp_pwdp);
            misc = get_misc(algorithm, ent->sp_pwdp);
            std::cout << std::setw(20) << ent->sp_namp
                      << std::setw(15) << algorithm
                      << std::setw(35) << salt
                      << std::setw(88) << hash
                      << std::setw(20) << misc
                      << '\n';
        }
        endspent();
    }
    else if (list_users)
    {
        std::cout << std::left;
        if (verbose)
            std::cout << std::setw(25) << "User"
                      << std::setw(25) << "UID"
                      << std::setw(25) << "GID"
                      << std::setw(25) << "Home"
                      << std::setw(25) << "Shell"
                      << std::setw(25) << "Comment"
                      << '\n'
                      << std::setw(25) << "----"
                      << std::setw(25) << "---"
                      << std::setw(25) << "---"
                      << std::setw(25) << "----"
                      << std::setw(25) << "-----"
                      << std::setw(25) << "-------"
                      << '\n';
        else
            std::cout << "User\n----\n";

        for (passwd *ent = getpwent(); ent != NULL; ent = getpwent())
        {
            if (verbose)
            {
                std::cout << std::setw(25) << ent->pw_name
                          << std::setw(25) << ent->pw_uid
                          << std::setw(25) << ent->pw_gid
                          << std::setw(25) << ent->pw_dir
                          << std::setw(25) << ent->pw_shell
                          << std::setw(25) << ent->pw_gecos
                          << '\n';
            }
            else
            {
                std::cout << ent->pw_name << '\n';
            }
        }
        endpwent();
    }

    return EXIT_SUCCESS;
}