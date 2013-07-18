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

#include "tprint.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "linked-list.h"


struct TPrint {
    FILE *fout;
    struct linked_list_t *l_columns;
    int rows;

    char *fmt_int32;
    char *fmt_uint64;
    char *fmt_str;
    char *fmt_double;

    int spaces_left;
    int spaces_between;
    int show_borders;
    int show_header;
};

struct TPrintColumn {
    char *caption;
    int max_width;
    enum TPrintAlign caption_align;
    enum TPrintAlign data_align;
    struct linked_list_t *l_data; // list of strings
};

static void tprint_column_free (struct TPrintColumn *col);


char *strdup_printf(const char *fmt, ...)
{
    char *temp;
    int len;
    va_list args;

    va_start (args, fmt);
    len = vsnprintf (NULL, 0, fmt, args);
    va_end (args);

    temp = calloc(len + 1, sizeof(char));

    va_start (args, fmt);
    vsnprintf (temp, len+1, fmt, args);
    va_end (args);

    return temp;
}






struct TPrint* tprint_create(FILE *fout, int show_borders, int show_header, int spaces_left, int spaces_between)
{
    struct TPrint *tprint;

    tprint = calloc(1, sizeof(struct TPrint));
    tprint->fout = fout;
    tprint->spaces_left = spaces_left;
    tprint->spaces_between = spaces_between;
    tprint->show_borders = show_borders;
    tprint->show_header = show_header;
	tprint->l_columns = linked_list_create();

	tprint->fmt_int32 = strdup("%d");
    tprint->fmt_uint64 = strdup("%lu");
    tprint->fmt_str = strdup("%s");
    tprint->fmt_double = strdup("%0.3f");

    return tprint;
}

void tprint_free(struct TPrint *tprint)
{
    free(tprint->fmt_int32);
    free(tprint->fmt_uint64);
    free(tprint->fmt_str);
    free(tprint->fmt_double);

	LINKED_LIST_FOR_EACH(tprint->l_columns)
	{
        struct TPrintColumn *col = linked_list_get(tprint->l_columns);
        tprint_column_free(col);
	}

    linked_list_free(tprint->l_columns);
    free(tprint);
}

void tprint_set_double_fmt(struct TPrint *tprint, const char *fmt)
{
    if(tprint->fmt_double)
        free(tprint->fmt_double);
    tprint->fmt_double = strdup(fmt);
}

void tprint_set_int32_fmt (struct TPrint *tprint, const char *fmt)
{
    if(tprint->fmt_int32)
        free(tprint->fmt_int32);
    tprint->fmt_int32 = strdup(fmt);
}

static void column_add_str(struct TPrintColumn *column, char *str)
{
    if (column->max_width < strlen(str))
        column->max_width = strlen(str);

    linked_list_add(column->l_data, str);
}

void tprint_column_add(struct TPrint *tprint, const char *caption, enum TPrintAlign caption_align, enum TPrintAlign data_align)
{
    struct TPrintColumn *col;

    col = calloc(1, sizeof(struct TPrintColumn));
	col->l_data = linked_list_create();
    if (tprint->show_header)
	{
        col->caption = strdup(caption);
        col->max_width = strlen(caption);
    }
	else
	{
        col->caption = NULL;
        col->max_width = 0;
    }
    col->caption_align = caption_align;
    col->data_align = data_align;

    linked_list_add(tprint->l_columns, col);
}

void tprint_column_free(struct TPrintColumn *col)
{
	LINKED_LIST_FOR_EACH(col->l_data)
	{
        char *str = linked_list_get(col->l_data);
        free(str);
    }
    linked_list_free(col->l_data);
	if (col->caption)
        free(col->caption);
    free (col);
}

// convert data to string
void tprint_data_add_int32 (struct TPrint *tprint, int col, int data)
{
    struct TPrintColumn *column;
    char *str;

    linked_list_goto(tprint->l_columns, col);
    column = linked_list_get(tprint->l_columns);
    if (!column)
        return;
    str = strdup_printf(tprint->fmt_int32, data);
    column_add_str(column, str);
}

