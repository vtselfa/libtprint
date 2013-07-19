/*
 * Table Print utilities
 * Copyright (C) 2012-2013 Paul Ionkin <paul.ionkin@gmail.com>
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

#include "table-print.h"


int main()
{
    struct table_print_t *tp;
    double d1 = 40.488, d2 = 112.908 , d3 = 3.23;
    int i1 = 532, i2 = 3;
    int i;

    tp = table_print_create (stderr, TRUE, TRUE, 0, 4);

    table_print_column_add (tp, "", table_print_align_center, table_print_align_right);
    table_print_column_add (tp, "Align left", table_print_align_center, table_print_align_left);
    table_print_column_add (tp, "Align right", table_print_align_center, table_print_align_right);
    table_print_column_add (tp, "1", table_print_align_center, table_print_align_left);
    table_print_column_add (tp, "Align center", table_print_align_center, table_print_align_center);

    /*
    table_print_column_add (tp, "test 2", table_print_align_center, table_print_align_left);
    table_print_column_add (tp, "", table_print_align_center, table_print_align_right);
    table_print_column_add (tp, "test 4", table_print_align_center, table_print_align_center);
    */
    for (i = 0; i < 20; i++)
	{
        table_print_data_add_str(tp, 0, "test");
        d1 *= 2;
        table_print_data_add_double(tp, 1, d1);
        d2 *= 3;
        table_print_data_add_double(tp, 2, d2);

        i1 *= 2;
        table_print_data_add_int32(tp, 3, i1);

        i2 *= 3;
        table_print_data_add_int32(tp, 4, i2);

        table_print_data_add_str(tp, 5, "test2");

        d3 *= 3;
        table_print_data_add_double(tp, 6, d3);

        table_print_data_add_str(tp, 7, "test3");
    }

    table_print_print(tp);
    table_print_free(tp);

    return 0;
}
