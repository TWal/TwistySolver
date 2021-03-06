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

    int nbTest = 2;

    auto testLambda = [&](const std::vector<uint>& solution) {
        Cube cube2 = cube;
        for(uint i : solution) {
            for(uint j = 0; j <= i%3; ++j) {
                cube2.applyMult(Cube(3, (AXIS)(i/3), 1));
            }
        }
        EXPECT_ID(cube2);
        --nbTest;
        return nbTest != 0;
    };

    auto cleanupLambda = [&]() {
        nbTest = 2;
        cube = Cube();
    };

    for(int i = 0; i < 11; ++i) {
        cube.applyMult(Cube(3, R, 1));
        cube.applyMult(Cube(3, B, 1));
        cube.applyMult(Cube(3, L, 1));
        cube.applyMult(Cube(3, F, 1));
    }
    solver.solve(cube, testLambda);
    cleanupLambda();

    for(int i = 0; i < 11; ++i) {
        cube.applyMult(Cube(3, R, 1));
        cube.applyMult(Cube(3, B, 1));
        cube.applyMult(Cube(3, U, 1));
        cube.applyMult(Cube(3, L, 1));
        cube.applyMult(Cube(3, F, 1));
        cube.applyMult(Cube(3, D, 1));
    }
    solver.solve(cube, testLambda);
    cleanupLambda();

    solver.solve(cube, [](const std::vector<uint>& solution) {
        EXPECT_EQ(solution.size(), 0);
        return true;
    });
}
