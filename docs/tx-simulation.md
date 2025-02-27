---
title: Ledger state and transaction simulation in digitalbits-core
---

Minimum supported ledger version: 12

digitalbits-core has built-in functionality to scale any existing ledger state by a specific multiplier,
and simulate transaction stream application. This is primarily used to analyze performance of the transaction subsystem.

There are a few steps involved in preparing ledger state for transaction simulation. Typically, the sequence of commands is as follows:

### Simulating bucketlist
Use `simulate-bucketlist` command to scale the bucketlist by MULTIPLIER and prepare the desired ledger state at ledger L.

`digitalbits-core simulate-bucketlist <L> --multiplier <MULTIPLIER>`

* the configuration must include up-to-date readable history archives.
* because bucket files are only available per checkpoint, ledger L will be rounded up to the nearest checkpoint. This means that upon the completion, the LCL might be set to a ledger different from L. For instance, running `simulate-bucketlist 27000000` will produce LCL of 27000063.
* It is convenient to generate the necessary buckets once, and re-use them as needed. Simulated buckets are stored in `buckets` directory, or whatever `BUCKET_DIR_PATH` is configured to. To avoid re-generating buckets and skip directly to application, use `--history-archive-state` flag and use the HAS file `simulate-digitalbits-history.json` generated by the previous bucketlist simulation run.

### Generating simulated transactions
Generate transactions with `generate-transactions`

`digitalbits-core generate-transactions --first-ledger-inclusive <F> --last-ledger-inclusive <L> --multiplier <MULTIPLIER>`

* the configuration must include writable `simulate` archive to publish simulated transactions.
* ledger range provided via `first-ledger-inclusive` and `last-ledger-inclusive` must be consistent with the LCL generated by `simulate-bucketlist`. For example, if LCL is 27000063, then `first-ledger-inclusive` must be 27000064.

This command will generate scaled transactions and publish them to a special `simulate` archive. Because generating signatures for simulated transactions is computationally expensive, it is convenient to generate all the desired transactions first.

### Simulating transactions
Finally, run simulated transaction replay with `simulate-transactions`

`digitalbits-core simulate-transactions --first-ledger-inclusive <F> --last-ledger-inclusive <L>`

* the configuration must include readable `simulate` archive used to publish generated transactions.
* ledger range provided via `first-ledger-inclusive` and `last-ledger-inclusive` must be consistent with the checkpoint range published to `simulate` archive.

#### Replay options
There are two ways to replay generated transactions 

##### Sustained load
Use `ops-per-ledger` flag when running the command to provide desired number of operations to apply per ledger. This allows to control the load when replaying simulated transactions.

##### Mimic network traffic
Without `ops-per-ledger` flag specified, number of operations per ledger will match scaled public network traffic.

#### Application order 
Transactions are generated and applied such that given a multiplier M, a stream tx_1, tx_2, ..., tx_K is converted to
tx_1_1, tx_1_2, ... tx_1_M, tx_2_1, tx_2_2, ..., tx_2_M, ..., tx_K_1, tx_K_2, ..., tx_K_M

Currently, there are no other application strategies implemented, but it is possible that they will be available in the future. 

