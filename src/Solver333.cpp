#include "Solver333.h"
#include <cstring>
#include "Utils.h"
#include <algorithm>

Solver333::Solver333() :
    _eo(12, 2, [](const char* orient) {
        char nothing[12] = {0};
        return Cube(nothing, nothing, nothing, orient);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getEdgeOri(), 12);
    }),
    _co(8, 3, [](const char* orient) {
        char nothing[12] = {0};
        return Cube(nothing, nothing, orient, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getCornerOri(), 8);
    }),
    _udslice(12, {FL, FR, BR, BL}, [](const char* perm) {
        char nothing[12] = {0};
        return Cube(nothing, perm, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getEdgePerm(), 12);
    }),
    _ep(8, [](const char* perm) {
        char nothing[12] = {0};
        char extPerm[12] = {0};
        memcpy(extPerm, perm, 8);
        return Cube(nothing, extPerm, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getEdgePerm(), 8);
    }),
    _cp(8, [](const char* perm) {
        char nothing[12] = {0};
        return Cube(perm, nothing, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getCornerPerm(), 8);
    }),
    _udslicep(4, [](const char* perm) {
        char nothing[12] = {0};
        char extPerm[12] = {0};
        memcpy(&extPerm[8], perm, 4);
        extPerm[8] += 8;
        extPerm[9] += 8;
        extPerm[10] += 8;
        extPerm[11] += 8;
        return Cube(nothing, extPerm, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, &cube.getEdgePerm()[8], 4);
    }),
    _phase1({{&_udslice, &_co, &_eo}}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}),
    _phase2({{&_udslicep, &_cp, &_ep}}, {U*3, U*3+1, U*3+2, D*3, D*3+1, D*3+2, R*3+1, B*3+1, L*3+1, F*3+1}) {
}

Solver333::~Solver333() {
}

std::vector<uint> Solver333::solve(const Cube& cube) {
    uint eo = _eo.fromCube(cube);
    uint co = _co.fromCube(cube);
    uint udslice = _udslice.fromCube(cube);
    int solution1[30];
    int solution2[30];

    _phase1.prepareSolve({{udslice, co, eo}});
    int length = _phase1.solve(solution1, 0, std::numeric_limits<int>::max());
    _phase1.convertSolutionToMoves(solution1, length);

    Cube cube2 = cube;
    for(int i = 0; i < length; ++i) {
        for(int j = 0; j <= solution1[i]%3; ++j) {
            cube2.applyMult(Cube((AXIS)(solution1[i]/3)));
        }
    }
    std::vector<uint> solution(solution1, &solution1[length]);

    uint ep = _ep.fromCube(cube2);
    uint cp = _cp.fromCube(cube2);
    uint udslicep = _udslicep.fromCube(cube2);

    _phase2.prepareSolve({{udslicep, cp, ep}});
    length = _phase2.solve(solution2, 0, std::numeric_limits<int>::max());
    _phase2.convertSolutionToMoves(solution2, length);

    solution.insert(solution.end(), solution2, &solution2[length]);
    return solution;
}

