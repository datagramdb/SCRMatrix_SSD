//
// Created by giacomo on 04/07/21.
//
#include <SCRMatrix_SSD/all.h>
#include <yaucl/algorithms/binsearch.h>
#include <iostream>

void preliminary(const std::string& path = "/home/giacomo/test_directory") {
    SCRMatrix_SSD::matrix_status M;

    {
        yaucl::structures::rbtree::RBTree<size_t, std::pair<size_t, size_t>> RBT{&compareUL_Int};
        M.index_in_memory = &RBT;
        if (!std::filesystem::exists(path)) {
            open_matrix_status_serialize(M, path);
            emit_cell(M, 0, 1, 0.1);
            emit_cell(M, 0, 3, 0.2);
            emit_cell(M, 10, 2, 0.3);
            emit_cell(M, 10, 3, 0.4);
            emit_cell(M, 11, 5, 0.5);
            close_matrix_status(M, true);
        }
    }

    {
        yaucl::structures::rbtree::RBTree<size_t, std::pair<size_t, size_t>> RBT{&compareUL_Int};
        M.index_in_memory = &RBT;
        open_matrix_status_read(M,path);
        print_matrix(M);

        // Might close RBT here!
        std::cout << getValue(M, 0, 0) << std::endl;
        std::cout << getValue(M, 0, 1) << std::endl;
        std::cout << getValue(M, 0, 3) << std::endl;
        std::cout << getValue(M, 10, 11) << std::endl;
        std::cout << getValue(M, 10, 2) << std::endl;
        std::cout << getValue(M, 10, 23) << std::endl;
        std::cout << getValue(M, 40, 23) << std::endl;
        std::cout << getValue(M, 11, 5) << std::endl;
        std::cout << getValue(M, 10, 3) << std::endl;
        close_matrix_status(M, true);
    }


}

int main() {
    preliminary();
    return 0;
}