#ifndef CUBE_H
#define CUBE_H

#include "types.h"
#include "CornerPiece.h"
#include "EdgePiece.h"
#include "CenterPiece.h"
#include "NEdgePiece.h"
#include <vector>

class Cube {
    public:
        Cube();
        Cube(uint size);
        Cube(uint size, const char* cp, const char* ep, const char* co, const char* eo);
        Cube(uint size, AXIS axis, uint layer);

        void mult(const Cube& rhs, Cube& out) const;
        void applyMult(const Cube& rhs);

        void makeIdentity();

        const char* getCornerPerm() const;
        const char* getEdgePerm() const;
        const char* getCornerOri() const;
        const char* getEdgeOri() const;
        const char* getCenterPerm(uint x, uint y) const;
        const char* getNEdgePerm(uint x) const;

        Cube& setCornerPerm(const char* data);
        Cube& setEdgePerm(const char* data);
        Cube& setCornerOri(const char* data);
        Cube& setEdgeOri(const char* data);
        Cube& setCenterPerm(uint x, uint y, const char* data);
        Cube& setNEdgePerm(uint x, const char* data);

    private:
        CornerPiece _corners;
        EdgePiece _edges;
        std::vector<CenterPiece> _centers;
        std::vector<NEdgePiece> _nedges;
        uint _size;
};

#endif

