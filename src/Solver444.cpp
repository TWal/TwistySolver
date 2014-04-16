#include "Solver444.h"
#include "Utils.h"

static const std::vector<uint> phase1AllowedMoves = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
};

static const std::vector<uint> phase2AllowedMoves = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
        19,         22,     24, 25, 26,     28,         31,     33, 34, 35
};

static const std::vector<uint> phase3AllowedMoves = {
     0,  1,  2,  3,  4,  5,      7,      9, 10, 11, 12, 13, 14,     16,
        19,         22,         25,         28,         31,         34
};

static const std::vector<uint> phase4AllowedMoves = {
     0,  1,  2,      4,          7,      9, 10, 11,     13,         16,
                    22,         25,                     31,         34
};

static const std::vector<uint> phase5AllowedMoves = {
         1,          4,          7,         10,         13,         16,
                                25,                                 34
};

static const std::array<char, 16> ufdbCenters = {{
    0, 1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 15, 16, 17, 18, 19
}};

static const std::array<char, 8> lrCenters = {{
    8, 9, 10, 11, 20, 21, 22, 23
}};

static const std::array<char, 8> udCenters = {{
    0, 1, 2, 3, 12, 13, 14, 15
}};

static const std::array<char, 8> fbCenters = {{
    4, 5, 6, 7, 16, 17, 18, 19
}};

static const std::array<char, 12> lowEdges = {{
    0, 2, 4, 6, 9, 11, 13, 15, 16, 19, 20, 23
}};

static const std::array<char, 12> highEdges = {{
    1, 3, 5, 7, 8, 10, 12, 14, 17, 18, 21, 22
}};

static const std::array<char, 16> udEdges = {{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
}};

static const std::array<char, 8> mEdges = {{
    0, 1, 4, 5, 8, 9, 12, 13
}};

Solver444::Solver444(const CubeProperties& props) :
    _props(props),
    _lrCenters1(24, {8, 9, 10, 11, 20, 21, 22, 23}, phase1AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setCenterPerm(0, 0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getCenterPerm(0, 0), 24);
    }, "444/1_lrCenters.mvtbl"),
    _udCenters2(16, {0, 1, 2, 3, 8, 9, 10, 11}, phase2AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, ufdbCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(ufdbCenters, cube.getCenterPerm(0, 0), perm);
    }, "444/2_udCenters.mvtbl"),
    _lrCenters2(8, {0, 1, 2, 3}, phase2AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, lrCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(lrCenters, cube.getCenterPerm(0, 0), perm);
    }, "444/2_lrCenters.mvtbl"),
    _edges2(24, std::unordered_set<char>(lowEdges.begin(), lowEdges.end()), phase2AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setNEdgePerm(0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getNEdgePerm(0), 24);
    }, "444/2_edges.mvtbl"),
    _parity2(24, phase2AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setNEdgePerm(0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getNEdgePerm(0), 24);
    }),
    _fCenters3(8, {0, 1, 2, 3}, phase3AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, fbCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(fbCenters, cube.getCenterPerm(0, 0), perm);
    }), //, "444/3_fCenters.mvtbl"),
    _rCenters3(8, {0, 1, 2, 3}, phase3AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, lrCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(lrCenters, cube.getCenterPerm(0, 0), perm);
    }), //, "444/3_rCenters.mvtbl"),
    _flEdge3(24, {16, 17}, phase3AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setNEdgePerm(0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getNEdgePerm(0), 24);
    }),
    _frEdge3(24, {18, 19}, phase3AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setNEdgePerm(0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getNEdgePerm(0), 24);
    }),
    _brEdge3(24, {20, 21}, phase3AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setNEdgePerm(0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getNEdgePerm(0), 24);
    }),
    _blEdge3(24, {22, 23}, phase3AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setNEdgePerm(0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getNEdgePerm(0), 24);
    }),
    _uCenters4(8, {0, 1, 2, 3}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, udCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(udCenters, cube.getCenterPerm(0, 0), perm);
    }),
    _rCenters4(8, {0, 1, 2, 3}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, lrCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(lrCenters, cube.getCenterPerm(0, 0), perm);
    }),
    _ulEdge4(16, {2, 3}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, udEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(udEdges, cube.getNEdgePerm(0), perm);
    }),
    _urEdge4(16, {6, 7}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, udEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(udEdges, cube.getNEdgePerm(0), perm);
    }),
    _drEdge4(16, {14, 15}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, udEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(udEdges, cube.getNEdgePerm(0), perm);
    }),
    _dlEdge4(16, {10, 11}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, udEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(udEdges, cube.getNEdgePerm(0), perm);
    }),
    _ufEdge4(16, {0, 1}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, udEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(udEdges, cube.getNEdgePerm(0), perm);
    }),
    _ubEdge4(16, {4, 5}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, udEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(udEdges, cube.getNEdgePerm(0), perm);
    }),
    _dbEdge4(16, {12, 13}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, udEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(udEdges, cube.getNEdgePerm(0), perm);
    }),
    _dfEdge4(16, {8, 9}, phase4AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, udEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(udEdges, cube.getNEdgePerm(0), perm);
    }),
    _fCenters5(8, {0, 1, 2, 3}, phase5AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, fbCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(fbCenters, cube.getCenterPerm(0, 0), perm);
    }),
    _uCenters5(8, {0, 1, 2, 3}, phase5AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, udCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(udCenters, cube.getCenterPerm(0, 0), perm);
    }),
    _ufEdge5(8, {0, 1}, phase5AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, mEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(mEdges, cube.getNEdgePerm(0), perm);
    }),
    _ubEdge5(8, {2, 3}, phase5AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, mEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(mEdges, cube.getNEdgePerm(0), perm);
    }),
    _dbEdge5(8, {6, 7}, phase5AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, mEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(mEdges, cube.getNEdgePerm(0), perm);
    }),
    _dfEdge5(8, {4, 5}, phase5AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, mEdges, perm, extPerm);
        return _props.makeCube().setNEdgePerm(0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(mEdges, cube.getNEdgePerm(0), perm);
    }),

    _phase1({{&_lrCenters1}}, phase1AllowedMoves, _props.identity()),
    _phase2({{&_udCenters2, &_lrCenters2, &_edges2, &_parity2}}, phase2AllowedMoves, _props.identity(), phase3AllowedMoves),
    _phase3({{&_fCenters3, &_rCenters3, &_flEdge3, &_frEdge3, &_brEdge3, &_blEdge3}}, phase3AllowedMoves, _props.identity(), phase4AllowedMoves),
    _phase4({{&_uCenters4, &_rCenters4, &_ulEdge4, &_urEdge4, &_drEdge4, &_dlEdge4, &_ufEdge4, &_ubEdge4, &_dbEdge4, &_dfEdge4}}, phase4AllowedMoves, _props.identity(), phase5AllowedMoves),
    _phase5({{&_fCenters5, &_uCenters5, &_ufEdge5, &_ubEdge5, &_dbEdge5, &_dfEdge5}}, phase5AllowedMoves, _props.identity()) {
}

