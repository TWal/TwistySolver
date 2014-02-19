#include <gtest/gtest.h>
#include "OrientationCoordinate.h"
#include "PermutationCoordinate.h"
#include "PositionCoordinate.h"

#define EXPECT_SAME_ARRAY(array1, array2, nb) \
    for(int i = 0; i < nb; ++i) { \
        EXPECT_EQ(array1[i], array2[i]); \
    }

static const std::vector<uint> allowedMoves = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

TEST(Coordinate, OrientationFunc) {
    char arrtmp[8];
    unsigned int n;

    char arr1[8] = {0, 2, 1, 2, 1, 0, 2, 1};
    n = OrientationCoordinate::orientToInt(arr1, 8, 3);
    OrientationCoordinate::intToOrient(n, arrtmp, 8, 3);
    EXPECT_SAME_ARRAY(arr1, arrtmp, 8);

    char arr2[8] = {2, 2, 0, 2, 0, 2, 2, 2};
    n = OrientationCoordinate::orientToInt(arr2, 8, 3);
    OrientationCoordinate::intToOrient(n, arrtmp, 8, 3);
    EXPECT_SAME_ARRAY(arr2, arrtmp, 8);

    char arrid[8] = {0};
    EXPECT_EQ(OrientationCoordinate::orientToInt(arrid, 8, 3), 0);
    EXPECT_EQ(OrientationCoordinate::orientToInt(arrid, 0, 0), 0);

}

TEST(Coordinate, PermutationFunc) {
    char arrtmp[8];
    unsigned int n;

    char arr1[8] = {5, 3, 7, 1, 2, 4, 6, 0};
    n = PermutationCoordinate::permToInt(arr1, 8);
    PermutationCoordinate::intToPerm(n, arrtmp, 8);
    EXPECT_SAME_ARRAY(arr1, arrtmp, 8);

    char arr2[8] = {6, 4, 2, 7, 1, 0, 3, 5};
    n = PermutationCoordinate::permToInt(arr2, 8);
    PermutationCoordinate::intToPerm(n, arrtmp, 8);
    EXPECT_SAME_ARRAY(arr2, arrtmp, 8);

    char arrid[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(PermutationCoordinate::permToInt(arrid, 8), 0);
    EXPECT_EQ(PermutationCoordinate::permToInt(arrid, 0), 0);
}

TEST(Coordinate, PositionFunc) {
    char arrtmp[8];
    unsigned int n;

    char arr1[8] = {5, 3, 7, 1, 2, 4, 6, 0};
    n = PositionCoordinate::posToInt(arr1, 8, {4, 5, 6, 7});
    PositionCoordinate::intToPos(n, arrtmp, 8, {4, 5, 6, 7});
    for(int i = 0; i < 8; ++i) {
        EXPECT_TRUE(arrtmp[i] == -1 || (arrtmp[i] < 8 && arrtmp[i] >= 4));
    }

    char arr2[8] = {6, 4, 2, 7, 1, 0, 3, 5};
    n = PositionCoordinate::posToInt(arr2, 8, {4, 5, 6, 7});
    PositionCoordinate::intToPos(n, arrtmp, 8, {4, 5, 6, 7});
    for(int i = 0; i < 8; ++i) {
        EXPECT_TRUE(arrtmp[i] == -1 || (arrtmp[i] < 8 && arrtmp[i] >= 4));
    }
}

TEST(Coordinate, OrientationMoveTable) {
    OrientationCoordinate oc(8, 3, [](const char* orient) {
        char nothing[12] = {0};
        return Cube(3, nothing, nothing, orient, nothing);
    }, [](const Cube& cube, char* orient) {
        memcpy(orient, cube.getCornerOri(), 8);
    });
    oc.buildMoveTable(allowedMoves, CubeProperties(3));

    OrientationCoordinate oc2(12, 2, [](const char* orient) {
        char nothing[12] = {0};
        return Cube(3, nothing, nothing, nothing, orient);
    }, [](const Cube& cube, char* orient) {
        memcpy(orient, cube.getEdgeOri(), 12);
    });
    oc2.buildMoveTable(allowedMoves, CubeProperties(3));

    uint c = 0;
    uint c2 = 0;
    for(int i = 0; i < 6; ++i) {
        c = oc.moveTableLookup(c, R, 1);
        c = oc.moveTableLookup(c, U, 1);
        c = oc.moveTableLookup(c, R, 3);
        c = oc.moveTableLookup(c, U, 3);
        c2 = oc2.moveTableLookup(c2, R, 1);
        c2 = oc2.moveTableLookup(c2, U, 1);
        c2 = oc2.moveTableLookup(c2, R, 3);
        c2 = oc2.moveTableLookup(c2, U, 3);

        EXPECT_EQ(c2, 0);

        if(i == 2) {
            Cube cube = oc.toCube(c);
            int sum = 0;
            for(int j = 0; j < 8; ++j) {
                sum += cube.getCornerOri()[j];
            }
            EXPECT_EQ(sum, 3);
        }
    }
    EXPECT_EQ(c, 0);
}

TEST(Coordinate, PermutationMoveTable) {
    PermutationCoordinate pc(8, [](const char* perm) {
        char nothing[12] = {0};
        return Cube(3, perm, nothing, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getCornerPerm(), 8);
    });

    pc.buildMoveTable(allowedMoves, CubeProperties(3));

    uint c = 0;
    for(int i = 0; i < 6; ++i) {
        c = pc.moveTableLookup(c, R, 1);
        c = pc.moveTableLookup(c, U, 1);
        c = pc.moveTableLookup(c, R, 3);
        c = pc.moveTableLookup(c, U, 3);
        if(i%2 == 1) {
            EXPECT_EQ(c, 0);
        }
    }
}

TEST(Coordinate, PositionMoveTable) {
    PositionCoordinate poc(12, {FL, FR, BR, BL}, [](const char* perm) {
        char nothing[12] = {0};
        return Cube(3, nothing, perm, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getEdgePerm(), 12);
    });

    poc.buildMoveTable(allowedMoves, CubeProperties(3));

    Cube cube;
    uint id = poc.fromCube(cube);
    uint c = id;

    c = poc.moveTableLookup(c, R, 1);
    c = poc.moveTableLookup(c, U, 1);
    c = poc.moveTableLookup(c, R, 3);

    c = poc.moveTableLookup(c, L, 3);
    c = poc.moveTableLookup(c, U, 1);
    c = poc.moveTableLookup(c, L, 1);

    c = poc.moveTableLookup(c, R, 3);
    c = poc.moveTableLookup(c, U, 1);
    c = poc.moveTableLookup(c, R, 1);

    c = poc.moveTableLookup(c, L, 1);
    c = poc.moveTableLookup(c, U, 1);
    c = poc.moveTableLookup(c, L, 3);

    c = poc.moveTableLookup(c, R, 1);
    c = poc.moveTableLookup(c, U, 1);
    c = poc.moveTableLookup(c, R, 3);

    EXPECT_EQ(c, id);
}

