//
// Created by giacomo on 04/07/21.
//
#include <SCRMatrix_SSD/all.h>
#include <yaucl/algorithms/binsearch.h>
#include <iostream>
#include <cassert>

void matrix_multiplication_with_self(const std::string& path = "matrix1", const std::string& resultPath = "result") {
    if (std::filesystem::exists(resultPath))
        assert(false);

    SCRMatrix_SSD::matrix_status M, result;
    {
        yaucl::structures::rbtree::RBTree<size_t, std::pair<size_t, size_t>> RBT{&compareUL_Int};
        M.index_in_memory = &RBT;
        result.index_in_memory = nullptr;
        if (!std::filesystem::exists(path)) {
            open_matrix_status_serialize(M, path);
            emit_cell(M, 0, 0, 1.0);
            emit_cell(M, 1, 1, 2.0);
            emit_cell(M, 1, 3, 4.0);
            emit_cell(M, 2, 0, 1.0);
            emit_cell(M, 2, 2, 3.0);
            emit_cell(M, 3, 3, 6.0);
            close_matrix_status(M, true);
        }
    }

    {
        yaucl::structures::rbtree::RBTree<size_t, std::pair<size_t, size_t>> RBT{&compareUL_Int};
        M.index_in_memory = &RBT;
        open_matrix_status_read(M, path);
        print_matrix(M);

        open_matrix_status_serialize(result, resultPath);
        emit_matrix_multiplication(M, M, result);
        close_matrix_status(result, true);
        close_matrix_status(M, true);
    }

    open_matrix_status_read(result, resultPath);
    print_matrix(result);
    close_matrix_status(result, true);
}

int main() {
    matrix_multiplication_with_self();
    return 0;
}