#ifndef SOLVER333_H
#define SOLVER333_H

#include <vector>
#include "Cube.h"
#include "OrientationCoordinate.h"
#include "PermutationCoordinate.h"
#include "PositionCoordinate.h"

class Solver333 {
    public:
        Solver333();
        ~Solver333();
        std::vector<uint> solve(const Cube& cube);
    private:
        void _buildPhase1EdgePruningTable();
        void _buildPhase1CornerPruningTable();
        void _buildPhase2EdgePruningTable();
        void _buildPhase2CornerPruningTable();
        uint _searchPhase1(uint eo, uint co, uint udslice, uint cost, uint bound, bool& found, uint* solution);
        uint _searchPhase2(uint ep, uint cp, uint udslicep, uint cost, uint bound, bool& found, uint* solution);
        uint _estimateCostPhase1(uint eo, uint co, uint udslice);
        uint _estimateCostPhase2(uint ep, uint cp, uint udslicep);
        bool _isMoveDisallowed(uint axis, uint* solution, uint depth);
        char* _phase1EPT;
        char* _phase1CPT;
        char* _phase2EPT;
        char* _phase2CPT;
        OrientationCoordinate _eo;
        OrientationCoordinate _co;
        PositionCoordinate _udslice;
        PermutationCoordinate _ep;
        PermutationCoordinate _cp;
        PermutationCoordinate _udslicep;
};


#endif

