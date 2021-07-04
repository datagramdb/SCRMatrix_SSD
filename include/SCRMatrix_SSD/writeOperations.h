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
    bool emit_cell(matrix_status& M, size_t i, size_t j, double value);
    void emit_matrix_multiplication(matrix_status& M, matrix_status& N, matrix_status& result);
}


#endif //SCRMatrix_SSD_WRITEOPERATIONS_H
