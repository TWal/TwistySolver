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

static const std::array<char, 16> ufdbCenters = {{
    0, 1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 15, 16, 17, 18, 19
}};

static const std::array<char, 8> lrCenters = {{
    8, 9, 10, 11, 20, 21, 22, 23
}};

Solver444::Solver444(const CubeProperties& props) :
    _props(props),
    _lrCenters1(24, {8, 9, 10, 11, 20, 21, 22, 23}, phase1AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setCenterPerm(0, 0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getCenterPerm(0, 0), 24);
    }),
    _udCenters2(16, {0, 1, 2, 3, 8, 9, 10, 11}, phase2AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<16>(24, ufdbCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<16>(ufdbCenters, cube.getCenterPerm(0, 0), perm);
    }),
    _lrCenters2(8, {0, 1, 2, 3}, phase2AllowedMoves, _props, [this](const char* perm) {
        char extPerm[24];
        Utils::unzoom<8>(24, lrCenters, perm, extPerm);
        return _props.makeCube().setCenterPerm(0, 0, extPerm);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>(lrCenters, cube.getCenterPerm(0, 0), perm);
    }),
    _edges2(24, {0, 2, 4, 6, 9, 11, 13, 15, 16, 19, 20, 23}, phase2AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setNEdgePerm(0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getNEdgePerm(0), 24);
    }),
    _phase1({{&_lrCenters1}}, phase1AllowedMoves, _props.identity()),
    _phase2({{&_udCenters2, &_lrCenters2, &_edges2}}, phase2AllowedMoves, _props.identity(), phase3AllowedMoves) {
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

    std::vector<uint> solution = solution1;
    solution.insert(solution.end(), solution2.begin(), solution2.end());
    callback(solution);
}

