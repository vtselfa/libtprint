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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "linked-list.h"
#include "table-print.h"


struct table_print_t
{
	FILE *fout;
	struct list_t *columns;
	int rows;

	int spaces_left;
	int spaces_between;
	int show_borders;
	int show_header;
	int min_column_width;
};


struct table_print_column_t
{
	char *caption;
	int max_width;
	enum table_print_align_t caption_align;
	enum table_print_align_t data_align;
	struct list_t *data; // list of strings
};


char* strdup_printf(const char *fmt, ...)
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


struct list_t *str_token_list_create(const char *str, char *delim)
{
	struct list_t *token_list;
	char *token;
	char *tmp;

	/* Create list */
	token_list = list_create();

	/* Split string into tokens */
	tmp = strdup(str);
	token = strtok(tmp, delim);
	while (token)
	{
		/* Insert in token list */
		token = strdup(token);
		list_add(token_list, token);

		/* Next token */
		token = strtok(NULL, delim);
	}

	/* Free copy of 'str' */
	free(tmp);

	/* Return token list */
	return token_list;
}


void table_print_column_add(struct table_print_t *tp, const char *caption, enum table_print_align_t caption_align, enum table_print_align_t data_align)
{
	struct table_print_column_t *col;

	col = calloc(1, sizeof(struct table_print_column_t));
	col->data = list_create();
	if (tp->show_header)
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

	list_add(tp->columns, col);
}


void table_print_column_free(struct table_print_column_t *col)
{
	int i;
	LIST_FOR_EACH(col->data, i)
	{
		char *str = list_get(col->data, i);
		free(str);
	}
	list_free(col->data);
	if (col->caption)
		free(col->caption);
	free(col);
}


struct table_print_t* table_print_create(FILE *fout, int show_borders, int show_header, int spaces_left, int spaces_between, int min_column_width)
{
	struct table_print_t *tp;

	tp = calloc(1, sizeof(struct table_print_t));
	tp->fout = fout;
	tp->spaces_left = spaces_left;
	tp->spaces_between = spaces_between;
	tp->show_borders = show_borders;
	tp->show_header = show_header;
	tp->min_column_width = min_column_width;
	tp->columns = list_create();

	return tp;
}


void table_print_free(struct table_print_t *tp)
{
	int column;
	LIST_FOR_EACH(tp->columns, column)
	{
		struct table_print_column_t *c = list_get(tp->columns, column);
		table_print_column_free(c);
	}

	list_free(tp->columns);
	free(tp);
}


void table_print_add_row(struct table_print_t *tp, const char* fmt, ...)
{
	int column;
	int num_args;
	char *tmp;
	va_list args;
	struct list_t *tokens;

	va_start(args, fmt);
	temp = strdup_printf()
	va_end(args);

	tokens = str_token_list_create(tmp, "\n");

	if (list_count(tokens) > list_count(tp->columns))
		fatal("The number of items to add to the table is greater than the number of columns");

	LIST_FOR_EACH(tokens, column)
	{
		char *data = list_get(tp->columns, column);
		if (data)
			column_add_str(column, data);
	}

	list_free(tokens);
	free(tmp);
}


void table_print_add_to_column(struct table_print_t *tp, int column, const char* fmt, ...)
{
	int len;
	int num_args;
	char *tmp;
	va_list args;
	struct list_t *tokens;

	va_start(args, fmt);
	temp = strdup_printf()
	va_end(args);

	if (tmp)
		column_add_str(column, tmp);
	free(tmp);
}


void table_print_print_no_borders(struct table_print_t *tp)
{
	int spaces_left;
	int first = TRUE;
	int column;

	if (tp->show_header)
	{
		LIST_FOR_EACH(tp->columns, column)
		{
			struct table_print_column_t *col = list_get(tp->columns, column);

			if (first)
			{
				spaces_left = tp->spaces_left;
				first = FALSE;
			}
			else
				spaces_left = tp->spaces_between;

			if (col->caption_align == table_print_align_left)
				fprintf (tp->fout, "%*s%-*s", spaces_left, "", col->max_width, col->caption);
			else if (col->caption_align == table_print_align_center)
				fprintf(tp->fout, "%*s%-*s", spaces_left + (col->max_width - (int) strlen(col->caption)) / 2, "", col->max_width - (col->max_width - (int) strlen(col->caption)) / 2, col->caption);
			else
				fprintf (tp->fout, "%*s%*s", spaces_left, "", col->max_width, col->caption);

			int count = list_count(col->data);
			if (tp->rows < count)
				tp->rows = count;

		}
		fprintf(tp->fout, "\n");
	}
	else
	{
		LIST_FOR_EACH(tp->columns, column)
		{
			struct table_print_column_t *col = list_get(tp->columns, column);
			int count = list_count(col->data);
			if (tp->rows < count)
				tp->rows = count;
		}
	}

	for (int row = 0; row < tp->rows; row++)
	{
		first = TRUE;
		LIST_FOR_EACH(tp->columns, column)
		{
			struct table_print_column_t *col = list_get(tp->columns, column);
			char *cell;
			cell = list_get(col->data, column);

			if (first)
			{
				spaces_left = tp->spaces_left;
				first = FALSE;
			}
			else
				spaces_left = tp->spaces_between;

			if (col->data_align == table_print_align_left)
				fprintf(tp->fout, "%*s%-*s", spaces_left, "", col->max_width, cell);
			else if (col->data_align == table_print_align_center)
				fprintf(tp->fout, "%*s%-*s", spaces_left + (col->max_width - (int) strlen(cell)) / 2, "", col->max_width - (col->max_width - (int) strlen(cell)) / 2, cell);
			else
				fprintf(tp->fout, "%*s%*s", spaces_left, "", col->max_width, cell);
		}
		fprintf(tp->fout, "\n");
	}
}


