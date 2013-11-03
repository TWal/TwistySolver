#include "Solver333.h"
#include <cstring>
#include "Utils.h"
#include <algorithm>

static const std::vector<uint> phase1Moves = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
static const std::vector<uint> phase2Moves = {U*3, U*3+1, U*3+2, D*3, D*3+1, D*3+2, R*3+1, B*3+1, L*3+1, F*3+1};

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
    _phase1EPT({&_eo, &_udslice}, phase1Moves),
    _phase1CPT({&_co, &_udslice}, phase1Moves),
    _phase2EPT({&_ep, &_udslicep}, phase2Moves),
    _phase2CPT({&_cp, &_udslicep}, phase2Moves) {
}

Solver333::~Solver333() {
}

std::vector<uint> Solver333::solve(const Cube& cube) {
    uint eo = _eo.fromCube(cube);
    uint co = _co.fromCube(cube);
    uint udslice = _udslice.fromCube(cube);
    bool found = false;
    uint bound = _estimateCostPhase1(eo, co, udslice);
    uint solution1[30];

    while(!found) {
        bound = _searchPhase1(eo, co, udslice, 0, bound, found, solution1);
    }

    Cube cube2 = cube;
    for(uint i = 0; i < bound; ++i) {
        for(uint j = 0; j <= solution1[i]%3; ++j) {
            cube2.applyMult(Cube((AXIS)(solution1[i]/3)));
        }
    }
    std::vector<uint> solution(solution1, &solution1[bound]);
    uint ep = _ep.fromCube(cube2);
    uint cp = _cp.fromCube(cube2);
    uint udslicep = _udslicep.fromCube(cube2);
    found = false;
    bound = _estimateCostPhase2(ep, cp, udslicep);
    uint solution2[30];

    while(!found) {
        bound = _searchPhase2(ep, cp, udslicep, 0, bound, found, solution2);
    }

    solution.insert(solution.end(), solution2, &solution2[bound]);
    return solution;
}

uint Solver333::_searchPhase1(uint eo, uint co, uint udslice, uint cost, uint bound, bool& found, uint* solution) {
    if(eo == 0 && co == 0 && udslice == 0) {
        found = true;
        return cost;
    }

    uint finalCostLow = cost + _estimateCostPhase1(eo, co, udslice);
    if(finalCostLow > bound) {
        found = false;
        return finalCostLow;
    }

    uint min = std::numeric_limits<uint>::max();
    bool newFound;
    for(uint i = 0; i < 6; ++i) {
        if(_isMoveDisallowed(i, solution, cost)) {
            continue;
        }
        for(uint j = 1; j <= 3; ++j) {
            solution[cost] = i*3+j-1;
            uint newEo = _eo.moveTableLookup(eo, (AXIS)i, j);
            uint newCo = _co.moveTableLookup(co, (AXIS)i, j);
            uint newUdslice = _udslice.moveTableLookup(udslice, (AXIS)i, j);
            uint totalCost = _searchPhase1(newEo, newCo, newUdslice, cost + 1, bound, newFound, solution);
            if(newFound) {
                found = true;
                return totalCost;
            }
            min = std::min(totalCost, min);
        }
    }
    found = false;
    return min;
}

uint Solver333::_searchPhase2(uint ep, uint cp, uint udslicep, uint cost, uint bound, bool& found, uint* solution) {
    if(ep == 0 && cp == 0 && udslicep == 0) {
        found = true;
        return cost;
    }

    uint finalCostLow = cost + _estimateCostPhase2(ep, cp, udslicep);
    if(finalCostLow > bound) {
        found = false;
        return finalCostLow;
    }

    uint min = std::numeric_limits<uint>::max();
    bool newFound;
    for(uint i = 0; i < 6; ++i) {
        if(_isMoveDisallowed(i, solution, cost)) {
            continue;
        }
        for(uint j = 1; j <= 3; ++j) {
            if(i != U && i != D && j != 2) {
                continue;
            }
            solution[cost] = i*3+j-1;
            uint newEp = _ep.moveTableLookup(ep, (AXIS)i, j);
            uint newCp = _cp.moveTableLookup(cp, (AXIS)i, j);
            uint newUdslicep = _udslicep.moveTableLookup(udslicep, (AXIS)i, j);
            uint totalCost = _searchPhase2(newEp, newCp, newUdslicep, cost + 1, bound, newFound, solution);
            if(newFound) {
                found = true;
                return totalCost;
            }
            min = std::min(totalCost, min);
        }
    }
    found = false;
    return min;
}

uint Solver333::_estimateCostPhase1(uint eo, uint co, uint udslice) {
    return std::max(_phase1EPT.lookup({eo,udslice}), _phase1CPT.lookup({co, udslice}));
}

uint Solver333::_estimateCostPhase2(uint ep, uint cp, uint udslicep) {
    return std::max(_phase2EPT.lookup({ep, udslicep}), _phase2CPT.lookup({cp, udslicep}));
}

