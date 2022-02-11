#pragma once

#include <memory>
#include <string>

#include "util/NonCopyable.h"

namespace digitalbits
{
class Application;
/**
 * SecretsManager manager is responsible for communication with a secrets storage,
 * e.g. AWS Secrets Manager.
 * 
 */
class SecretsManager : NonMovableOrCopyable
{
  public:
    static std::unique_ptr<SecretsManager> create(Application&);

    virtual ~SecretsManager();
    virtual std::string getSecret(const std::string&) const = 0;
};
}
