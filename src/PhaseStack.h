#ifndef PHASESTACK_H
#define PHASESTACK_H

#include <vector>
#include <array>
#include "Coordinate.h"
#include "types.h"

template<unsigned int NC>
class PhaseStack {
    public:
        PhaseStack(const std::array<Coordinate*, NC>& coords, const std::vector<uint>& allowedMoves) :
            _moveStack(32, -1),
            _coordStack(33),
            _pos(0),
            _coords(coords),
            _allowedMoves(allowedMoves) {
        }

        inline void setFirstCoord(const std::array<uint, NC>& coords) {
            _coordStack[0] = coords;
        }

        inline void advance() {
            for(uint i = 0; i < NC; ++i) {
                _coordStack[_pos+1][i] = _coords[i]->moveTableLookup(_coordStack[_pos][i], _allowedMoves[_moveStack[_pos]]);
            }
            ++_pos;
        }

        inline void backtrack() {
            _moveStack[_pos] = -1;
            --_pos;
        }

        inline void setMove(int move) {
            _moveStack[_pos] = move;
        }

        inline int getMove() const {
            return _moveStack[_pos];
        }

        inline void setCost(int stackPos) {
            _pos = stackPos;
        }

        inline int getCost() const {
            return _pos;
        }

        inline const std::array<uint, NC>& getCoord() const {
            return _coordStack[_pos];
        }

        //TODO: The whole class is generic, but this method is not
        inline bool isMoveDisallowed() const {
            int move = _allowedMoves[_moveStack[_pos]];
            return (_pos != 0) && (((move/3) == (_allowedMoves[_moveStack[_pos-1]]/3)) || ((move/3)+3 == (_allowedMoves[_moveStack[_pos-1]]/3)));
        }

        inline bool isEmpty() const {
            return _pos < 0;
        }

        inline void setBound(int bound) {
            _moveStack.resize(bound, -1);
            _coordStack.resize(bound+1);
        }

        inline void copySolution(int* solution) const {
            for(int i = 0; i < _pos; ++i) {
                solution[i] = _moveStack[i];
            }
        }

    private:
        std::vector<int> _moveStack;
        std::vector<std::array<uint, NC>> _coordStack;
        int _pos;
        std::array<Coordinate*, NC> _coords;
        const std::vector<uint> _allowedMoves;
};

#endif

