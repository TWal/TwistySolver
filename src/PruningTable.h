#ifndef PRUNINGTABLE_H
#define PRUNINGTABLE_H

#include <array>
#include <vector>
#include "Coordinate.h"
#include <cstring>

template <unsigned int N>
class PruningTable {
    private:
        char* _pruningTable;
        std::array<Coordinate*, N> _coords;
    public:
        PruningTable(const std::array<Coordinate*, N>& coords, const std::vector<uint>& allowedMoves) :
            _pruningTable(nullptr),
            _coords(coords) {

            uint nbEntries = 1;
            for(Coordinate* coord : _coords) {
                nbEntries *= coord->size();
            }
            _pruningTable = new char[nbEntries];

            memset(_pruningTable, -1, nbEntries);

            Cube identity;
            std::array<uint, N> tmpCoords;

            for(uint i = 0; i < N; ++i) {
                tmpCoords[i] = _coords[i]->fromCube(identity);
            }
            _pruningTable[makeIndex(tmpCoords)] = 0;

            uint done = 1;
            char depth = 0;
            while(done < nbEntries) {
                for(uint i = 0; i < nbEntries; ++i) {
                    if(_pruningTable[i] == depth) {
                        std::array<uint, N> currentCoords;
                        unmakeIndex(i, currentCoords);
                        for(uint j : allowedMoves) {
                            for(int k = 0; k < N; ++k) {
                                tmpCoords[k] = _coords[k]->moveTableLookup(currentCoords[k], j);
                            }
                            uint newEntry = makeIndex(tmpCoords);
                            if(_pruningTable[newEntry] == -1) {
                                _pruningTable[newEntry] = depth + 1;
                                ++done;
                            }
                        }
                    }
                }
                ++depth;
            }
        }

        ~PruningTable() {
            delete[] _pruningTable;
        }

        inline uint makeIndex(const std::array<uint, N>& indexes) {
            uint res = 0;
            for(uint i = 0; i < N; ++i) {
                res *= _coords[i]->size();
                res += indexes[i];
            }
            return res;
        }

        inline void unmakeIndex(uint entry, std::array<uint, N>& indexes) {
            for(int i = N-1; i >= 0; --i) {
                indexes[i] = entry % _coords[i]->size();
                entry /= _coords[i]->size();
            }
        }

        inline char lookup(const std::array<uint, N>& indexes) {
            return _pruningTable[makeIndex(indexes)];
        }
};

#endif

