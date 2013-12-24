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

void Solver333::solve(const Cube& cube, const std::function<bool (const std::vector<uint>&)>& callback) {
    std::vector<uint> solution1;
    std::vector<uint> solution2;
    std::vector<uint> solution;
    int bound = std::numeric_limits<int>::max();

    _phase1.prepareSolve(cube);

    do {
        solution.clear();
        int length2 = -1;
        while(length2 == -1) {
            int length = _phase1.solve(solution1, 0, bound);
            if(length == -1) {
                return;
            }
            _phase1.convertSolutionToMoves(solution1);

            Cube cube2 = cube;
            for(int i = 0; i < length; ++i) {
                for(int j = 0; j <= solution1[i]%3; ++j) {
                    cube2.applyMult(Cube((AXIS)(solution1[i]/3)));
                }
            }
            solution.assign(solution1.begin(), solution1.end());

            _phase2.prepareSolve(cube2);
            length2 = _phase2.solve(solution2, length, bound);
            if(length2 != -1) {
                _phase2.convertSolutionToMoves(solution2);
                solution.insert(solution.end(), solution2.begin(), solution2.end());
            }
        }
        bound = solution.size() - 1;
    } while(callback(solution));
}

