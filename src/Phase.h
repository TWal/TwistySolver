#ifndef PHASE_H
#define PHASE_H

#include <tuple>
#include <array>
#include "PruningTable.h"
#include <limits>
#include "types.h"
#include <algorithm>

template<unsigned int ... D>
struct PhaseList {
};

template<typename D>
struct PhaseDataEntryExtractor {
};

template<unsigned int ... R>
struct PhaseDataEntryExtractor<PhaseList<R ...>> {
    static const uint size = sizeof...(R);
    template<typename T, unsigned int NC>
    static inline std::array<T, size> f(const std::array<T, NC>& arr) {
        return {{arr[R] ...}};
    }
};

template<unsigned int N, typename D1, typename ... D>
struct PhaseDataExtractor : public PhaseDataExtractor<N-1, D ...> {
};

template<typename D1, typename ... D>
struct PhaseDataExtractor<0, D1, D ...> : public PhaseDataEntryExtractor<D1> {
};

template<typename T, unsigned int NC, unsigned int N, typename ... D>
struct PhaseDistanceEstimator {
    static inline char f(const T& pruning, const std::array<uint, NC>& state) {
        return std::max(PhaseDistanceEstimator<T, NC, N-1, D ...>::f(pruning, state), std::get<N>(pruning)->lookup(PhaseDataExtractor<N, D ...>::template f<uint, NC>(state)));
    }
};

template<typename T, unsigned int NC, typename ... D>
struct PhaseDistanceEstimator<T, NC, 0, D ...> {
    static inline char f(const T& pruning, const std::array<uint, NC>& state) {
        return std::get<0>(pruning)->lookup(PhaseDataExtractor<0, D ...>::template f<uint, NC>(state));
    }
};

template<typename T, unsigned int N>
struct PhasePruningTupleCleaner {
    static inline void f(const T& pruning) {
        delete std::get<N>(pruning);
        PhasePruningTupleCleaner<T, N-1>::f(pruning);
    }
};

template<typename T>
struct PhasePruningTupleCleaner<T, 0> {
    static inline void f(const T& pruning) {
        delete std::get<0>(pruning);
    }
};

template <unsigned int NC, typename ... D>
class Phase {
    private:
        typedef std::tuple<PruningTable<PhaseDataEntryExtractor<D>::size>* ...> pruning_tuple;
        static constexpr uint pruning_nb = sizeof...(D);

        std::array<Coordinate*, NC> _coords;
        pruning_tuple _pruning;
        std::vector<uint> _allowedMoves;

        //TODO: The whole class is generic, but this method is not
        inline bool _isMoveDisallowed(int move, int* solution, int depth) {
            move = _allowedMoves[move];
            return (depth != 0) && (((move/3) == (_allowedMoves[solution[depth-1]]/3)) || ((move/3)+3 == (_allowedMoves[solution[depth-1]]/3)));
        }

        template<unsigned int N> uint _pruningTableLookup(const std::array<uint, NC>& state) {
            return std::get<N>(_pruning)->lookup(PhaseDataExtractor<N, D ...>::template f<uint, NC>(state));
        }

        inline uint _estimateCost(const std::array<uint, NC>& state) {
            return PhaseDistanceEstimator<pruning_tuple, NC, pruning_nb-1, D ...>::f(_pruning, state);
        }

        inline void _applyMove(const std::array<uint, NC>& current, std::array<uint, NC>& output, int move) {
            for(int i = 0; i < NC; ++i) {
                output[i] = _coords[i]->moveTableLookup(current[i], move);
            }
        }

        inline void _saveCoordToStack(const std::array<uint, NC>& coords, uint* stack, int stackPos) {
            for(uint i = 0; i < NC; ++i) {
                stack[stackPos*NC+i] = coords[i];
            }
        }

        inline void _getCoordInStack(std::array<uint, NC>& coords, uint* stack, int stackPos) {
            for(uint i = 0; i < NC; ++i) {
                coords[i] = stack[stackPos*NC+i];
            }
        }

    public:
        Phase(const std::array<Coordinate*, NC>& coords, const std::vector<uint>& allowedMoves) :
            _coords(coords),
            _pruning(new PruningTable<PhaseDataEntryExtractor<D>::size>(PhaseDataEntryExtractor<D>::template f<Coordinate*, NC>(_coords), allowedMoves) ...),
            _allowedMoves(allowedMoves) {
        }

        ~Phase() {
            PhasePruningTupleCleaner<pruning_tuple, pruning_nb-1>::f(_pruning);
        }

        uint solve(const std::array<uint, NC>& coords, int* solution, int stackPos = 0, uint currentBound = 0) {
            bool found = false;
            uint nextBound = currentBound ? currentBound : _estimateCost(coords);
            std::array<uint, NC> currentCoords = coords;

            uint coordStack[NC*30]; //FIXME
            for(int i = 0; i < stackPos; ++i) {
                _saveCoordToStack(currentCoords, coordStack, i);
                _applyMove(currentCoords, currentCoords, _allowedMoves[solution[i]]);
            }
            _saveCoordToStack(currentCoords, coordStack, stackPos);

            while(!found) {
                uint bound = nextBound;
                nextBound = std::numeric_limits<uint>::max();
                stackPos = std::max(0, stackPos);
                do {
                    _getCoordInStack(currentCoords, coordStack, stackPos);
                    uint finalCostLow = stackPos + _estimateCost(currentCoords);
                    if(finalCostLow > bound) {
                        nextBound = std::min(finalCostLow, nextBound);
                        solution[stackPos] = -1;
                        --stackPos;
                        continue;
                    }
                    if(finalCostLow == stackPos) {
                        found = true;
                        break;
                    }
                    if(solution[stackPos]+1 == _allowedMoves.size()) {
                        solution[stackPos] = -1;
                        --stackPos;
                        continue;
                    }
                    ++solution[stackPos];
                    if(_isMoveDisallowed(solution[stackPos], solution, stackPos)) {
                        continue;
                    }
                    _applyMove(currentCoords, currentCoords, _allowedMoves[solution[stackPos]]);
                    ++stackPos;
                    _saveCoordToStack(currentCoords, coordStack, stackPos);
                } while(stackPos >= 0);
            }
            return stackPos;
        }

        void convertSolutionToMoves(int* solution, uint length) {
            for(uint i = 0; i < length; ++i) {
                solution[i] = _allowedMoves[solution[i]];
            }
        }
};

#endif

