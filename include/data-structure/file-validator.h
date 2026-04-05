#ifndef FILE_VALIDATOR
#define FILE_VALIDATOR

#include <string>

namespace Validator
{

bool avlValidator(const std::string &filename);

bool heapValidator(const std::string &filename);

} // namespace Validator


#endif