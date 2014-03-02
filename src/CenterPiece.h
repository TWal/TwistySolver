#ifndef CENTERPIECE_H
#define CENTERPIECE_H

#include "types.h"

class CenterPiece {
    public:
        CenterPiece();
        CenterPiece(const char* perm);
        CenterPiece(AXIS axis, uint layer, uint x, uint y);

        void mult(const CenterPiece& rhs, CenterPiece& out) const;

        void makeIdentity();

        const char* getPerm() const;

        void setPerm(const char* perm);

        static const char permMove[3][6][24];
    private:
        char _perm[24];
};

#endif

