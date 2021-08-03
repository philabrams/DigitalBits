# Issue description (set as title of the issue)

*example: crash when trying to create an account*

# What I was trying to do

*example: I tried to submit a transaction containing `createAccount` and then digitalbits-core crashed*

*include actual example that causes the problem, if applicable (json or base64 encoded transaction for example)*

# Expected result
*example: it should not crash!*

# Information

## Version
*output of `info` or tag/release you downloaded if you can't get info to work*
example:
```json

   "info" : {
      "build" : "1.0.5-1-g7f8dc82",
      "history_failure_rate" : "0.0",
      "ledger" : {
         "age" : 4,
         "baseFee" : 100,
         "baseReserve" : 100000000,
         "closeTime" : 1627382327,
         "hash" : "3641c406ccd7a03d26d966a09a5ffcdc77344fe3c581d77f7c0ca079e69de48d",
         "maxTxSetSize" : 100,
         "num" : 1709699,
         "version" : 15
      },
      "network" : "LiveNet Global DigitalBits Network ; February 2021",
      "peers" : {
         "authenticated_count" : 14,
         "pending_count" : 0
      },
      "protocol_version" : 15,
      "quorum" : {
         "node" : "self",
         "qset" : {
            "agree" : 12,
            "cost" : 68552,
            "delayed" : 0,
            "disagree" : 0,
            "fail_at" : 3,
            "hash" : "bd50f0",
            "lag_ms" : 122,
            "ledger" : 1709698,
            "missing" : 0,
            "phase" : "EXTERNALIZE",
            "validated" : true
         },
         "transitive" : {
            "critical" : null,
            "intersection" : true,
            "last_check_ledger" : 1428350,
            "node_count" : 12
         }
      },
      "startedOn" : "2021-07-10T06:25:36Z",
      "state" : "Synced!"
   }
}
```

## Configuration

*paste your configuration here*

**IMPORTANT: replace your PEER_SEED or any other information that you do not want to share with XXXXXXX**

## OS information

*example: Linux Ubuntu 14.04.2 LTS*

## Logs around the time of the issue

*log location is normally defined in your log file something like `digitalbits-core.log`*

If you can reproduce the issue locally, it's even better - gather the logs in debug (more verbose) by setting the log level in your configuration file with:

```
COMMANDS=[
"ll?level=debug"
]
```

*example*
```
2015-11-12T18:00:15.753 GDKXE [Herder DEBUG] emitEnvelope s:2 i:694683 a:Synced!
2015-11-12T18:00:15.753 GDKXE [Herder DEBUG] broadcast  s:2 i:694683
2015-11-12T18:00:15.753 GDKXE [Herder DEBUG] HerderImpl::valueExternalized txSet: 2d7b96
2015-11-12T18:00:15.754 GDKXE [Herder INFO ] Quorum information for 694681 : {"agree":3,"disagree":0,"fail_at":2,"hash":"273af2","missing":0,"phase":"EXTERNALIZE"}

2015-11-12T18:00:15.754 GDKXE [Ledger INFO ] Got consensus: [seq=694683, prev=41900d, tx_count=0, sv: [  txH: 2d7b96, ct: 1447351216, upgrades: [ ] ]]
2015-11-12T18:00:15.754 GDKXE [Ledger DEBUG] starting closeLedger() on ledgerSeq=694683
2015-11-12T18:00:15.754 GDKXE [Ledger DEBUG] processing fees and sequence numbers
2015-11-12T18:00:15.755 GDKXE [Tx DEBUG] applyTransactions: ledger = 694683
2015-11-12T18:00:15.755 GDKXE [Bucket DEBUG] Deleting empty bucket file /data/tmp/bucket-5da00e572caabfa7/tmp-bucket-2d606ce437f41561.xdr
2015-11-12T18:00:15.755 GDKXE [Bucket DEBUG] Deleting empty bucket file /data/tmp/bucket-5da00e572caabfa7/tmp-bucket-5e6ef0a5fe4baefc.xdr
2015-11-12T18:00:15.755 GDKXE [Bucket DEBUG] Deleting empty bucket file /data/tmp/bucket-5da00e572caabfa7/tmp-bucket-fd6e8d0389dfa982.xdr
2015-11-12T18:00:15.755 GDKXE [Bucket DEBUG] Deleting empty bucket file /data/tmp/bucket-5da00e572caabfa7/tmp-bucket-fd1a7350fa32a5fc.xdr
2015-11-12T18:00:15.757 GDKXE [Ledger DEBUG] Advancing LCL: [seq=694682, hash=41900d] -> [seq=694683, hash=88dc48]
2015-11-12T18:00:15.757 GDKXE [Ledger DEBUG] New current ledger: seq=694684
```

## Stack trace (if applicable)

If it's a crash, if you can paste the stack trace here. Stack traces may be obtained by attaching a debugger before the crash, or by opening core dumps (be sure to set `ulimit -c unlimited` to generate core dumps).
