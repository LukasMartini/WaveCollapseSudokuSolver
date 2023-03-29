//
// Created by Luke Martin on 2023-03-27.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "gtest/gtest.h"

struct Tile {
    int value = -1; // -1 is the default, unset value. I would've preferred to use a more descriptive string, but typecasting is for losers.
    std::vector<int> possibilities;
    const std::vector<std::vector<int>> adjacencies;
    bool isSet;
};
typedef std::vector<Tile> TileList;

class SudokuTable {
    public:
        SudokuTable();
        SudokuTable(std::map<int, int> &presetValues);
        SudokuTable(std::ifstream &file);

    private:
        bool verifyTL(TileList &t) const;
        TileList populateNewTileList();
        TileList populateNewTileList(std::map<int, int> &presetValues); // Overloaded version that handles presets
        std::vector<int> findBoxIndexes(int &index) const;
        std::vector<int> findRowIndexes(int &index) const;
        std::vector<int> findColIndexes(int &index) const;

        TileList table;

    // ---------- FRIEND TEST SECTION ---------- //
        // ----- defaultCtorSuite ----- //
        FRIEND_TEST(defaultCtorSuite, givenNoInputEnsureValidTable);

        // ----- verifyTLSuite ----- //
        FRIEND_TEST(verifyTLSuite, flagSetWhileValueStillDefaultExpectDeath);
        FRIEND_TEST(verifyTLSuite, missingFlagSetAfterChangeToTableExpectDeath);
        FRIEND_TEST(verifyTLSuite, valueOutsideOfExpectedRangeExpectDeath);

        // ----- populateNewTileListOneArgOverloadedSuite ----- //
        FRIEND_TEST(populateNewTileListOneArgOverloadedSuite, supplyValidPresetsExpectCorrectValues);
        FRIEND_TEST(populateNewTileListOneArgOverloadedSuite, supplyInvalidPositiveKeysExpectFailure);
        FRIEND_TEST(populateNewTileListOneArgOverloadedSuite, supplyInvalidPositiveValuesExpectFailure);
        FRIEND_TEST(populateNewTileListOneArgOverloadedSuite, supplyInvalidNegativeKeysExpectFailure);
        FRIEND_TEST(populateNewTileListOneArgOverloadedSuite, supplyInvalidNegativeValuesExpectFailure);

        // ----- findBoxIndexesSuite ----- //
        FRIEND_TEST(findBoxIndexesSuite, givenBaseTableEnsureCorrectBoxIndexes);

        // ----- findRowIndexesSuite ----- //
        FRIEND_TEST(findRowIndexesSuite, givenBaseTableEnsureCorrectRowIndexes);

        // ----- findColIndexesSuite ----- //
        FRIEND_TEST(findColIndexesSuite, givenBaseTableEnsureCorrectColIndexes);
};