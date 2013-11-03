#ifndef SOLVER333_H
#define SOLVER333_H

#include <vector>
#include "Cube.h"
#include "OrientationCoordinate.h"
#include "PermutationCoordinate.h"
#include "PositionCoordinate.h"
#include "PruningTable.h"

class Solver333 {
    public:
        Solver333();
        ~Solver333();
        std::vector<uint> solve(const Cube& cube);
    private:
        uint _searchPhase1(uint eo, uint co, uint udslice, uint cost, uint bound, bool& found, uint* solution);
        uint _searchPhase2(uint ep, uint cp, uint udslicep, uint cost, uint bound, bool& found, uint* solution);
        uint _estimateCostPhase1(uint eo, uint co, uint udslice);
        uint _estimateCostPhase2(uint ep, uint cp, uint udslicep);
        static inline bool _isMoveDisallowed(uint axis, uint* solution, uint depth) {
            return (depth != 0) && ((axis == (solution[depth-1]/3)) || (axis+3 == (solution[depth-1]/3)));
        }
        OrientationCoordinate _eo;
        OrientationCoordinate _co;
        PositionCoordinate _udslice;
        PermutationCoordinate _ep;
        PermutationCoordinate _cp;
        PermutationCoordinate _udslicep;
        PruningTable<2> _phase1EPT;
        PruningTable<2> _phase1CPT;
        PruningTable<2> _phase2EPT;
        PruningTable<2> _phase2CPT;
};


#endif

