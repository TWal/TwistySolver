#include <gtest/gtest.h>
#include "Solver333.h"

#define EXPECT_ID(cube) \
    for(int i = 0; i < 8; ++i) { \
        EXPECT_EQ(cube.getCornerPerm()[i], i); \
        EXPECT_EQ(cube.getCornerOri()[i], 0); \
    } \
    for(int i = 0; i < 12; ++i) { \
        EXPECT_EQ(cube.getEdgePerm()[i], i); \
        EXPECT_EQ(cube.getEdgeOri()[i], 0); \
    }

TEST(Solver, Solve) {
    Solver333 solver;
    Cube cube;
    std::vector<uint> solution;

    for(int i = 0; i < 11; ++i) {
        cube.applyMult(Cube(R));
        cube.applyMult(Cube(B));
        cube.applyMult(Cube(L));
        cube.applyMult(Cube(F));
    }
    solution = solver.solve(cube);
    for(uint i : solution) {
        for(uint j = 0; j <= i%3; ++j) {
            cube.applyMult(Cube((AXIS)(i/3)));
        }
    }
    EXPECT_ID(cube);

    for(int i = 0; i < 11; ++i) {
        cube.applyMult(Cube(R));
        cube.applyMult(Cube(B));
        cube.applyMult(Cube(U));
        cube.applyMult(Cube(L));
        cube.applyMult(Cube(F));
        cube.applyMult(Cube(D));
    }
    solution = solver.solve(cube);
    for(uint i : solution) {
        for(uint j = 0; j <= i%3; ++j) {
            cube.applyMult(Cube((AXIS)(i/3)));
        }
    }
    EXPECT_ID(cube);

    solution = solver.solve(cube);
    EXPECT_EQ(solution.size(), 0);
}
