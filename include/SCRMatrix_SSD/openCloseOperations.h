/*
 * openCloseOperations.h
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

#ifndef SCRMatrix_SSD_OPENCLOSEOPERATIONS_H
#define SCRMatrix_SSD_OPENCLOSEOPERATIONS_H

#include <string>
#include <filesystem>
#include <SCRMatrix_SSD/matrix_status.h>

namespace SCRMatrix_SSD {
    /**
     * Open the matrix for writing: this assumes that the provided folder does not exist.
     * The folder will contain the matrix representation
     *
     * @param M  Primary memory reference to the matrix
     * @param matrix_folder         Matrix path
     * @return                      Whether the matrix was successfully opened
     */
    bool open_matrix_status_serialize(struct matrix_status& M, const std::string& matrix_folder);


    /**
     * Opens the matrix in read mode by memory mapping the previously-serialized files. The matrix should be there!
     *
     * @param M
     * @param matrix_folder
     * @return
     */
    bool open_matrix_status_read(struct matrix_status& M, const std::string& matrix_folder);

    /**
     * Closes the matrix, both in reading and writing
     * @param M
     */
    bool close_matrix_status(struct matrix_status& M, bool dispose_while_iterating = false);
}

#endif //SCRMatrix_SSD_OPENCLOSEOPERATIONS_H
