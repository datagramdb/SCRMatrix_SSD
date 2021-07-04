/*
 * matrix_status.h
 * This file is part of SCRMatrix_SSD
 *
 * Copyright (C) 2021 - Giacomo Bergami
 *
 * SCRMatrix_SSD is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * SCRMatrix_SSD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with <program name>. If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef SCRMatrix_SSD_MATRIX_STATUS_H
#define SCRMatrix_SSD_MATRIX_STATUS_H

#include <cstdio>
#include <utility>
#include <yaucl/structures/rbtree/rbtree.h>

namespace SCRMatrix_SSD {
    /**
* In order to obtain C-style code, more efficient than using VTables for method invocation,
* we're using C++ just for struct-operations only (copy, assignment, move, construction).
*
* Initialization and deinitialization are demanded to C, as we're coping with file and fd-s,
* that should be closed only once
*/
    struct matrix_status {
        FILE *cellfd, *colfd, *rowfd;
        int  cell_fd, col_fd, row_fd;
        ssize_t prev_row, prev_col;
        bool first_insertion;
        ssize_t offset;
        std::string matrix_folder;
        yaucl::structures::rbtree::RBTree<size_t, std::pair<size_t, size_t>>* index_in_memory;

        double* nzcells_values;
        size_t* nzcells_colids;
        ssize_t* matrix_rowoffsets;
        size_t nrows, nz_cells, ncols;

        matrix_status();
        matrix_status(const matrix_status& ) = default;
        matrix_status(matrix_status&& ) = default;
        matrix_status&operator=(const matrix_status&) = default;
    };
}


#endif //SCRMatrix_SSD_MATRIX_STATUS_H
