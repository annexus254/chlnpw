#include "../include/hash.h"
#include "../include/utils.h"
#include <string.h>

#define ERROR -1
#define SUCCESS 0

#define SET_ERR_RETURN(error_message)       \
    {                                       \
        result.result_code = ERROR;         \
        strcpy(result.hash, error_message); \
        return &result;                     \
    }

CHLNPW_NOTHROW
hash_result *
gen_hash(const char *algorithm, const char *pwd, const char *salt) CHLNPW_NOEXCEPT
{
    static hash_result result;
    char *prefix, *setting, *hash;
    int check_salt_res;

    if (strcmp(algorithm, "yescrypt") == 0)
        prefix = "$y$";
    else if (strcmp(algorithm, "gost-yescrypt") == 0)
        prefix = "$gy$";
    else if (strcmp(algorithm, "scrypt") == 0)
        prefix = "$7$";
    else if (strcmp(algorithm, "bcrypt") == 0)
        prefix = "$2b$";
    else if (strcmp(algorithm, "sha512crypt") == 0)
        prefix = "$6$";
    else if (strcmp(algorithm, "sha256crypt") == 0)
        prefix = "$5$";
    else if (strcmp(algorithm, "sha1crypt") == 0)
        prefix = "$sha1$";
    else if (strcmp(algorithm, "sunmd5") == 0)
        prefix = "$md5$";
    else if (strcmp(algorithm, "md5crypt") == 0)
        prefix = "$1$";
    else if (strcmp(algorithm, "bsdicrypt") == 0)
        prefix = "$_$";
    else if (strcmp(algorithm, "descrypt") == 0)
        prefix = "";
    else if (strcmp(algorithm, "nt") == 0)
        prefix = "$3$";
    else
    {
        SET_ERR_RETURN("invalid hashing algorithm");
    }

    if (salt != NULL)
    {
        setting = strcat(result.hash, prefix);
        setting = strcat(result.hash, salt);
        check_salt_res = crypt_checksalt(setting);
        if ((check_salt_res != CRYPT_SALT_OK) && (check_salt_res != CRYPT_SALT_METHOD_LEGACY))
            SET_ERR_RETURN("invalid salt");
    }
    else
    {
        setting = crypt_gensalt(prefix, 0, NULL, 0);
        if ((setting == NULL) || (setting[0] == '*'))
            SET_ERR_RETURN(strerror(errno));
    }

    hash = crypt(pwd, setting);
    if ((hash == NULL) || (hash[0] == '*'))
    {
        if (errno == EINVAL)
        {
            SET_ERR_RETURN("salt given when none is expected for the provided algorithm")
        }
        else
            SET_ERR_RETURN(strerror(errno));
    }

    result.result_code = SUCCESS;
    strcpy(result.hash, hash);
    return &result;
}