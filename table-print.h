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


enum table_print_align_t
{
    table_print_align_left = 0,
    table_print_align_center,
    table_print_align_right,
};

// create table_print_t object
// fout: FILE to write table to. Must be opened with write permissions. Can specify stdout / stderr
// borders: set to TRUE to draw inner and outer borders
// show_header: set to TRUE to display table header row
// spaces_left: spaces on the left side of the table
// spaces_between: spaces between columns
struct table_print_t* table_print_create(FILE *fout, int show_borders, int show_header, int spaces_left, int spaces_between);

// destroy table_print_t object
void table_print_free(struct table_print_t *tp);

// Append column to the table
// caption: label of the column, can be NULL
// caption_align: how to align column caption
// data_align: how to align data in the column
void table_print_column_add(struct table_print_t *tp, const char *caption, enum table_print_align_t caption_align, enum table_print_align_t data_align);

// set table format for double numbers
void table_print_set_double_fmt(struct table_print_t *tp, const char *fmt);

// set table format for int32 numbers
void table_print_set_int32_fmt(struct table_print_t *tp, const char *fmt);

void table_print_data_add_int32(struct table_print_t *tp, int col, int data);
void table_print_data_add_uint64(struct table_print_t *tp, int col, unsigned long long data);
void table_print_data_add_str(struct table_print_t *tp, int col, const char *data);
void table_print_data_add_double(struct table_print_t *tp, int col, double data);

// output table to the specified FILE
void table_print_print(struct table_print_t *tp);



/* Private functions */

char* strdup_printf(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
struct table_print_column_t;
void table_print_column_free(struct table_print_column_t *col);

#endif
