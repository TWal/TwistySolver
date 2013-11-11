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

        static inline bool _isMoveDisallowed(uint axis, uint* solution, uint depth) {
            return (depth != 0) && (((axis/3) == (solution[depth-1]/3)) || ((axis/3)+3 == (solution[depth-1]/3)));
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

    public:
        Phase(const std::array<Coordinate*, NC>& coords, const std::vector<uint>& allowedMoves) :
            _coords(coords),
            _pruning(new PruningTable<PhaseDataEntryExtractor<D>::size>(PhaseDataEntryExtractor<D>::template f<Coordinate*, NC>(_coords), allowedMoves) ...),
            _allowedMoves(allowedMoves) {
        }

        ~Phase() {
            PhasePruningTupleCleaner<pruning_tuple, pruning_nb-1>::f(_pruning);
        }

        uint solve(const std::array<uint, NC>& coords, uint* solution) {
            uint bound = _estimateCost(coords);
            bool found = false;
            while(!found) {
                bound = search(coords, 0, bound, found, solution);
            }
            return bound;
        }

        uint search(const std::array<uint, NC>& coords, uint cost, uint bound, bool& found, uint* solution) {
            uint finalCostLow = cost + _estimateCost(coords);
            if(finalCostLow == cost) {
                found = true;
                return cost;
            }

            if(finalCostLow > bound) {
                found = false;
                return finalCostLow;
            }

            uint min = std::numeric_limits<uint>::max();
            bool newFound;
            std::array<uint, NC> newCoords;
            for(uint i : _allowedMoves) {
                if(_isMoveDisallowed(i, solution, cost)) {
                    continue;
                }
                solution[cost] = i;
                _applyMove(coords, newCoords, i);
                uint totalCost = search(newCoords, cost + 1, bound, newFound, solution);
                if(newFound) {
                    found = true;
                    return totalCost;
                }
                min = std::min(totalCost, min);
            }
            found = false;
            return min;
        }
};

#endif