void tprint_data_add_uint64 (struct TPrint *tprint, int col, unsigned long long data)
{
    struct TPrintColumn *column;
    char *str;

    linked_list_goto(tprint->l_columns, col);
    column = linked_list_get(tprint->l_columns);
    if (!column)
        return;
    str = strdup_printf(tprint->fmt_uint64, data);
    column_add_str(column, str);
}

void tprint_data_add_str(struct TPrint *tprint, int col, const char *data)
{
    struct TPrintColumn *column;
    char *str;

    linked_list_goto(tprint->l_columns, col);
    column = linked_list_get(tprint->l_columns);
    if (!column)
        return;
    str = strdup_printf(tprint->fmt_str, data);
    column_add_str (column, str);
}

void tprint_data_add_double(struct TPrint *tprint, int col, double data)
{
    struct TPrintColumn *column;
    char *str;

    linked_list_goto(tprint->l_columns, col);
    column = linked_list_get(tprint->l_columns);
    if (!column)
        return;
    str = strdup_printf(tprint->fmt_double, data);
    column_add_str(column, str);
}

void tprint_print_no_borders(struct TPrint *tprint)
{
    int spaces_left;
    int first = TRUE;

    if (tprint->show_header)
	{
		LINKED_LIST_FOR_EACH(tprint->l_columns)
		{
            struct TPrintColumn *col = linked_list_get(tprint->l_columns);

            if (first)
			{
                spaces_left = tprint->spaces_left;
                first = FALSE;
            }
			else
                spaces_left = tprint->spaces_between;

            if (col->caption_align == TPAlign_left)
                fprintf (tprint->fout, "%*s%-*s", spaces_left, "", col->max_width, col->caption);
            else if (col->caption_align == TPAlign_center)
                fprintf(tprint->fout, "%*s%-*s", spaces_left + (col->max_width - (int) strlen(col->caption)) / 2, "", col->max_width - (col->max_width - (int) strlen(col->caption)) / 2, col->caption);
            else
                fprintf (tprint->fout, "%*s%*s", spaces_left, "", col->max_width, col->caption);

            int count = linked_list_count(col->l_data);
			if (tprint->rows < count)
                tprint->rows = count;

        }
        fprintf(tprint->fout, "\n");
    }
	else
	{
		LINKED_LIST_FOR_EACH(tprint->l_columns)
		{
            struct TPrintColumn *col = linked_list_get(tprint->l_columns);
            int count = linked_list_count(col->l_data);
			if (tprint->rows < count)
                tprint->rows = count;
        }
    }

    for (int row = 0; row < tprint->rows; row++)
	{
        first = TRUE;
		LINKED_LIST_FOR_EACH(tprint->l_columns)
		{
            struct TPrintColumn *col = linked_list_get(tprint->l_columns);
            char *cell;
			linked_list_goto(col->l_data, row);
            cell = linked_list_get(col->l_data);

            if (first)
			{
                spaces_left = tprint->spaces_left;
                first = FALSE;
            }
			else
                spaces_left = tprint->spaces_between;

            if (col->data_align == TPAlign_left)
                fprintf(tprint->fout, "%*s%-*s", spaces_left, "", col->max_width, cell);
            else if (col->data_align == TPAlign_center)
                fprintf(tprint->fout, "%*s%-*s", spaces_left + (col->max_width - (int) strlen(cell)) / 2, "", col->max_width - (col->max_width - (int) strlen(cell)) / 2, cell);
            else
                fprintf(tprint->fout, "%*s%*s", spaces_left, "", col->max_width, cell);
        }
        fprintf(tprint->fout, "\n");
    }
}

