#ifndef SOLVER333_H
#define SOLVER333_H

#include <vector>
#include "Cube.h"
#include "OrientationCoordinate.h"
#include "PermutationCoordinate.h"
#include "PositionCoordinate.h"
#include "Phase.h"
#include "CubeProperties.h"

class Solver333 {
    public:
        Solver333(const CubeProperties& props = CubeProperties(3));
        ~Solver333();
        void solve(const Cube& cube, const std::function<bool (const std::vector<uint>&)>& callback);

    private:
        CubeProperties _props;
        OrientationCoordinate _eo;
        OrientationCoordinate _co;
        PositionCoordinate _udslice;
        PermutationCoordinate _ep;
        PermutationCoordinate _cp;
        PermutationCoordinate _udslicep;
        Phase<3, PhaseList<0, 1>, PhaseList<0, 2>> _phase1;
        Phase<3, PhaseList<0, 1>, PhaseList<0, 2>> _phase2;
};


#endif

