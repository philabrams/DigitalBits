---
title: Overview
---

DigitalBits is a decentralized, federated peer-to-peer network that allows people to
send payments in any asset anywhere in the world instantaneously, and with
minimal fee.

`DigitalBits-core` is the core component of this network. `DigitalBits-core` is a C++
implementation of the DigitalBits Consensus Protocol configured to construct a chain
of ledgers that are guaranteed to be in agreement across all the participating
nodes at all times.

For more detail on the DigitalBits Consensus Protocol and how it establishes this
guarantee see [`src/scp/readme.md`](https://github.com/xdbfoundation/DigitalBits/blob/master/src/scp/readme.md).


- [Building & Installing](https://github.com/xdbfoundation/DigitalBits/blob/master/README.md)
- [DigitalBits-core administration](software/admin.md)
- [Architecture](https://github.com/xdbfoundation/DigitalBits/blob/master/docs/architecture.md)
- [Key Concepts](https://github.com/xdbfoundation/docs/blob/master/guides/readme.md)
- [Integration with other services](https://github.com/xdbfoundation/DigitalBits/blob/master/docs/integration.md)
- [Major Components](#major-components)
- [Supporting Code Directories](#supporting-code-directories)



## Major Components

There are a few major components of the system. Each component has a dedicated
source directory and its own dedicated `readme.md`.


* **DCP** is our implementation of the DigitalBits Consensus Protocol (DCP). This
  component is fully abstracted from the rest of the system. It receives
  candidate black-box values and signals when these values have reached
  consensus by the network (called _externalizing_ a value) (See
  [`src/scp/readme.md`](https://github.com/xdbfoundation/DigitalBits/blob/master/src/scp/readme.md)).

* **Herder** is responsible for interfacing between DCP and the rest of
  `digitalbits-core`. Herder provides DCP with concrete implementations of the
  methods DCP uses to communicate with peers, to compare values, to determine
  whether values contain valid signatures, and so forth. Herder often
  accomplishes its tasks by delegating to other components
  (See [`src/herder/readme.md`](https://github.com/xdbfoundation/DigitalBits/blob/master/src/herder/readme.md)).

* **Overlay** connects to and keeps track of the peers this node knows
  about and is connected to. It floods messages and fetches from peers the data
  that is needed to accomplish consensus (See
  [`src/overlay/readme.md`](https://github.com/xdbfoundation/DigitalBits/blob/master/src/overlay/readme.md)). All
  other data downloads are handled without imposing on the DCP-nodes, see
  [`./architecture.md`](https://github.com/xdbfoundation/DigitalBits/blob/master/docs/architecture.md).

* **Ledger** applies the transaction set that is externalized by DCP. It also
  forwards the externalization event to other components: it submits the changed
  ledger entries to the bucket list, triggers the publishing of history, and
  informs the overlay system to update its map of flooded messages. Ledger also
  triggers the history system's catching-up routine when it detects that this
  node has fallen behind of the rest of the network (See
  [`src/ledger/readme.md`](https://github.com/xdbfoundation/DigitalBits/blob/master/src/ledger/readme.md)).

* **History** publishes transaction and ledger entries to off-site permanent
  storage for auditing, and as a source of catch-up data for other nodes. When
  this node falls behind, the history system fetches catch-up data and submits
  it to Ledger twice: first to verify its security, then to apply it (See
  [`src/history/readme.md`](https://github.com/xdbfoundation/DigitalBits/blob/master/src/history/readme.md)).

* **BucketList** stores ledger entries on disk arranged for hashing and
  block-catch-up. BucketList coordinates the hashing and deduplicating of
  buckets by multiple background threads
  (See [`src/bucket/readme.md`](https://github.com/xdbfoundation/DigitalBits/blob/master/src/bucket/readme.md)).

* **Transactions** implements all the various transaction types (See
  [src/transactions/readme.md](https://github.com/xdbfoundation/DigitalBits/blob/master/src/transactions/readme.md)).


## Supporting Code Directories

* **src/main** handles booting, loading of the configuration and of persistent
  state flags. Launches the test suite if requested.

* **src/crypto** contains standard cryptographic routines, including random
  number generation, hashing, hex encoding and DigitalBits Key encoding.

* **src/util** gathers assorted logging and utility routines.

* **src/lib** keeps various 3rd party libraries we use.

* **src/database** is a thin layer above the functionality provided by the
  database-access library `soci`.

* **src/process** is an asynchronous implementation of `system()`, for running
  subprocesses.

* **src/simulation** provides support for instantiating and exercising
  in-process test networks.

* **src/xdr** contains to definition of the wire protocol in the [`XDR`
    (RFC4506)](https://tools.ietf.org/html/rfc4506.html) specification language.

* **src/generated** contains the wire protocol's C++ classes, generated from
  the definitions in `src/xdr`.
