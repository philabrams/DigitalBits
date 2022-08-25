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

        auto& secretStringAws = outcome.GetResult().GetSecretString(); 
        std::string output(secretStringAws.c_str(), secretStringAws.size());

        return  output;
    }
    else
    {
        auto& errorMsgAws = outcome.GetError().GetMessage();
        std::string errorMsg(errorMsgAws.c_str(), errorMsgAws.size());

        LOG_INFO(DEFAULT_LOG, "Secret retrieval failed, ARN {}, error {}",
            secretId, errorMsg);

        throw std::runtime_error(fmt::format(
            "Secret retrieval failed, ARN {}", secretId));
    }

    return "";
}
}