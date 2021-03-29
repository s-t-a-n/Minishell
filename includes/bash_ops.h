#ifndef BASH_OPS
# define BASH_OPS

#include "command.h"

int	builtin_echo(t_command *cmd);
int	builtin_exit(t_command *cmd);
int	builtin_unset(t_command *cmd);
int	builtin_export(t_command *cmd);
int	builtin_cd(t_command *cmd);
int	builtin_pwd(t_command *cmd);
int	builtin_env(t_command *cmd);

int	exec_bin(t_command *cmd);
int op_assignment(t_command *cmd);

#endif
