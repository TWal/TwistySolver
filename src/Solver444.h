#ifndef SOLVER444_H
#define SOLVER444_H

#include <vector>
#include "Cube.h"
#include "OrientationCoordinate.h"
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
        Phase<1, PhaseList<0>> _phase1;
        //Phase<3, PhaseList<2>> _phase2;
        Phase<3, PhaseList<0, 1>, PhaseList<2>> _phase2;
};


#endif

