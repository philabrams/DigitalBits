// Copyright 2020 DigitalBits Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "transactions/ManageBuyOfferOpFrame.h"

namespace digitalbits
{
namespace txsimulation
{

class TxSimManageBuyOfferOpFrame : public ManageBuyOfferOpFrame
{
    OperationResult mSimulationResult;
    uint32_t mCount;

  public:
    TxSimManageBuyOfferOpFrame(Operation const& op, OperationResult& res,
                               TransactionFrame& parentTx,
                               OperationResult const& simulationResult,
                               uint32_t partition);
    int64_t generateNewOfferID(LedgerTxnHeader& header) override;
};
}
}
