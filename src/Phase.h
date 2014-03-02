#ifndef PHASE_H
#define PHASE_H

#include <tuple>
#include <array>
#include "PruningTable.h"
#include <limits>
#include "types.h"
#include <algorithm>
#include "PhaseStack.h"

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
        PhaseStack<NC> _stack;
        int _bound;
        int _nextBound;
        bool _shouldSkipFirstSolution;

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
        Phase(const std::array<Coordinate*, NC>& coords, const std::vector<uint>& allowedMoves, const Cube& identity = Cube(3)) :
            _coords(coords),
            _pruning(new PruningTable<PhaseDataEntryExtractor<D>::size>(PhaseDataEntryExtractor<D>::template f<Coordinate*, NC>(_coords), allowedMoves, identity) ...),
            _allowedMoves(allowedMoves),
            _stack(_coords, _allowedMoves) {
        }

        ~Phase() {
            PhasePruningTupleCleaner<pruning_tuple, pruning_nb-1>::f(_pruning);
        }

        void prepareSolve(const Cube& cube) {
            std::array<uint, NC> coords;
            for(uint i = 0; i < NC; ++i) {
                coords[i] = _coords[i]->fromCube(cube);
            }
            _stack.setFirstCoord(coords);
            _stack.setCost(0);
            _bound = _estimateCost(coords);
            _nextBound = std::numeric_limits<int>::max();
            _shouldSkipFirstSolution = false;
        }

        int solve(std::vector<uint>& solution, int totalCost, int totalBound) {
            int superBound = totalBound - totalCost;

            bool solutionAllowed = !_shouldSkipFirstSolution;
            _shouldSkipFirstSolution = true;
            bool found = false;
            while(!found) {
                _stack.setBound(_bound);
                do {
                    int finalCostLow = _stack.getCost() + _estimateCost(_stack.getCoord());
                    if(finalCostLow > _bound) {
                        _nextBound = std::min(finalCostLow, _nextBound);
                        _stack.backtrack();
                        continue;
                    }
                    if(finalCostLow == _stack.getCost() && finalCostLow == _bound) {
                        if(solutionAllowed) {
                            found = true;
                            break;
                        }
                        solutionAllowed = true;
                    }
                    if(_stack.getMove()+1 == _allowedMoves.size()) {
                        _stack.backtrack();
                        continue;
                    }

                    _stack.setMove(_stack.getMove()+1);
                    if(_stack.isMoveDisallowed()) {
                        continue;
                    }
                    _stack.advance();
                } while(!_stack.isEmpty());
                if(_nextBound > superBound) {
                    return -1; //Solution not found
                }
                if(!found) {
                    _stack.setCost(0);
                    _bound = _nextBound;
                    _nextBound = std::numeric_limits<int>::max();
                }
            }

            _stack.copySolution(solution);
            return _stack.getCost();
        }

        void convertSolutionToMoves(std::vector<uint>& solution) {
            for(uint& move : solution) {
                move = _allowedMoves[move];
            }
        }
};

#endif