Solver444::~Solver444() {
}

void Solver444::solve(const Cube& cube, const std::function<bool (const std::vector<uint>&)>& callback) {
    std::vector<uint> solution1;
    _phase1.prepareSolve(cube);
    _phase1.solve(solution1, 0, 1000);
    _phase1.convertSolutionToMoves(solution1);

    Cube cube2 = cube;
    for(uint move : solution1) {
        for(int i = 0; i <= Utils::getNb(move); ++i) {
            cube2.applyMult(_props.makeCube((AXIS)(Utils::getAxis(move)), Utils::getLayer(move)));
        }
    }
    std::vector<uint> solution2;
    _phase2.prepareSolve(cube2);
    _phase2.solve(solution2, 0, 1000);
    _phase2.convertSolutionToMoves(solution2);

    for(uint move : solution2) {
        for(int i = 0; i <= Utils::getNb(move); ++i) {
            cube2.applyMult(_props.makeCube((AXIS)(Utils::getAxis(move)), Utils::getLayer(move)));
        }
    }

    std::vector<uint> solution3;
    _phase3.prepareSolve(cube2);
    _phase3.solve(solution3, 0, 1000);
    _phase3.convertSolutionToMoves(solution3);

    for(uint move : solution3) {
        for(int i = 0; i <= Utils::getNb(move); ++i) {
            cube2.applyMult(_props.makeCube((AXIS)(Utils::getAxis(move)), Utils::getLayer(move)));
        }
    }

    std::vector<uint> solution4;
    _phase4.prepareSolve(cube2);
    _phase4.solve(solution4, 0, 1000);
    _phase4.convertSolutionToMoves(solution4);

    for(uint move : solution4) {
        for(int i = 0; i <= Utils::getNb(move); ++i) {
            cube2.applyMult(_props.makeCube((AXIS)(Utils::getAxis(move)), Utils::getLayer(move)));
        }
    }

    std::vector<uint> solution5;
    _phase5.prepareSolve(cube2);
    _phase5.solve(solution5, 0, 1000);
    _phase5.convertSolutionToMoves(solution5);

    for(uint move : solution5) {
        for(int i = 0; i <= Utils::getNb(move); ++i) {
            cube2.applyMult(_props.makeCube((AXIS)(Utils::getAxis(move)), Utils::getLayer(move)));
        }
    }

    Cube cube3(3);
    char edgePerm[12];
    cube2.getNEdge(0).convertToGroupedEdges(edgePerm);
    cube3.setEdgePerm(edgePerm);
    cube3.setCornerPerm(cube2.getCornerPerm());
    cube3.setCornerOri(cube2.getCornerOri());

    std::vector<uint> solution6;

    //Fix parity   TODO: do it the right way
    if(ParityCoordinate::parToInt(cube3.getEdgePerm(), 12) != ParityCoordinate::parToInt(cube3.getCornerPerm(), 8)) {
        //Insert parity algorithm
        solution6.assign({7, 25, 1, 7, 25, 19, 7, 25, 1, 19});
        std::swap(edgePerm[UF], edgePerm[UB]);
        cube3.setEdgePerm(edgePerm);
    }
    std::vector<uint> solution7;
    _solver333.solve(cube3, [&](const std::vector<uint>& solution) {
        solution7.assign(solution.begin(), solution.end());
        return false;
    });

    std::vector<uint> solution = solution1;
    solution.insert(solution.end(), solution2.begin(), solution2.end());
    solution.insert(solution.end(), solution3.begin(), solution3.end());
    solution.insert(solution.end(), solution4.begin(), solution4.end());
    solution.insert(solution.end(), solution5.begin(), solution5.end());
    solution.insert(solution.end(), solution6.begin(), solution6.end());
    solution.insert(solution.end(), solution7.begin(), solution7.end());

    callback(solution);
}

