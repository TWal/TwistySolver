#ifndef CUBE_H
#define CUBE_H

#include "types.h"
#include "CornerPiece.h"
#include "EdgePiece.h"

class Cube {
    public:
        Cube();
        Cube(uint size);
        Cube(uint size, const char* cp, const char* ep, const char* co, const char* eo);
        Cube(uint size, AXIS axis, uint layer);

        void mult(const Cube& rhs, Cube& out) const;
        void applyMult(const Cube& rhs);

        const char* getCornerPerm() const;
        const char* getEdgePerm() const;
        const char* getCornerOri() const;
        const char* getEdgeOri() const;

    private:
        CornerPiece _corners;
        EdgePiece _edges;
};

#endif

