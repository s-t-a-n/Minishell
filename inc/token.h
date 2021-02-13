#ifndef TOKEN_H
# define TOKEN_H

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

typedef enum		s_token_type
{
					WORD,
					STRING,
					VARIABLE,
					SPACE,
					PIPE,
					NEWLINE,
					TOKEN_TYPE_INDEX
}					e_token_type;

typedef struct		s_range
{
	size_t			begin;
	size_t			end;
}					t_range;

typedef struct		s_token
{
	e_token_type	type;
	t_range			range;
}					t_token;

/*
t_bool      token_find_in_str(const char *str, size_t begin, size_t len);
*/

t_bool				token_cmp(const t_token *a, const t_token *b);

t_token				*token_create();
t_token				*token_destroy(t_token *token);

#endif
