#pragma once

#include <memory>
#include <string>


#include "util/NonCopyable.h"

namespace digitalbits
{
class Config;
/**
 * SecretsManager manager is responsible for communication with a secrets storage,
 * e.g. AWS Secrets Manager.
 * 
 */
class SecretsManager : NonMovableOrCopyable
{
  public:
    static std::unique_ptr<SecretsManager> create();

    virtual ~SecretsManager();
    // Get a secret from AWS by its Id (ARN or name).
    virtual std::string getSecretById(const std::string&) const = 0;
};
}
