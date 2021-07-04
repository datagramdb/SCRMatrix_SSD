/*
 * readOperations.h
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

#ifndef SCRMatrix_SSD_READOPERATIONS_H
#define SCRMatrix_SSD_READOPERATIONS_H

#include <SCRMatrix_SSD/matrix_status.h>


namespace SCRMatrix_SSD {

    /**
     * If the matrix M is open on read, prints all the non zero-value by row
     * @param M
     */
    void print_matrix(struct matrix_status& M);


    /**
     * If the matrix M is open on read, returns the value M[i,j]. 0 is returned if goes beyond the matrix limits, or
     * if the non-zero cell is not there.
     *
     * @param M     Matrix, that should be open on read
     * @param i     Row id, counting from zero
     * @param j     Column id, counting from zero
     * @return      Value associated to the matrix, if exists. Otherwise, 0.0 is returned
     */
    double getValue(struct matrix_status& M, size_t i, size_t j);

#if 0
    double matrixMultiplication(struct matrix_status& M, struct matrix_status& N, size_t i, size_t j) {
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

        double summation = 0.0;
        for (; i_offset < iplus_offset; i_offset++) {
            size_t curr_k = M.nzcells_colids[(size_t)i_offset];

            double getN_kj = getValue(N, curr_k, j);
            if (getN_kj != 0.0) {
                summation += M.nzcells_values[(size_t)i_offset] * getN_kj;
            }
        }
        return summation;
    }
#endif

}


#endif //SCRMatrix_SSD_READOPERATIONS_H