void tprint_print_with_borders(struct TPrint *tprint)
{
    int first = TRUE;
    int spaces_left;
    int full_width = 0;
    char *str;
    int i;

    if (tprint->show_borders)
	{
		LINKED_LIST_FOR_EACH(tprint->l_columns)
		{
            struct TPrintColumn *col = linked_list_get(tprint->l_columns);
            full_width += (col->max_width + tprint->spaces_between);
        }
    }

    full_width += linked_list_count(tprint->l_columns);
    full_width -= 1;

    if (tprint->show_header)
	{
        str = calloc(full_width + 1, sizeof(char));
        for (i = 0; i < full_width; i++)
            str[i] = '=';
        str[i] = '\0';

        fprintf(tprint->fout, "%*s%s\n", tprint->spaces_left + 1, "", str);
        free(str);

        fprintf(tprint->fout, "%*s", tprint->spaces_left, "");
		LINKED_LIST_FOR_EACH(tprint->l_columns)
		{
            struct TPrintColumn *col = linked_list_get(tprint->l_columns);
			int count;

            if (first)
                first = FALSE;
            spaces_left = tprint->spaces_between;

            if (col->caption_align == TPAlign_left)
                fprintf(tprint->fout, "|%*s%-*s%*s", spaces_left / 2, "", col->max_width, col->caption, spaces_left / 2, "");
            else if (col->caption_align == TPAlign_center)
				fprintf(tprint->fout, "|%*s%s%*s", spaces_left / 2 +
					(col->max_width - (int) strlen(col->caption)) / 2, "",
					col->caption, col->max_width - (int) strlen (col->caption) -
					(col->max_width - (int) strlen(col->caption)) / 2 + spaces_left / 2, "");
            else
                fprintf (tprint->fout, "|%*s%*s%*s", spaces_left / 2, "", col->max_width, col->caption, spaces_left / 2, "");

			count = linked_list_count(col->l_data);
            if (tprint->rows < count)
                tprint->rows = count;
        }
        fprintf(tprint->fout, "|\n");
    }
	else
	{
		LINKED_LIST_FOR_EACH(tprint->l_columns)
		{
            struct TPrintColumn *col = linked_list_get(tprint->l_columns);
			int count = linked_list_count(col->l_data);
            if (tprint->rows < count)
                tprint->rows = count;
        }
    }

    str = calloc(full_width + 1, sizeof(char));
    for (i = 0; i < full_width; i++)
        str[i] = '=';
    str[i] = '\0';

    fprintf(tprint->fout, "%*s%s\n", tprint->spaces_left + 1, "", str);
    free(str);

    for (int row = 0; row < tprint->rows; row++) {
        first = TRUE;
        fprintf(tprint->fout, "%*s", tprint->spaces_left, "");
		LINKED_LIST_FOR_EACH(tprint->l_columns)
		{
            struct TPrintColumn *col = linked_list_get(tprint->l_columns);
            char *cell;
			linked_list_goto(col->l_data, row);
            cell = linked_list_get(col->l_data);

            if (first)
                first = FALSE;

            spaces_left = tprint->spaces_between;

            if (col->data_align == TPAlign_left)
                fprintf(tprint->fout, "|%*s%-*s%*s",
                    spaces_left / 2, "",
                    col->max_width, cell,
                    spaces_left / 2, ""
                );
			else if (col->data_align == TPAlign_center)
                fprintf(tprint->fout, "|%*s%s%*s",
                    spaces_left / 2 +  (col->max_width - (int) strlen (cell)) / 2, "", cell,
                    col->max_width - (int) strlen(cell) - (col->max_width - (int) strlen(cell)) / 2 + spaces_left / 2, ""
                );
			else
                fprintf(tprint->fout, "|%*s%*s%*s",
                    spaces_left / 2, "",
                    col->max_width, cell,
                    spaces_left / 2, ""
                );
        }
        fprintf(tprint->fout, "|\n");
    }

    str = calloc(full_width + 1, sizeof(char));
    for (i = 0; i < full_width; i++)
        str[i] = '=';
    str[i] = '\0';

    fprintf(tprint->fout, "%*s%s\n", tprint->spaces_left + 1, "", str);
    free(str);
}

void tprint_print(struct TPrint *tprint)
{
    if (tprint->show_borders)
        tprint_print_with_borders(tprint);
    else
        tprint_print_no_borders(tprint);
}
