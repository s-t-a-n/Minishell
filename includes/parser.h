#ifndef PARSER_H
# define PARSER_H

#include "journal.h"
#include "execscheme.h"

typedef struct		s_parser
{
	t_execscheme	*scheme;
}					t_parser;

t_execscheme		*parse();

t_parser			*parser_create();
t_parser			*parser_destroy(t_parser **parser);

t_vector			parse_get_subtokens(t_token *first, t_token *last);
void				parse_replace_tokens_with_token(t_vector tokens, t_token *first, t_token *last, t_token *token);

void				parse_perform_string_substitution(t_vector tokens, t_token *first, t_token *last, t_vector subtokens);
t_bool				parse_expand_first_string(t_vector tokens, t_token *first, t_token *last);
t_bool				parse_expand_strings(e_token_type string_type);

char				*parse_retreive_var_from_env_for_token(t_token *token);
void				parse_perform_var_substitution(t_vector tokens, t_token *var_sym, t_token *var_name);
t_bool				parse_is_variable(t_token *var_name);
t_bool				parse_expand_first_variable(t_vector tokens, t_token *var_sym);
t_bool				parse_expand_variables();

t_bool				parse_should_expand_literals();
t_bool				parse_expand();

#endif
