#ifndef FILE_VALIDATOR
#define FILE_VALIDATOR

#include <string>
#include <sstream>

namespace Validator
{

bool avlValidator(const std::string &filename);

bool heapValidator(const std::string &filename);

bool sllValidator(const std::string &filename);

bool trieValidator(const std::string &filename);

bool graphValidator(const std::string &filename);

bool graphValidator(std::stringstream &ss);

} // namespace Validator


#endif