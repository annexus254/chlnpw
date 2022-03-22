#include "include/chlnpw.h"

#include <getopt.h>
#include <pwd.h>
#include <shadow.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>

int main(int argc, char **argv, char **envp)
{
    bool list_users, verbose, list_pwds, create_backup;
    int option, source_backup_fd, dest_backup_fd;
    char read_buffer[BUFSIZ];
    std::string algorithm, hash, salt, misc, dest_filename;
    spwd *sp_ent;
    ssize_t read_bytes, written_bytes;

    list_users = verbose = list_pwds = create_backup = false;

    while ((option = getopt(argc, argv, "uvpb")) > 0)
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
        case 'b':
            create_backup = true;
            break;
        default:
            exit(EXIT_FAILURE);
            break;
        }
    }

    if (create_backup)
    {
        if (optind >= argc)
            err_exit("No path has been provided");

        std::string temp_dest_filename = argv[optind];
        dest_filename = basename(&temp_dest_filename[0]);
        SYSCALL_ERR_CHECK(source_backup_fd = open(argv[optind], O_RDONLY), "open(source_file)");
        SYSCALL_ERR_CHECK(dest_backup_fd = open(dest_filename.c_str(), O_CREAT | O_TRUNC | O_WRONLY , S_IRUSR | S_IWUSR | S_IRGRP), "open(destination file)");

        while (read_bytes = read(source_backup_fd, read_buffer, BUFSIZ))
        {
            SYSCALL_ERR_CHECK(read_bytes, "read(source_file)");
            written_bytes = write(dest_backup_fd, read_buffer, read_bytes);
            SYSCALL_ERR_CHECK(written_bytes, "write(destination_file)");
            if(written_bytes != read_bytes)
                SYSCALL_ERR_CHECK(-1 , "write(destination file)");
        }

        SYSCALL_ERR_CHECK(close(source_backup_fd), "close(source_file)");
        SYSCALL_ERR_CHECK(close(dest_backup_fd), "close(destination_file)");
    }

    if (list_users && list_pwds)
    {
        if (!is_privileged())
            err_exit("Root privileges needed in order to access the shadow file");

        std::cout << std::left;
        if (verbose)
        {
            std::cout << std::setw(20) << "User"
                      << std::setw(8) << "UID"
                      << std::setw(8) << "GID"
                      << std::setw(15) << "Algorithm"
                      << std::setw(35) << "Salt"
                      << std::setw(88) << "Hash"
                      << std::setw(20) << "Misc"
                      << '\n'
                      << std::setw(20) << "----"
                      << std::setw(8) << "---"
                      << std::setw(8) << "---"
                      << std::setw(15) << "--------"
                      << std::setw(35) << "----"
                      << std::setw(88) << "----"
                      << std::setw(20) << "----"
                      << '\n';
            for (passwd *ent = getpwent(); ent != NULL; ent = getpwent())
            {
                is_processed = false;
                sp_ent = getspnam(ent->pw_name);

                algorithm = get_algorithm(sp_ent->sp_pwdp);
                salt = get_salt(algorithm, sp_ent->sp_pwdp);
                hash = get_hash(algorithm, sp_ent->sp_pwdp);
                misc = get_misc(algorithm, sp_ent->sp_pwdp);

                std::cout << std::setw(20) << ent->pw_name
                          << std::setw(8) << ent->pw_uid
                          << std::setw(8) << ent->pw_gid
                          << std::setw(15) << algorithm
                          << std::setw(35) << salt
                          << std::setw(88) << hash
                          << std::setw(20) << misc
                          << '\n';
            }
            endpwent();
        }
        else
        {
            std::cout << std::setw(20) << "User"
                      << std::setw(15) << "Algorithm"
                      << std::setw(35) << "Salt"
                      << std::setw(88) << "Hash"
                      << '\n'
                      << std::setw(20) << "----"
                      << std::setw(15) << "--------"
                      << std::setw(35) << "----"
                      << std::setw(88) << "----"
                      << '\n';
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
                          << '\n';
            }
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
            std::cout << std::setw(20) << "User"
                      << std::setw(88) << "Hash"
                      << '\n'
                      << std::setw(20) << "----"
                      << std::setw(88) << "----"
                      << '\n';
        }

        for (spwd *ent = getspent(); ent != NULL; ent = getspent())
        {
            is_processed = false;
            if (verbose)
            {
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
            else
            {
                std::cout << std::setw(20) << ent->sp_namp
                          << std::setw(88) << ent->sp_pwdp
                          << '\n';
            }
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