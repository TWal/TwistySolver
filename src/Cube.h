#ifndef CUBE_H
#define CUBE_H

#include "types.h"

class Cube {
    public:
        Cube();
        Cube(uint cp, uint ep, uint co, uint eo);
        Cube(const char* cp, const char* ep, const char* co, const char* eo);
        Cube(AXIS axis);

        void cMult(const Cube& rhs, Cube& out) const;
        void eMult(const Cube& rhs, Cube& out) const;
        void mult(const Cube& rhs, Cube& out) const;
        void applyCMult(const Cube& rhs);
        void applyEMult(const Cube& rhs);
        void applyMult(const Cube& rhs);

        const char* getCornerPerm() const;
        const char* getEdgePerm() const;
        const char* getCornerOri() const;
        const char* getEdgeOri() const;

        static const char cpermMove[6][8];
        static const char epermMove[6][12];
        static const char coriMove[6][8];
        static const char eoriMove[6][12];

    private:
        char _cperm[8];
        char _eperm[12];
        char _cori[8];
        char _eori[12];
};

#endif

