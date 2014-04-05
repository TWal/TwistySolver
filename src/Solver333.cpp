#include "Solver333.h"
#include <cstring>
#include "Utils.h"
#include <algorithm>

static const std::vector<uint> phase1AllowedMoves = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
static const std::vector<uint> phase2AllowedMoves = {U*3, U*3+1, U*3+2, D*3, D*3+1, D*3+2, R*3+1, B*3+1, L*3+1, F*3+1};

Solver333::Solver333(const CubeProperties& props) :
    _props(props),
    _eo(12, 2, phase1AllowedMoves, _props, [this](const char* orient) {
        char nothing[12] = {0};
        return _props.makeCube(nothing, nothing, nothing, orient);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getEdgeOri(), 12);
    }, "333/1_eo.mvtbl"),
    _co(8, 3, phase1AllowedMoves, _props, [this](const char* orient) {
        char nothing[12] = {0};
        return _props.makeCube(nothing, nothing, orient, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getCornerOri(), 8);
    }, "333/1_co.mvtbl"),
    _udslice(12, {FL, FR, BR, BL}, phase1AllowedMoves, _props, [this](const char* perm) {
        char nothing[12] = {0};
        return _props.makeCube(nothing, perm, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getEdgePerm(), 12);
    }, "333/1_udslice.mvtbl"),
    _ep(8, phase2AllowedMoves, _props, [this](const char* perm) {
        char nothing[12] = {0};
        char extPerm[12] = {0};
        Utils::unzoom<8>(12, {{0, 1, 2, 3, 4, 5, 6, 7}}, perm, extPerm);
        return _props.makeCube(nothing, extPerm, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<8>({{0, 1, 2, 3, 4, 5, 6, 7}}, cube.getEdgePerm(), perm);
    }, "333/2_ep.mvtbl"),
    _cp(8, phase2AllowedMoves, _props, [this](const char* perm) {
        char nothing[12] = {0};
        return _props.makeCube(perm, nothing, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        memcpy(perm, cube.getCornerPerm(), 8);
    }, "333/2_cp.mvtbl"),
    _udslicep(4, phase2AllowedMoves, _props, [this](const char* perm) {
        char extPerm[12] = {0};
        char nothing[12] = {0};
        Utils::unzoom<4>(12, {{8, 9, 10, 11}}, perm, extPerm);
        return _props.makeCube(nothing, extPerm, nothing, nothing);
    }, [](const Cube& cube, char* perm) {
        Utils::zoom<4>({{8, 9, 10, 11}}, cube.getEdgePerm(), perm);
    }, "333/2_udslicep.mvtbl"),
    _phase1({{&_udslice, &_co, &_eo}}, phase1AllowedMoves),
    _phase2({{&_udslicep, &_cp, &_ep}}, phase2AllowedMoves) {
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
                    cube2.applyMult(_props.makeCube((AXIS)(solution1[i]/3), 1));
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

