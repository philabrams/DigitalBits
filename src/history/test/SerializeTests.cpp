// Copyright 2018 DigitalBits Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "history/HistoryArchive.h"
#include "lib/catch.hpp"

#include <filesystem>
#include <fstream>
#include <string>

using namespace digitalbits;
namespace fs = std::filesystem;

TEST_CASE("Serialization round trip", "[history]")
{
    std::vector<std::string> testFiles = {
        "digitalbits-history.testnet.6714239.json",
        "digitalbits-history.livenet.15686975.json",
        "digitalbits-history.testnet.6714239.networkPassphrase.json"};
    for (int i = 0; i < testFiles.size(); i++)
    {
        fs::path cwd = fs::current_path();

        std::string fnPath = "";

        if (cwd.filename() != "src") {
            fnPath = "src/testdata/";
        } else {
            fnPath = "testdata/";
        }

        std::string testFilePath = fnPath + testFiles[i];
        SECTION("Serialize " + testFilePath)
        {
            std::ifstream in(testFilePath);
            REQUIRE(in);
            in.exceptions(std::ios::badbit);
            std::string hasString((std::istreambuf_iterator<char>(in)),
                                  std::istreambuf_iterator<char>());

            // Test fromString
            HistoryArchiveState has;
            has.fromString(hasString);
            REQUIRE(hasString == has.toString());

            // Test load
            HistoryArchiveState hasLoad;
            hasLoad.load(testFilePath);
            REQUIRE(hasString == hasLoad.toString());
        }
    }
}
