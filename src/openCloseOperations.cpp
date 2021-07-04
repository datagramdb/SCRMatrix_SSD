/*
 * openCloseOperations.cpp
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

#include "SCRMatrix_SSD/openCloseOperations.h"
#include <SCRMatrix_SSD/matrix_status.h>
#include <yaucl/fs/mmapFile.h>
#include <cassert>

bool SCRMatrix_SSD::open_matrix_status_serialize(SCRMatrix_SSD::matrix_status &M, const std::string &matrix_folder) {
    if (M.cellfd || M.colfd || M.rowfd) return false;
    std::filesystem::path p = matrix_folder;
    assert (!std::filesystem::exists(p));
    std::filesystem::create_directories(p);
    M.cellfd = fopen((p / "cell.bin").string().c_str(), "w");
    M.colfd = fopen((p / "col.bin").string().c_str(), "w");
    M.rowfd = fopen((p / "row.bin").string().c_str(), "w");
    M.matrix_folder = matrix_folder;
    M.first_insertion = true;
    return true;
}

bool SCRMatrix_SSD::open_matrix_status_read(SCRMatrix_SSD::matrix_status &M, const std::string &matrix_folder) {
    if (M.cellfd || M.colfd || M.rowfd) return false;
    std::filesystem::path p = matrix_folder;
    assert (std::filesystem::exists(p));

    M.nzcells_values = (double*)yaucl::fs::mmapFile((p / "cell.bin").c_str(), &M.nz_cells, &M.cell_fd);//fopen((p / "cell.bin").string().c_str(), "r");
    M.nz_cells = M.nz_cells / sizeof(double);

    M.nzcells_colids = (size_t*)yaucl::fs::mmapFile((p / "col.bin").c_str(), &M.ncols, &M.col_fd);//fopen((p / "cell.bin").string().c_str(), "r");
    M.ncols = M.ncols / sizeof(size_t);

    M.matrix_rowoffsets = (ssize_t*)yaucl::fs::mmapFile((p / "row.bin").c_str(), &M.nrows, &M.row_fd);//fopen((p / "cell.bin").string().c_str(), "r");
    M.nrows = M.nrows / sizeof(ssize_t);

    auto p1 = p / "col_primary.bin";
    auto p2 = p / "col_values.bin";
    if (std::filesystem::exists(p1) && std::filesystem::exists(p2) && M.index_in_memory) {

#ifdef DEBUG
        std::cerr << "Printing the columns [Debug]:" << std::endl;
        std::cerr << "-----------------------------" << std::endl;
#endif
        size_t nz_cols; int primary_fd;
        size_t values_size; int values_fd;
        size_t* primary = (size_t*)yaucl::fs::mmapFile(p1.c_str(), &nz_cols, &primary_fd);
        size_t* values = (size_t*)yaucl::fs::mmapFile(p2.c_str(), &values_size, &values_fd);

#ifdef DEBUG
        std::cerr << "primary: ";
        for (size_t i = 0, N = nz_cols/sizeof(size_t); i<N; i++) {
            std::cerr << primary[i] << ", ";
        }
        std::cerr << std::endl;
        std::cerr << "values: ";
        for (size_t i = 0, N = values_size/sizeof(size_t); i<N; i++) {
            std::cerr << values[i] << ", ";
        }
        std::cerr << std::endl;
#endif

        nz_cols = nz_cols / (sizeof(size_t));

        for (size_t i = 0; i<nz_cols; i+=2) {
            size_t colid = primary[i];
            size_t offset = primary[i+1];
            auto key = M.index_in_memory->insertKey(colid);
#ifdef DEBUG
            std::cerr << "\t Column " << colid << ": {";
#endif
            size_t entries = values[offset];
            assert(entries>0);
            for (size_t j = 0; j<entries; j++) {
                size_t rowid = values[offset+2+j*2];
                size_t offset_row = values[offset+2+j*2+1];
                key->add(rowid, offset_row);
#ifdef DEBUG
                std::cerr << ' ' << rowid << ":[" << M.nzcells_values[offset_row] << "] ";
#endif
            }
#ifdef DEBUG
            std::cerr << " }" << std::endl;
#endif
        }

        close(values_size);
        close(primary_fd);
    }

    return true;
}

bool SCRMatrix_SSD::close_matrix_status(SCRMatrix_SSD::matrix_status &M, bool dispose_while_iterating) {
    bool onWrite = false;
    if (M.cellfd) { fclose(M.cellfd); M.cellfd = nullptr; onWrite = true; }
    if (M.colfd)  { fclose(M.colfd); M.colfd = nullptr; onWrite = true; }
    if (M.rowfd) { fclose(M.rowfd); M.rowfd = nullptr; onWrite = true; }
    if (M.nzcells_values && M.cell_fd){ close(M.cell_fd); M.nzcells_values = nullptr; }
    if (M.nzcells_colids && M.col_fd) { close(M.col_fd); M.nzcells_colids = nullptr; }
    if (M.matrix_rowoffsets && M.row_fd) { close(M.row_fd); M.matrix_rowoffsets = nullptr; }
    M.first_insertion = false;
    M.nrows = 0;
    M.offset = M.nz_cells = M.ncols = 0;
    size_t zero = 0;
    if (onWrite && M.index_in_memory && (!M.matrix_folder.empty())) {
        {
            std::filesystem::path p = M.matrix_folder;
            if (!(std::filesystem::exists(p)))
                return false;
            FILE* col_primary = fopen((p / "col_primary.bin").string().c_str(), "w");
            FILE* col_values = fopen((p / "col_values.bin").string().c_str(), "w");
            size_t offset = 0;

            auto it = M.index_in_memory->iterator();
            auto curr = dispose_while_iterating ? it.disposeWhileIterating() : it.next();
            while (curr) {
                // <key, offset>
                fwrite(&curr->key, sizeof(size_t), 1, col_primary);
                fwrite(&offset, sizeof(size_t), 1, col_primary);
                size_t N = curr->overflowList.size();
                offset += /*sizeof(size_t) */ 2*  (N+1);

                // 1) <Block size, 0>
                fwrite(&N, sizeof(size_t), 1, col_values);
                fwrite(&zero, sizeof(size_t), 1, col_values);
                for (const std::pair<size_t, size_t>& offset : curr->overflowList) {
                    // 2) <row-id, cell_offset>
                    fwrite(&offset.first, sizeof(size_t), 1, col_values);
                    fwrite(&offset.second, sizeof(size_t), 1, col_values);
                }
                curr = dispose_while_iterating ? it.disposeWhileIterating() : it.next();
            }
            if (dispose_while_iterating) {
                M.index_in_memory->root = nullptr;
                M.index_in_memory = nullptr;
            }

            fclose(col_values);
            fclose(col_primary);
            return true;
        }

        // Serializing the index, if any

    } else
        return true;
}
