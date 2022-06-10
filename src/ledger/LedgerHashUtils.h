#pragma once

// Copyright 2018 DigitalBits Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "crypto/ShortHash.h"
#include "ledger/InternalLedgerEntry.h"
#include "util/HashOfHash.h"
#include "xdr/DigitalBits-ledger.h"
#include <functional>

namespace digitalbits
{

static PoolID const&
getLiquidityPoolID(Asset const& asset)
{
    throw std::runtime_error("cannot get PoolID from Asset");
}

static PoolID const&
getLiquidityPoolID(TrustLineAsset const& tlAsset)
{
    return tlAsset.liquidityPoolID();
}

static inline void
hashMix(size_t& h, size_t v)
{
    // from https://github.com/ztanml/fast-hash (MIT license)
    v ^= v >> 23;
    v *= 0x2127599bf4325c37ULL;
    v ^= v >> 47;
    h ^= v;
    h *= 0x880355f21e6d1965ULL;
}

template <typename T>
static size_t
getAssetHash(T const& asset)
{
    size_t res = asset.type();

    switch (asset.type())
    {
    case digitalbits::ASSET_TYPE_NATIVE:
        break;
    case digitalbits::ASSET_TYPE_CREDIT_ALPHANUM4:
    {
        auto& a4 = asset.alphaNum4();
        hashMix(res, std::hash<digitalbits::uint256>()(a4.issuer.ed25519()));
        hashMix(res, digitalbits::shortHash::computeHash(digitalbits::ByteSlice(
                         a4.assetCode.data(), a4.assetCode.size())));
        break;
    }
    case digitalbits::ASSET_TYPE_CREDIT_ALPHANUM12:
    {
        auto& a12 = asset.alphaNum12();
        hashMix(res, std::hash<digitalbits::uint256>()(a12.issuer.ed25519()));
        hashMix(res, digitalbits::shortHash::computeHash(digitalbits::ByteSlice(
                         a12.assetCode.data(), a12.assetCode.size())));
        break;
    }
    case digitalbits::ASSET_TYPE_POOL_SHARE:
    {
        hashMix(res, std::hash<digitalbits::uint256>()(getLiquidityPoolID(asset)));
        break;
    }
    default:
        throw std::runtime_error("unknown Asset type");
    }
    return res;
}

}

// implements a default hasher for "LedgerKey"
namespace std
{
template <> class hash<digitalbits::Asset>
{
  public:
    size_t
    operator()(digitalbits::Asset const& asset) const
    {
        return digitalbits::getAssetHash<digitalbits::Asset>(asset);
    }
};

template <> class hash<digitalbits::TrustLineAsset>
{
  public:
    size_t
    operator()(digitalbits::TrustLineAsset const& asset) const
    {
        return digitalbits::getAssetHash<digitalbits::TrustLineAsset>(asset);
    }
};

template <> class hash<digitalbits::LedgerKey>
{
  public:
    size_t
    operator()(digitalbits::LedgerKey const& lk) const
    {
        size_t res = lk.type();
        switch (lk.type())
        {
        case digitalbits::ACCOUNT:
            digitalbits::hashMix(res, std::hash<digitalbits::uint256>()(
                                      lk.account().accountID.ed25519()));
            break;
        case digitalbits::TRUSTLINE:
        {
            auto& tl = lk.trustLine();
            digitalbits::hashMix(
                res, std::hash<digitalbits::uint256>()(tl.accountID.ed25519()));
            digitalbits::hashMix(res, hash<digitalbits::TrustLineAsset>()(tl.asset));
            break;
        }
        case digitalbits::DATA:
            digitalbits::hashMix(res, std::hash<digitalbits::uint256>()(
                                      lk.data().accountID.ed25519()));
            digitalbits::hashMix(
                res,
                digitalbits::shortHash::computeHash(digitalbits::ByteSlice(
                    lk.data().dataName.data(), lk.data().dataName.size())));
            break;
        case digitalbits::OFFER:
            digitalbits::hashMix(
                res, digitalbits::shortHash::computeHash(digitalbits::ByteSlice(
                         &lk.offer().offerID, sizeof(lk.offer().offerID))));
            break;
        case digitalbits::CLAIMABLE_BALANCE:
            digitalbits::hashMix(res, std::hash<digitalbits::uint256>()(
                                      lk.claimableBalance().balanceID.v0()));
            break;
        case digitalbits::LIQUIDITY_POOL:
            digitalbits::hashMix(res, std::hash<digitalbits::uint256>()(
                                      lk.liquidityPool().liquidityPoolID));
            break;
        default:
            abort();
        }
        return res;
    }
};

template <> class hash<digitalbits::InternalLedgerKey>
{
  public:
    size_t
    operator()(digitalbits::InternalLedgerKey const& glk) const
    {
        return glk.hash();
    }
};
}
