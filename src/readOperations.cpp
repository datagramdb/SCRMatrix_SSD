/*
 * readOperations.cpp
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

#include "SCRMatrix_SSD/readOperations.h"
#include <unordered_map>
#include <vector>
using namespace SCRMatrix_SSD;

void SCRMatrix_SSD::print_matrix(SCRMatrix_SSD:: matrix_status &M) {
    ssize_t curr_row_offset = -1, next_row_offset = -1;
    double val = 0; size_t col = 0;
    ssize_t curr_row = -1, next_row = -1;
    size_t matrix_rowoffsets_offset = 0;
#ifdef DEBUG
    std::cout << "Printing the matrix:" << std::endl;
    std::cout << "--------------------" << std::endl;
#endif
    while ((curr_row_offset==-1) && (curr_row < (ssize_t)M.nrows)) {
        curr_row_offset = M.matrix_rowoffsets[matrix_rowoffsets_offset++];
        //fread(&curr_row_offset, sizeof(ssize_t), 1, matrix_status_holder.rowfd);
        curr_row++;
    }
    next_row = curr_row;
    while ((next_row_offset==-1) && (next_row < (ssize_t)M.nrows)) {
        next_row_offset = M.matrix_rowoffsets[matrix_rowoffsets_offset++];
        next_row++;
    }
    for (ssize_t i = 0; i < M.nz_cells; /*i++*/) {
        if (i == next_row_offset) {
            curr_row = next_row;
            curr_row_offset = next_row_offset;
            next_row_offset = -1;
            while ((next_row_offset == -1) && (next_row < (ssize_t)M.nrows)) {
                next_row_offset = M.matrix_rowoffsets[matrix_rowoffsets_offset++];
                next_row++;
            }
        }
#if 0
        val = M.nzcells_values[i];
        col = M.nzcells_colids[i];
        std::cout << "M(" << curr_row << ',' << col << ") = " << val << std::endl;
#else
        while ((i != next_row_offset) && (i < M.nz_cells)) {
            val = M.nzcells_values[i];
            col = M.nzcells_colids[i];
#ifdef DEBUG
            std::cout << "M(" << curr_row << ',' << col << ") = " << val << std::endl;
#endif
            i++;
        }
#endif
    }
}

double SCRMatrix_SSD::getValue(SCRMatrix_SSD:: matrix_status &M, size_t i, size_t j) {
    // If i exceedes the maximum number of the serialized rows, necessarily the associated value is zero!
    if (i>=M.nrows)
        return 0.0;

    ssize_t i_offset = M.matrix_rowoffsets[i];
    size_t curr_row = i;
    ssize_t iplus_offset = (i == (M.nrows-1)) ? M.nz_cells : M.matrix_rowoffsets[++curr_row];
    // TODO: more efficient without scanning the rows, by not serializing -1
    // This should also imply to refactor the matrix print operation!
    while ((iplus_offset==-1) && ((++curr_row) < M.nrows)) {
        iplus_offset = M.matrix_rowoffsets[curr_row];
    }

    // If the offset associated to i is -1, it means that the row is completely empty, so the value must be zero!
    if (i_offset==-1)
        return 0.0;

    for (; i_offset < iplus_offset; i_offset++) {
        // If I have already skipped the row-id, then the value is missing
        if (M.nzcells_colids[(size_t)i_offset]>j) return 0.0;
        else if (M.nzcells_colids[(size_t)i_offset]==j) return M.nzcells_values[(size_t)i_offset];
    }
    return 0.0;
}
