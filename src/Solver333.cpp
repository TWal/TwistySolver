#include "Solver333.h"
#include <cstring>
#include "Utils.h"
#include <algorithm>

Solver333::Solver333() :
    _phase1EPT(nullptr), _phase1CPT(nullptr),
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
    }) {
    _eo.buildMoveTable();
    _co.buildMoveTable();
    _udslice.buildMoveTable();
    _ep.buildMoveTable();
    _cp.buildMoveTable();
    _udslicep.buildMoveTable();
    _buildPhase1EdgePruningTable();
    _buildPhase1CornerPruningTable();
    _buildPhase2EdgePruningTable();
    _buildPhase2CornerPruningTable();
}

Solver333::~Solver333() {
    delete[] _phase1EPT;
    delete[] _phase1CPT;
}

void Solver333::_buildPhase1EdgePruningTable() {
    uint nbEntries = _eo.size() * _udslice.size();
    _phase1EPT = new char[nbEntries];
    memset(_phase1EPT, -1, nbEntries);
    _phase1EPT[0] = 0;
    uint done = 1;
    char depth = 0;
    while(done < nbEntries) {
        for(uint i = 0; i < nbEntries; ++i) {
            if(_phase1EPT[i] == depth) {
                uint eo = i / _udslice.size();
                uint udslice = i % _udslice.size();
                for(uint j = 0; j < 18; ++j) {
                    uint newEo = _eo.moveTableLookup(eo, j);
                    uint newUdslice = _udslice.moveTableLookup(udslice, j);
                    uint newEntry = newEo * _udslice.size() + newUdslice;
                    if(_phase1EPT[newEntry] == -1) {
                        _phase1EPT[newEntry] = depth + 1;
                        ++done;
                    }
                }
            }
        }
        ++depth;
    }
}

void Solver333::_buildPhase1CornerPruningTable() {
    uint nbEntries = _co.size() * _udslice.size();
    _phase1CPT = new char[nbEntries];
    memset(_phase1CPT, -1, nbEntries);
    _phase1CPT[0] = 0;
    uint done = 1;
    char depth = 0;
    while(done < nbEntries) {
        for(uint i = 0; i < nbEntries; ++i) {
            if(_phase1CPT[i] == depth) {
                uint eo = i / _udslice.size();
                uint udslice = i % _udslice.size();
                for(uint j = 0; j < 18; ++j) {
                    uint newEo = _co.moveTableLookup(eo, j);
                    uint newUdslice = _udslice.moveTableLookup(udslice, j);
                    uint newEntry = newEo * _udslice.size() + newUdslice;
                    if(_phase1CPT[newEntry] == -1) {
                        _phase1CPT[newEntry] = depth + 1;
                        ++done;
                    }
                }
            }
        }
        ++depth;
    }
}

void Solver333::_buildPhase2EdgePruningTable() {
    uint nbEntries = _ep.size() * _udslicep.size();
    _phase2EPT = new char[nbEntries];
    memset(_phase2EPT, -1, nbEntries);
    _phase2EPT[0] = 0;
    uint done = 1;
    char depth = 0;
    while(done < nbEntries) {
        for(uint i = 0; i < nbEntries; ++i) {
            if(_phase2EPT[i] == depth) {
                uint ep = i / _udslicep.size();
                uint udslicep = i % _udslicep.size();
                for(uint j = 0; j < 18; ++j) {
                    if((j/3) != U && (j/3) != D && (j%3) != 1) {
                        continue;
                    }
                    uint newEp = _ep.moveTableLookup(ep, j);
                    uint newUdslicep = _udslicep.moveTableLookup(udslicep, j);
                    uint newEntry = newEp * _udslicep.size() + newUdslicep;
                    if(_phase2EPT[newEntry] == -1) {
                        _phase2EPT[newEntry] = depth + 1;
                        ++done;
                    }
                }
            }
        }
        ++depth;
    }
}

void Solver333::_buildPhase2CornerPruningTable() {
    uint nbEntries = _cp.size() * _udslicep.size();
    _phase2CPT = new char[nbEntries];
    memset(_phase2CPT, -1, nbEntries);
    _phase2CPT[0] = 0;
    uint done = 1;
    char depth = 0;
    while(done < nbEntries) {
        for(uint i = 0; i < nbEntries; ++i) {
            if(_phase2CPT[i] == depth) {
                uint cp = i / _udslicep.size();
                uint udslicep = i % _udslicep.size();
                for(uint j = 0; j < 18; ++j) {
                    if((j/3) != U && (j/3) != D && (j%3) != 1) {
                        continue;
                    }
                    uint newCp = _cp.moveTableLookup(cp, j);
                    uint newUdslicep = _udslicep.moveTableLookup(udslicep, j);
                    uint newEntry = newCp * _udslicep.size() + newUdslicep;
                    if(_phase2CPT[newEntry] == -1) {
                        _phase2CPT[newEntry] = depth + 1;
                        ++done;
                    }
                }
            }
        }
        ++depth;
    }
}

std::vector<uint> Solver333::solve(const Cube& cube) {
    uint eo = Utils::orientToInt(cube.getEdgeOri(), 12, 2);
    uint co = Utils::orientToInt(cube.getCornerOri(), 8, 3);
    uint udslice = Utils::posToInt(cube.getEdgePerm(), 12, {FL, FR, BR, BL});
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
    uint ep = Utils::permToInt(cube2.getEdgePerm(), 8);
    uint cp = Utils::permToInt(cube2.getCornerPerm(), 8);
    uint udslicep = Utils::permToInt(&cube2.getEdgePerm()[8], 4);
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
    return std::max(_phase1EPT[eo*_udslice.size() + udslice], _phase1CPT[co*_udslice.size() + udslice]);
}

uint Solver333::_estimateCostPhase2(uint ep, uint cp, uint udslicep) {
    return std::max(_phase2EPT[ep*_udslicep.size() + udslicep], _phase2CPT[cp*_udslicep.size() + udslicep]);
}

bool Solver333::_isMoveDisallowed(uint axis, uint* solution, uint depth) {
    if(depth == 0) {
        return false;
    }
    if(axis == (solution[depth-1]/3)) {
        return true;
    }
    if(axis+3 == (solution[depth-1]/3)) {
        return true;
    }
    return false;
}


