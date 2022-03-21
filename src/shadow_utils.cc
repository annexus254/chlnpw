#include "../include/shadow_utils.h"
#include <vector>

bool is_processed = false;
std::vector<std::string> sections;

/**
 * @brief Decomposes the given hash into sections which are then stored in the global vector sections.
 * The is_processed variable is used to optimize the program in the sense that it prevents re-parsing and re-decomposition
 * of the hash.
 * Note that it is therefore the programmer's resposibility to ensure that he/she resets this variable to false, if the 
 * hash passed to the helper(getter) functions below is new.
 * 
 * The delimiter used to delineate the different sections is '$' as specified in the Modular Crypt Format
 * See https://en.wikipedia.org/wiki/Crypt_(C)#:~:text=9%5D%20programming%20languages.-,Key%20derivation%20functions%20supported%20by%20crypt,-%5Bedit%5D for details.
 * 
 * @param hash 
 * @return true 
 * @return false 
 */
static inline bool
process_hash(const std::string &hash)
{
    std::string section;

    if (is_processed)
        return false;
    else
    {
        sections.clear();
        section.clear();
        if (hash[0] != '$')
            section += hash[0];

        for (size_t i = 1, section_num = 0; i < hash.length(); i++)
        {
            if (hash[i] != '$')
                section += hash[i];
            else
            {
                section_num++;
                sections.push_back(section);
                section.clear();
            }
        }
        sections.push_back(section);
        is_processed = true;
    }
    return is_processed;
}

/**
 * @brief Converts the ID stored in the first section of the MCF-compliant string to a more human-friendly string
 * 
 * @param hash 
 * @return std::string 
 */
std::string
get_algorithm(const std::string &hash)
{
    if (hash[0] == '*' || hash[0] == '!')
        return "-";
    else if (hash[0] == '_')
        return "bsdicrypt";

    std::string algo_string;
    process_hash(hash);
    algo_string = sections[0];

    if (algo_string == "y")
        return "yescrypt";
    else if (algo_string == "gy")
    {
        return "gost-yescrypt";
    }
    else if (algo_string == "7")
    {
        return "scrypt";
    }
    else if (algo_string == "6")
    {
        return "sha512crypt";
    }
    else if (algo_string == "5")
    {
        return "sha256crypt";
    }
    else if (algo_string == "sha1")
    {
        return "sha1crypt";
    }
    else if (algo_string == "md5")
    {
        return "sunmd5";
    }
    else if (algo_string == "1")
    {
        return "md5crypt";
    }
    else if (algo_string == "3")
    {
        return "NT";
    }
    else if (algo_string == "2" || algo_string == "2a" || algo_string == "2b" || algo_string == "2x" || algo_string == "2y")
    {
        return "bcrypt";
    }
    else
    {
        if (algo_string.length() == 128)
            return "bigcrypt";
        else if (algo_string[0] == 'm' && algo_string[1] == 'd' && algo_string[2] == '5')
            return "sunmd5";
        else
            return "descrypt";
    }
}

/**
 * @brief Get the salt stored in the provided hash.
 * Note that the salt is the second last section of an MCF-compliant string.
 * 
 * @param algorithm 
 * @param hash 
 * @return std::string 
 */
std::string
get_salt(const std::string &algorithm, const std::string &hash)
{
    process_hash(hash);
    size_t salt_index;

    if (algorithm == "-")
        return "-";
    else if (algorithm == "descrypt")
        return sections[0].substr(0, 2);
    else if (algorithm == "bigcrypt" || algorithm == "bsdicrypt")
        return sections[0].substr(1, 2);
    else if (algorithm == "NT")
        return "-";
    else if(algorithm == "sunmd5")
        return sections[1];
    else
    {
        salt_index = sections.size() - 2;
        return sections[salt_index];
    }
}

/**
 * @brief Get the actual hash stored inside the MCF string 'hash' passed as an argument
 * 
 * @param algorithm 
 * @param hash 
 * @return std::string 
 */
std::string
get_hash(const std::string &algorithm, const std::string &hash)
{
    process_hash(hash);
    size_t hash_index;

    if (algorithm == "-")
        return "-";
    else if (algorithm == "descrypt")
        return sections[0].substr(2);
    else if (algorithm == "bigcrypt" || algorithm == "bsdicrypt")
        return sections[0].substr(3);
    else
    {
        hash_index = sections.size() - 1;
        return sections[hash_index];
    }
}

/**
 * @brief Get any miscellaneous information associated with a given hash e.g. rounds,paramaters e.t.c.
 * 
 * @param algorithm 
 * @param hash 
 * @return std::string 
 */
std::string
get_misc(const std::string &algorithm, const std::string &hash)
{
    size_t misc_index;

    process_hash(hash);
    misc_index = sections.size() - 3;

    if (algorithm == "-")
        return "-";
    else if(algorithm == "sunmd5")
    {
        if(sections[0] == "md5")
            return "-";
        else
            return sections[0].substr(4);
    }
    else if(algorithm == "NT")
    {
        if(sections[1] == "")   
            return "-";
        else
            return sections[1];
    }
    else if (algorithm == "descrypt" || algorithm == "bigcrypt" || algorithm == "bsdicrypt")
        return "-";
    else if (algorithm == "yescrypt" || algorithm == "gost-yescrypt")
    {
        if (sections.size() == 4)
        {
            return "param=" + sections[1];
        }
        else
        {
            return sections[1] + ",param=" + sections[2];
        }
    }
    else
    {
        if(sections.size() > 3)
        {
            return sections[misc_index];
        }
        else
        {
            return "-";
        }
        
    }
}