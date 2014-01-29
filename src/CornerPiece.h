#ifndef CORNERPIECE_H
#define CORNERPIECE_H

#include "types.h"

class CornerPiece {
    public:
        CornerPiece();
        CornerPiece(const char* perm, const char* ori);
        CornerPiece(AXIS axis);

        void mult(const CornerPiece& rhs, CornerPiece& out) const;

        const char* getOri() const;
        const char* getPerm() const;

        static const char permMove[6][8];
        static const char oriMove[6][8];

    private:
        char _perm[8];
        char _ori[8];
};

#endif

