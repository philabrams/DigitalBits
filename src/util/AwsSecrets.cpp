#include <aws/core/Aws.h>
#include <aws/secretsmanager/SecretsManagerClient.h>
#include <aws/secretsmanager/model/GetSecretValueRequest.h>
#include <aws/secretsmanager/model/GetSecretValueResult.h>
#include <fmt/format.h>

#include "util/Logging.h"

namespace digitalbits
{
std::string getSecretById(const std::string& secretId)
{
    Aws::SecretsManager::SecretsManagerClient sm;
    Aws::SecretsManager::Model::GetSecretValueRequest request;
    Aws::String secretIdAws(secretId.c_str(), secretId.size());

    request.SetSecretId(secretIdAws);
    auto outcome = sm.GetSecretValue(request);

    if (outcome.IsSuccess())
    {
        LOG_INFO(DEFAULT_LOG, "Secret retrieval successful, ARN {}",
            secretId);
        auto& result = outcome.GetResult();
        auto& secretStringAws = result.GetSecretString(); 
        std::string output(secretStringAws.c_str(), secretStringAws.size());

        return  output;
    }
    else
    {
        auto message = fmt::format("Secret retrieval failed {}, error {}",
            secretId, outcome.GetError());
        LOG_INFO(DEFAULT_LOG, message);

        throw std::runtime_error(message);
    }

    return "";
}
}