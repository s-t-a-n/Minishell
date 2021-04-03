#include <assert.h>
#include <stdlib.h>

#include "debugger.h"
#include "libft.h"

#include "minishell.h"
#include "shellcore.h"

extern char	**environ;

static t_shell	*g_shell__;

t_shellerr	shell_interactive(char **env)
{
	t_shellerr	err;

	if (g_shell__)
		return (SHELL_EXISTS);
	err = shell_init(env);
	if (err == SHELL_OK)
		return (_shell_loop(g_shell__));
	else
		return (err);
}

t_shellerr	shell_init(char **env)
{
	if (g_shell__)
		return (SHELL_EXISTS);
	if (env == NULL)
		g_shell__ = _shell_create(environ);
	else
		g_shell__ = _shell_create(env);
	_shell_register_sigint_handler();
	if (g_shell__ != NULL)
		return (SHELL_OK);
	else
		return (SHELL_ERRNO);
}

void	shell_deinit(void)
{
	if (g_shell__)
		_shell_destroy(&g_shell__);
}

int	shell_exec(const char *command_string)
{
	t_shell	*shell;
	int		rvalue;

	if (g_shell__)
	{
		return (_shell_exec(g_shell__, command_string));
	}
	else
	{
		shell = _shell_create(environ);
		rvalue = _shell_exec(g_shell__, command_string);
		_shell_destroy(&shell);
		return (rvalue);
	}
}
