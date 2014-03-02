#include "Solver444.h"

static const std::vector<uint> phase1AllowedMoves = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
};

Solver444::Solver444(const CubeProperties& props) :
    _props(props),
    _lCenters(24, {8, 9, 10, 11, 20, 21, 22, 23}, phase1AllowedMoves, _props, [this](const char* perm) {
        return _props.makeCube().setCenterPerm(0, 0, perm);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getCenterPerm(0, 0), 24);
    }),
    _phase1({&_lCenters}, phase1AllowedMoves, _props.identity()) {
}

Solver444::~Solver444() {
}

void Solver444::solve(const Cube& cube, const std::function<bool (const std::vector<uint>&)>& callback) {
    std::vector<uint> solution;
    _phase1.prepareSolve(cube);
    _phase1.solve(solution, 0, 1000);
    _phase1.convertSolutionToMoves(solution);
    callback(solution);
}

