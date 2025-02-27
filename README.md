# DigitalBits-core

DigitalBits-core is a replicated state machine that in consensus with a set of peers maintains a local copy of a cryptographic ledger and processes transactions against it. DigitalBits-core utilizes the DigitalBits Consensus Protocol, a fork of the [Stellar Consensus Protocol]( https://www.stellar.org/papers/stellar-consensus-protocol), as its federated consensus protocol.

DigitalBits-core is written in C++11 and runs on Linux, OSX and Windows.  
Learn more by reading the [overview document]( https://github.com/xdbfoundation/DigitalBits/blob/master/docs/readme.md).

# Documentation

Documentation of the code's layout and abstractions, as well as for the
functionality available, can be found in
[`./docs`]( https://github.com/xdbfoundation/DigitalBits/tree/master/docs).

# Installation

See [Installation](./INSTALL.md)

# Contributing

See [Contributing](./CONTRIBUTING.md)

# Reporting issues

Software has bugs, or maybe you have an idea for a change in digitalbits-core.

Checklist
 1. do a search of issues in case there is one already tracking the one you ran into.
 2. search open issues (not addressed yet) using the filter `is:open` (default). If you have new information, include it into the issue.
 3. search closed issues by removing the `is:open` filter. Two possibilities here:
     * the issue was resolved in a newer version - then you just need to install the version with the fix
     * the issue was closed for some reason. You may decide to reopen it depending on context. Make sure to explain why the issue should be re-opened.

For bugs being opened/re-opened, simply paste and fill the [Bug-Template.md](./Bug-Template.md) into the issue.

# Running tests

run tests with:
  `src/digitalbits-core --test`

run one test with:
  `src/digitalbits-core --test  testName`

run one test category with:
  `src/digitalbits-core --test '[categoryName]'`

Categories (or tags) can be combined: AND-ed (by juxtaposition) or OR-ed (by comma-listing).

Tests tagged as [.] or [hide] are not part of the default test test.

supported test options can be seen with
  `src/digitalbits-core --test --help`

display tests timing information:
  `src/digitalbits-core --test -d yes '[categoryName]'`

xml test output (includes nested section information):
  `src/digitalbits-core --test -r xml '[categoryName]'`

# Running tests against postgreSQL

There are two options.  The easiest is to have the test suite just
create a temporary postgreSQL database cluster in /tmp and delete it
after the test.  That will happen by default if you run `make check`.

You can also create a temporary database cluster manually, by running
`./src/test/selftest-pg bash` to get a shell, then running tests
manually.  The advantage of this is that you can examine the database
log in `$PGDATA/pg_log/` after running tests, as well as manually
inspect the database with `psql`.

Finally, you can use an existing database cluster so long as it has
databases named `test0`, `test1`, ..., `test9`, and `test`.  Do set
this up, make sure your `PGHOST` and `PGUSER` environment variables
are appropriately set, then run the following from bash:

    for i in $(seq 0 9) ''; do
        psql -c "create database test$i;"
    done

# Running stress tests
We adopt the convention of tagging a stress-test for subsystem foo as [foo-stress][stress][hide].

Then, running
* `digitalbits-core --test [stress]` will run all the stress tests,
* `digitalbits-core --test [foo-stress]` will run the stress tests for subsystem foo alone, and
* neither `digitalbits-core --test` nor `digitalbits-core --test [foo]` will run stress tests.
