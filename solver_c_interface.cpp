#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Solver.hpp"

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

using namespace GameSolver::Connect4;

extern "C" {

    struct SolverHandle {
        Solver solver;
    };

    DLLEXPORT SolverHandle* solver_init(const char* opening_book) {
        SolverHandle* handle = new SolverHandle;
        handle->solver.loadBook(opening_book ? opening_book : "7x6.book");
        return handle;
    }

    DLLEXPORT  void solver_delete(SolverHandle* handle) {
        delete handle;
    }

    DLLEXPORT  const char* solver_solve(SolverHandle* handle, const char* past_moves, bool weak, bool analyze, char* result_buffer, size_t buffer_size) {
        if (!handle || !past_moves || !result_buffer || buffer_size == 0) {
            return nullptr;
        }

        Position P;
        std::string moves_str(past_moves);
        if (P.play(moves_str) != moves_str.size()) {
            snprintf(result_buffer, buffer_size, "Invalid move");
            return result_buffer;
        }

        std::stringstream ss;
        ss << moves_str;

        if (analyze) {
            std::vector<int> scores = handle->solver.analyze(P, weak);
            for (int score : scores) {
                ss << " " << score;
            }
        }
        else {
            int score = handle->solver.solve(P, weak);
            ss << " " << score;
        }

        std::string result = ss.str();
        if (result.size() >= buffer_size) {
            snprintf(result_buffer, buffer_size, "Result too long");
            return result_buffer;
        }

        strcpy(result_buffer, result.c_str());
        return result_buffer;
    }
}