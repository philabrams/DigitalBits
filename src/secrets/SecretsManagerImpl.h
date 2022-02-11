#pragma once

#include "secrets/SecretsManager.h"

namespace digitalbits
{
class SecretsManagerImpl : public SecretsManager
{
  Application& mApp;

public:
  SecretsManagerImpl(Application& app);
  ~SecretsManagerImpl();
  std::string getSecret(const std::string&) const override;
};
}
