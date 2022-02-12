#include "secrets/SecretsManagerImpl.h"

#include <aws/core/Aws.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/secretsmanager/SecretsManagerClient.h>
#include <aws/secretsmanager/model/GetSecretValueRequest.h>
#include <aws/secretsmanager/model/GetSecretValueResult.h>

#include "util/Logging.h"

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
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
    // jnitialize AWS API
    Aws::InitAPI(options);
    auto secretId = "tutorial/MyFirstSecret";
    Aws::SecretsManager::SecretsManagerClient sm;
    Aws::SecretsManager::Model::GetSecretValueRequest request;

    request.SetSecretId(secretId);
    auto outcome = sm.GetSecretValue(request);
    if (outcome.IsSuccess())
    {
        CLOG_INFO(Ledger, "Secret retrieval successful.");
        auto& result = outcome.GetResult();
        CLOG_INFO(Ledger, "Secret name {}", result.GetName().c_str());
        CLOG_INFO(Ledger, "Secret value {}", result.GetSecretString().c_str());
    }
    else
    {
        CLOG_INFO(Ledger, "Secret retrieval failed {}", outcome.GetError());
    }

    Aws::ShutdownAPI(options);

    return "";
}
}