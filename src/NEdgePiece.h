#ifndef NEDGEPIECE_H
#define NEDGEPIECE_H

#include "types.h"

class NEdgePiece {
    public:
        NEdgePiece();
        NEdgePiece(const char* perm);
        NEdgePiece(AXIS axis, uint layer, uint pos);

        void mult(const NEdgePiece& rhs, NEdgePiece& out) const;

        const char* getOri() const;
        const char* getPerm() const;

        static const char permMove[2][6][24];
    private:
        char _perm[24];
};


#endif

