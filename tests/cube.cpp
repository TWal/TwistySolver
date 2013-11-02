#include <gtest/gtest.h>
#include "Cube.h"

#define EXPECT_ID_CP(cube) \
    for(int i = 0; i < 8; ++i) {\
        EXPECT_EQ(cube.getCornerPerm()[i], i);\
    }

#define EXPECT_ID_CO(cube) \
    for(int i = 0; i < 8; ++i) {\
        EXPECT_EQ(cube.getCornerOri()[i], 0);\
    }

#define EXPECT_ID_EP(cube) \
    for(int i = 0; i < 12; ++i) {\
        EXPECT_EQ(cube.getEdgePerm()[i], i);\
    }

#define EXPECT_ID_EO(cube) \
    for(int i = 0; i < 12; ++i) {\
        EXPECT_EQ(cube.getEdgeOri()[i], 0);\
    }

#define EXPECT_ID(cube) \
    EXPECT_ID_CP(cube); \
    EXPECT_ID_CO(cube); \
    EXPECT_ID_EP(cube); \
    EXPECT_ID_EO(cube);

TEST(CubeTest, Identity) {
    Cube cube;
    EXPECT_ID(cube);
}

TEST(CubeTest, SexyMult) {
    Cube cube;
    Cube cubeR(R);
    Cube cubeU(U);

    for(int i = 0; i < 6; ++i) {
        cube.applyMult(cubeR);
        cube.applyMult(cubeU);
        cube.applyMult(cubeR);
        cube.applyMult(cubeR);
        cube.applyMult(cubeR);
        cube.applyMult(cubeU);
        cube.applyMult(cubeU);
        cube.applyMult(cubeU);
        if(i%2 == 1) {
            EXPECT_ID_CP(cube);
        }
        if(i == 2) {
            EXPECT_ID_EP(cube);
            EXPECT_ID_EO(cube);
        }
    }

    EXPECT_ID(cube);
}
