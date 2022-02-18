#pragma once

#include "secrets/SecretsManager.h"

namespace digitalbits
{
class SecretsManagerImpl : public SecretsManager
{
public:
  SecretsManagerImpl();
  ~SecretsManagerImpl();
  std::string getSecretById(const std::string&) const override;
};
}
