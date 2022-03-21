#ifndef _SHADOW_UTILS_H_
#define _SHADOW_UTILS_H_

#include "config.h"
#include <string>

extern bool is_processed;

std::string
get_algorithm(const std::string &hash);

std::string
get_salt(const std::string &algorithm ,const std::string &hash);

std::string
get_hash(const std::string &algorithm , const std::string &hash);

std::string
get_misc(const std::string &algorithm , const std::string &hash);

#endif