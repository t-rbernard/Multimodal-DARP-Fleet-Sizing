//
// Created by romain on 15/02/24.
//

//TODO : Implement following tests
//  - verify schedule continuity (forall schedule, schedule[0] < schedule[1]
//  -

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"


TEST(LineTests, LineGenerationScheduleOrder) {
    std::string instanceFolder = "contiguous_lines_debug_instance/";
    std::string datFile = "graph.dat";
    Graph graphFromSingleFile("../resources/test/instances/" + instanceFolder + datFile);

    for(const auto& line : graphFromSingleFile.getPTLines()) {
        for(const auto& schedule : line.getTimetables()) {
            for(size_t i = 0; i < line.scheduleSize(); ++i) {
                ASSERT_GT(schedule.at(i), schedule.at(i + 1)); //assert schedule value order
            }
        }
        ASSERT_TRUE(line.checkSchedules()); //assert line schedule check function is coherent with our preceding assertion
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}