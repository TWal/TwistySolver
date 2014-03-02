#ifndef EDGEPIECE_H
#define EDGEPIECE_H

#include "types.h"

class EdgePiece {
    public:
        EdgePiece();
        EdgePiece(const char* perm, const char* ori);
        EdgePiece(AXIS axis);

        void mult(const EdgePiece& rhs, EdgePiece& out) const;

        void makeIdentity();

        const char* getOri() const;
        const char* getPerm() const;

        void setPerm(const char* perm);
        void setOri(const char* ori);

        static const char permMove[6][12];
        static const char oriMove[6][12];

    private:
        char _perm[12];
        char _ori[12];
};

#endif

