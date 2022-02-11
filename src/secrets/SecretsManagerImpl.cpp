#include "secrets/SecretsManagerImpl.h"

namespace digitalbits 
{
std::unique_ptr<SecretsManager>
SecretsManager::create(Application& app)
{
    auto secretsManagerPtr = std::make_unique<SecretsManagerImpl>(app);
    return secretsManagerPtr;
}

SecretsManager::~SecretsManager() = default;
SecretsManagerImpl::~SecretsManagerImpl() = default;

SecretsManagerImpl::SecretsManagerImpl(Application& app) : mApp(app)
{

}

std::string SecretsManagerImpl::getSecret(const std::string&) const
{
    return "TopSecretInfo";
}
}