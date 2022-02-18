#include "secrets/SecretsManagerImpl.h"

#include <memory>
#include <stdexcept>

#include <aws/core/Aws.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/secretsmanager/SecretsManagerClient.h>
#include <aws/secretsmanager/model/GetSecretValueRequest.h>
#include <aws/secretsmanager/model/GetSecretValueResult.h>
#include <fmt/format.h>

#include "util/Logging.h"

namespace digitalbits 
{
std::unique_ptr<SecretsManager>
SecretsManager::create()
{
    auto secretsManagerPtr = std::make_unique<SecretsManagerImpl>();
    return secretsManagerPtr;
}

SecretsManager::~SecretsManager() = default;

SecretsManagerImpl::SecretsManagerImpl() = default;
SecretsManagerImpl::~SecretsManagerImpl() = default;

std::string SecretsManagerImpl::getSecretById(const std::string& secretId) const
{
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;

    Aws::InitAPI(options);
    Aws::SecretsManager::SecretsManagerClient sm;
    Aws::SecretsManager::Model::GetSecretValueRequest request;

    request.SetSecretId(secretId);
    auto outcome = sm.GetSecretValue(request);

    if (outcome.IsSuccess())
    {
        CLOG_INFO(SecretsManager, "Secret retrieval successful, ARN {}",
            secretId);
        auto& result = outcome.GetResult();
        std::string output(result.GetSecretString());

        Aws::ShutdownAPI(options);

        return  output;
    }
    else
    {
        auto message = fmt::format("Secret retrieval failed {}, error {}",
            secretId, outcome.GetError());
        CLOG(INFO, "SecretsManager") << message;

        Aws::ShutdownAPI(options);
        
        throw std::runtime_error(message);
    }

    return "";
}
}