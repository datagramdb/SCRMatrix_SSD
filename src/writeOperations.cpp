/*
 * writeOperations.cpp
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


#include "SCRMatrix_SSD/writeOperations.h"
using namespace SCRMatrix_SSD;

bool SCRMatrix_SSD::emit_cell(SCRMatrix_SSD::matrix_status &M, size_t i, size_t j, double value) {
    if (std::abs(value)<= std::numeric_limits<double>::epsilon())
        return true; // Skipping to write values that cannot be correctly represented by the machine!
    bool row_change = false;
    if ((M.first_insertion) || (i > M.prev_row)) {
        row_change = true;
        ssize_t no_elem = -1;
        do {
            M.prev_row++;
            fwrite(((M.prev_row == i) ? &M.offset : &no_elem),
                   sizeof(ssize_t),
                   1,
                   M.rowfd);
        } while (M.prev_row < i);
        M.prev_row = i;
        M.prev_col = j;
        M.first_insertion = false;
    } else {
        if (i == M.prev_row) {
            if (j <= M.prev_col) return false;
        } else if (i < M.prev_row) {
            return false;
        }
    }
    fwrite(&value, sizeof(double), 1, M.cellfd);
    fwrite(&j, sizeof(size_t), 1, M.colfd);
    if (M.index_in_memory)
        M.index_in_memory->insertKey(j)->add(i, M.offset);
    M.offset++;
    return true;
}

#include <cassert>

void SCRMatrix_SSD::emit_matrix_multiplication(SCRMatrix_SSD::matrix_status &M, SCRMatrix_SSD::matrix_status &N,
                                               SCRMatrix_SSD::matrix_status &result) {
    ssize_t curr_row_offset = -1, M_idx_Max = -1;
    double val = 0; size_t k_M = 0, k_N = 0;
    ssize_t i = -1, next_row = -1;
    size_t matrix_rowoffsets_offset = 0;
    while ((curr_row_offset==-1) && (i < (ssize_t)M.nrows)) {
        curr_row_offset = M.matrix_rowoffsets[matrix_rowoffsets_offset++];
        i++;
    }
    next_row = i;
    while ((M_idx_Max == -1) && (next_row < (ssize_t)M.nrows)) {
        M_idx_Max = M.matrix_rowoffsets[matrix_rowoffsets_offset++];
        next_row++;
    }
    for (ssize_t M_idx = 0; M_idx < M.nz_cells; ) {
        if (M_idx == M_idx_Max) {
            i = next_row;
            curr_row_offset = M_idx_Max;
            M_idx_Max = -1;
            while ((M_idx_Max == -1) && (next_row < (ssize_t)M.nrows)) {
                M_idx_Max = M.matrix_rowoffsets[matrix_rowoffsets_offset++];
                next_row++;
            }
        }

        size_t idx_curr = M_idx;
        size_t N_idx = 0;
        assert(N.index_in_memory);
        auto Nit = N.index_in_memory->iterator();
        while (Nit.hasNext()) {
            auto curr = Nit.next();
            size_t j = curr->key;

            M_idx = idx_curr;

            N_idx = 0;
            size_t N_idx_Max = curr->overflowList.size();
            double totalSum = 0.0;
            while ((M_idx != M_idx_Max) && (M_idx < M.nz_cells) && (N_idx < N_idx_Max)) {
                k_M = M.nzcells_colids[M_idx];
                auto& it_N_loc = curr->overflowList.at(N_idx);
                k_N = it_N_loc.first;
                if (k_M == k_N) {
                    totalSum += M.nzcells_values[M_idx] * N.nzcells_values[it_N_loc.second];
                    M_idx++;
                    N_idx++;
                } else if (k_M< k_N) {
                    M_idx++;
                } else {
                    N_idx++;
                }
            }
            if (std::abs(totalSum) > std::numeric_limits<double>::epsilon())
                emit_cell(result, i, j, totalSum);
        }
    }
}
