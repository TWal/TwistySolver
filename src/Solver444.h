#ifndef SOLVER444_H
#define SOLVER444_H

#include <vector>
#include "Cube.h"
#include "OrientationCoordinate.h"
#include "ParityCoordinate.h"
#include "PermutationCoordinate.h"
#include "PositionCoordinate.h"
#include "Phase.h"
#include "CubeProperties.h"

class Solver444 {
    public:
        Solver444(const CubeProperties& props = CubeProperties(4));
        ~Solver444();
        void solve(const Cube& cube, const std::function<bool (const std::vector<uint>&)>& callback);

    private:
        CubeProperties _props;
        PositionCoordinate _lrCenters1;
        PositionCoordinate _udCenters2;
        PositionCoordinate _lrCenters2;
        PositionCoordinate _edges2;
        ParityCoordinate _parity2;
        PositionCoordinate _fCenters3;
        PositionCoordinate _rCenters3;
        PositionCoordinate _flEdge3;
        PositionCoordinate _frEdge3;
        PositionCoordinate _brEdge3;
        PositionCoordinate _blEdge3;
        PositionCoordinate _uCenters4;
        PositionCoordinate _rCenters4;
        PositionCoordinate _ulEdge4;
        PositionCoordinate _urEdge4;
        PositionCoordinate _drEdge4;
        PositionCoordinate _dlEdge4;
        PositionCoordinate _ufEdge4;
        PositionCoordinate _ubEdge4;
        PositionCoordinate _dbEdge4;
        PositionCoordinate _dfEdge4;
        PositionCoordinate _fCenters5;
        PositionCoordinate _uCenters5;
        PositionCoordinate _ufEdge5;
        PositionCoordinate _ubEdge5;
        PositionCoordinate _dbEdge5;
        PositionCoordinate _dfEdge5;
        Phase<1, PhaseList<0>> _phase1;
        Phase<4, PhaseList<0, 1>, PhaseList<2, 3>> _phase2;
        Phase<6, PhaseList<0, 1>, PhaseList<2, 3>, PhaseList<4, 5>> _phase3;
        Phase<10, PhaseList<0, 1>, PhaseList<2, 3>, PhaseList<4, 5>, PhaseList<6, 7>, PhaseList<8, 9>> _phase4;
        Phase<6, PhaseList<0, 2, 3>, PhaseList<1, 4, 5>> _phase5;
};


#endif

