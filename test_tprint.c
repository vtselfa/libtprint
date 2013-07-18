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
#include "tprint.h"

int main ()
{
    struct TPrint *tp;
    double d1 = 40.488, d2 = 112.908 , d3 = 3.23;
    int i1 = 532, i2 = 3;
    int i;

    tp = tprint_create (stderr, TRUE, TRUE, 0, 4);

    tprint_column_add (tp, "", TPAlign_center, TPAlign_right);
    tprint_column_add (tp, "Align left", TPAlign_center, TPAlign_left);
    tprint_column_add (tp, "Align right", TPAlign_center, TPAlign_right);
    tprint_column_add (tp, "1", TPAlign_center, TPAlign_left);
    tprint_column_add (tp, "Align center", TPAlign_center, TPAlign_center);
    /*
    tprint_column_add (tp, "test 2", TPAlign_center, TPAlign_left);
    tprint_column_add (tp, "", TPAlign_center, TPAlign_right);
    tprint_column_add (tp, "test 4", TPAlign_center, TPAlign_center);
    */
    for (i = 0; i < 20; i++) {
        tprint_data_add_str (tp, 0, "test");
        d1 *= 2;
        tprint_data_add_double (tp, 1, d1);
        d2 *= 3;
        tprint_data_add_double (tp, 2, d2);

        i1 *= 2;
        tprint_data_add_int32 (tp, 3, i1);

        i2 *= 3;
        tprint_data_add_int32 (tp, 4, i2);

        tprint_data_add_str (tp, 5, "test2");

        d3 *= 3;
        tprint_data_add_double (tp, 6, d3);

        tprint_data_add_str (tp, 7, "test3");
    }

    tprint_print (tp);

    tprint_free (tp);

    return 0;
}
