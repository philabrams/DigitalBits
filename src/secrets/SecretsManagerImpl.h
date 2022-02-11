#pragma once

#include "secrets/SecretsManager.h"

namespace digitalbits
{
class SecretsManagerImpl : public SecretsManager
{
public:
  ~SecretsManagerImpl();
  std::string getSecret(const std::string&) const override;
};
}
