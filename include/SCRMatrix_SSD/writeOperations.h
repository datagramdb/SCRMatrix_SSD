/*
 * writeOperations.h
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

#ifndef SCRMatrix_SSD_WRITEOPERATIONS_H
#define SCRMatrix_SSD_WRITEOPERATIONS_H

#include <SCRMatrix_SSD/matrix_status.h>

namespace SCRMatrix_SSD {

    /**
     * If the matrix is open on write, then writes the cell (i,j)=value to the files
     *
     * This method requires that either no other cell was emitted to the matrix file, or the previous cell that was
     * emitted was (h,k) < (i,j), thus requiring h<i || h==i && k<j. This assumption guarantees to write the matrix
     * in a fast way!
     *
     * If the given double in absolute value is less or equal than the machine epsilon, then the value is not emitted!
     *
     * @param M         Matrix open on write
     * @param i         Row id
     * @param j         Column id
     * @param value     Value to be associated
     * @return
     */
    bool emit_cell(matrix_status& M, size_t i, size_t j, double value);

    /**
     * Given M and N two matrices open on read, performs the matrix multiplication of M*N in result.
     *
     * TODO: this requires that N already has the column index.
     * TODO: this assumes that the column index is loaded in primary memory after that
     *
     * @param M             Left operand, open on read
     * @param N             Right operand, open on read
     * @param result        Result operand, open on write
     */
    void emit_matrix_multiplication(matrix_status& M, matrix_status& N, matrix_status& result);
}


#endif //SCRMatrix_SSD_WRITEOPERATIONS_H
