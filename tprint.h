/*
 * Table Print utilities
 * Copyright (C) 2012-2013 Paul Ionkin <paul.ionkin@gmail.com>
 * Copyright (C) 2013 Vicent Selfa <vtselfa@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef TABLE_PRINT_H
#define TABLE_PRINT_H

#include <stdio.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

enum TPrintAlign {
    TPAlign_left = 0,
    TPAlign_center = 1,
    TPAlign_right = 2,
};

// create TPrint object
// fout: FILE to write table to. Must be opened with write permissions. Can specify stdout / stderr
// borders: set to TRUE to draw inner and outer borders
// show_header: set to TRUE to display table header row
// spaces_left: spaces on the left side of the table
// spaces_between: spaces between columns
struct TPrint *tprint_create(FILE *fout, int show_borders, int show_header, int spaces_left, int spaces_between);

// destroy TPrint object
void tprint_free(struct TPrint *tprint);

// Append column to the table
// caption: label of the column, can be NULL
// caption_align: how to align column caption
// data_align: how to align data in the column
void tprint_column_add(struct TPrint *tprint, const char *caption, enum TPrintAlign caption_align, enum TPrintAlign data_align);

// set table format for double numbers
void tprint_set_double_fmt(struct TPrint *tprint, const char *fmt);

// set table format for int32 numbers
void tprint_set_int32_fmt(struct TPrint *tprint, const char *fmt);

void tprint_data_add_int32(struct TPrint *tprint, int col, int data);
void tprint_data_add_uint64(struct TPrint *tprint, int col, unsigned long long data);
void tprint_data_add_str(struct TPrint *tprint, int col, const char *data);
void tprint_data_add_double(struct TPrint *tprint, int col, double data);

// output table to the specified FILE
void tprint_print (struct TPrint *tprint);

#endif
