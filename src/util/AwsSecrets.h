#pragma once

#include <string>

namespace digitalbits
{
// Get a secret from AWS by its Id (ARN or name).
std::string getSecretById(const std::string& secretId);
}