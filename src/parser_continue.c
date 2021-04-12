#include <assert.h>
#include <stdlib.h>

#include "debugger.h"
#include "libft.h"

#include "parser.h"

extern t_parser	*g_parser__;

static void	parse_flush_tokens(t_range area)
{
	size_t		len;

	len = area.begin;
	while (len > 0)
	{
		journal_remove(0);
		len--;
	}
	journal_rebuild_tokens();
}

t_execscheme	*parse_generate_execschemes(void)
{
	t_execscheme	*root;
	t_execscheme	*scheme;

	root = NULL;
	scheme = parse_get_next_scheme();
	while (scheme)
	{
		if (!root)
		{
			root = scheme;
			scheme->rel_type[PREV_R] = REL_START;
		}
		else
			execscheme_attach(root, scheme);
		scheme = parse_get_next_scheme();
	}
	if (!root)
		dbg("Failed to build execschemes!\n", "");
	parse_flush_tokens(g_parser__->matcharea);
	return (root);
}

t_execscheme	*parse(t_range matcharea)
{
	if (range_cmp(matcharea, range(0, 0)) == 0)
		return (NULL);
	g_parser__->matcharea = matcharea;
	parse_dump_match_area(g_parser__->matcharea);
	if (!parse_expand())
	{
		dbg("Failed to expand!\n", "");
		return (NULL);
	}
	if (g_parser__->rootscheme)
		execscheme_destroy(&g_parser__->rootscheme);
	g_parser__->rootscheme = parse_generate_execschemes();
	return (g_parser__->rootscheme);
}

t_parser	*parser_create(void)
{
	t_parser	*parser;

	if (g_parser__)
		return (g_parser__);
	parser = ft_calloc(sizeof(t_parser), 1);
	if (parser)
	{
	}
	return ((g_parser__ = parser));
}

t_parser	*parser_destroy(t_parser **parser)
{
	if (!g_parser__)
		return (NULL);
	execscheme_destroy(&g_parser__->rootscheme);
	free(g_parser__);
	g_parser__ = NULL;
	if (parser)
		*parser = NULL;
	return ((g_parser__ = NULL));
}

void	parser_set_env(t_env *env)
{
	assert(g_parser__);
	assert(! g_parser__->env);
	g_parser__->env = env;
}
