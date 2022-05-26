#include "../include/io.h"
#include "../include/utils.h"

#include <sys/stat.h>

#include <fstream>
#include <string>
#include <cstring>

#define SUCCESS 0
#define ERROR -1

#include <iostream>

int first_delimiter_index;

static inline std::string
get_username(const char *entry)
{
    std::string username;
    do
    {
        if (*entry != ':')
            username += *entry,
                first_delimiter_index++;
        else
            return username;
    } while (*(entry++));
    return username;
}

int write_hash(const char *path, const char *username, const char *hash)
{
    std::fstream shadow_file{path, std::ios_base::in | std::ios_base::out};
    char *shadow_line;
    std::string shadow_line_converted;
    int starting_offset;
    std::size_t shadow_line_len;
    struct stat shadow_file_stat;
    bool username_found = false;

    if(!is_privileged())
        func_err_exit("write_hash()" , "You must be root to perform this operation");

    SYSCALL_ERR_CHECK(stat(path, &shadow_file_stat), "stat(shadow_file)");
    shadow_line = new char[shadow_file_stat.st_size];

    while (shadow_file.getline(shadow_line, shadow_file_stat.st_size))
    {
        if (get_username(shadow_line) == username)
        {
            username_found = true;
            shadow_line_len = strlen(shadow_line);
            starting_offset = shadow_file.tellg() - (std::streampos)shadow_line_len - 1;
            shadow_line_converted += username;
            shadow_line_converted += ":";
            shadow_line_converted += hash;
            shadow_line_converted += ":";
            for (size_t i = 0, colon_count = 0; i < shadow_line_len; i++)
            {
                if (shadow_line[i] == ':')
                {
                    colon_count++;
                    continue;
                }
                if (colon_count >= 2)
                {
                    for (size_t j = i; j < shadow_line_len; j++)
                        shadow_line_converted += shadow_line[j];
                    shadow_line_converted += '\n';
                    break;
                }
            }
            memset(shadow_line, 0, shadow_file_stat.st_size);
            shadow_file.read(shadow_line, shadow_file_stat.st_size); // Read the rest of the file
            break;
        }
    }

    if (username_found)
    {
        shadow_file.clear();
        shadow_file.seekp(starting_offset);
        shadow_file << shadow_line_converted;
        shadow_file << shadow_line;
        shadow_file.close();
        delete[] shadow_line;
        return SUCCESS;
    }
    else
    {
        shadow_file.close();
        delete[] shadow_line;
        return ERROR;
    }
}