void table_print_print_with_borders(struct table_print_t *tp)
{
	int first = TRUE;
	int spaces_left;
	int full_width = 0;
	char *str;
	int i;

	if (tp->show_borders)
	{
		LIST_FOR_EACH(tp->columns, column)
		{
			struct table_print_column_t *col = list_get(tp->columns, column);
			full_width += (col->max_width + tp->spaces_between);
		}
	}

	full_width += list_count(tp->columns);
	full_width -= 1;

	if (tp->show_header)
	{
		str = calloc(full_width + 1, sizeof(char));
		for (i = 0; i < full_width; i++)
			str[i] = '=';
		str[i] = '\0';

		fprintf(tp->fout, "%*s%s\n", tp->spaces_left + 1, "", str);
		free(str);

		fprintf(tp->fout, "%*s", tp->spaces_left, "");
		LIST_FOR_EACH(tp->columns)
		{
			struct table_print_column_t *col = list_get(tp->columns);
			int count;

			if (first)
				first = FALSE;
			spaces_left = tp->spaces_between;

			if (col->caption_align == table_print_align_left)
				fprintf(tp->fout, "|%*s%-*s%*s", spaces_left / 2, "", col->max_width, col->caption, spaces_left / 2, "");
			else if (col->caption_align == table_print_align_center)
				fprintf(tp->fout, "|%*s%s%*s", spaces_left / 2 +
						(col->max_width - (int) strlen(col->caption)) / 2, "",
						col->caption, col->max_width - (int) strlen (col->caption) -
						(col->max_width - (int) strlen(col->caption)) / 2 + spaces_left / 2, "");
			else
				fprintf (tp->fout, "|%*s%*s%*s", spaces_left / 2, "", col->max_width, col->caption, spaces_left / 2, "");

			count = list_count(col->data);
			if (tp->rows < count)
				tp->rows = count;
		}
		fprintf(tp->fout, "|\n");
	}
	else
	{
		LIST_FOR_EACH(tp->columns)
		{
			struct table_print_column_t *col = list_get(tp->columns);
			int count = list_count(col->data);
			if (tp->rows < count)
				tp->rows = count;
		}
	}

	str = calloc(full_width + 1, sizeof(char));
	for (i = 0; i < full_width; i++)
		str[i] = '=';
	str[i] = '\0';

	fprintf(tp->fout, "%*s%s\n", tp->spaces_left + 1, "", str);
	free(str);

	for (int row = 0; row < tp->rows; row++) {
		first = TRUE;
		fprintf(tp->fout, "%*s", tp->spaces_left, "");
		LIST_FOR_EACH(tp->columns)
		{
			struct table_print_column_t *col = list_get(tp->columns);
			char *cell;
			list_goto(col->data, row);
			cell = list_get(col->data);

			if (first)
				first = FALSE;

			spaces_left = tp->spaces_between;

			if (col->data_align == table_print_align_left)
				fprintf(tp->fout, "|%*s%-*s%*s",
						spaces_left / 2, "",
						col->max_width, cell,
						spaces_left / 2, ""
					   );
			else if (col->data_align == table_print_align_center)
				fprintf(tp->fout, "|%*s%s%*s",
						spaces_left / 2 +  (col->max_width - (int) strlen (cell)) / 2, "", cell,
						col->max_width - (int) strlen(cell) - (col->max_width - (int) strlen(cell)) / 2 + spaces_left / 2, ""
					   );
			else
				fprintf(tp->fout, "|%*s%*s%*s",
						spaces_left / 2, "",
						col->max_width, cell,
						spaces_left / 2, ""
					   );
		}
		fprintf(tp->fout, "|\n");
	}

	str = calloc(full_width + 1, sizeof(char));
	for (i = 0; i < full_width; i++)
		str[i] = '=';
	str[i] = '\0';

	fprintf(tp->fout, "%*s%s\n", tp->spaces_left + 1, "", str);
	free(str);
}


void table_print_print(struct table_print_t *tp)
{
	if (tp->show_borders)
		table_print_print_with_borders(tp);
	else
		table_print_print_no_borders(tp);
}
