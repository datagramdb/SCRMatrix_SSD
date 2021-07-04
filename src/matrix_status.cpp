/*
 * matrix_status.cpp
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
#include <SCRMatrix_SSD/matrix_status.h>

SCRMatrix_SSD::matrix_status::matrix_status() : ncols{0}, index_in_memory{nullptr}, matrix_rowoffsets{nullptr}, nzcells_values{nullptr}, nzcells_colids{nullptr}, cellfd{nullptr}, colfd{nullptr}, rowfd{nullptr}, cell_fd{0}, col_fd{0}, row_fd{0}, prev_row{-1}, prev_col{-1}, offset{0}, first_insertion{false}, nz_cells{0}, nrows{0} {}
