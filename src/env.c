#include <stdlib.h>
#include <assert.h>

#include "libft.h"
#include "vector.h"

#include "env.h"

#define HASHMAP_SIZE 1024

t_env	*env_create(const char **environ)
{
	t_env	*this_env;

	assert(environ);
	this_env = ft_calloc(sizeof(t_env), 1);
	if (this_env)
	{
		this_env->hm_store = env_bootstrap_from_environ(environ);
		if (!this_env->hm_store)
			return (env_destroy(&this_env));
	}
	return (this_env);
}

t_env	*env_destroy(t_env **env)
{
	if (env)
	{
		if ((*env)->hm_store)
			hm_destroy((*env)->hm_store, env_node_destroy_hm);
		free(*env);
		*env = NULL;
	}
	return (NULL);
}

t_pair		*env_pair_create_from_line(const char *line)
{
	t_pair	*pair;
	char	**split;

	assert(line);
	split = ft_strsplit(line, '=');
	if (!split || ft_arraylen((const void **)split) != 2)
	{
		ft_array_destroy((void **)split, ft_arraylen((const void **)split));
		return (NULL);
	}
	pair = pair_create(split[0], split[1]);
	free(split);
	return (pair);
}

t_env_node	*env_node_create_from_line(const char *line)
{
	t_env_node	*node;
	int			equal_sign_index;
	char		*key;
	char		*value;

	assert(line);

	equal_sign_index = ft_strclen(line, '=');
	key = ft_strsub(line, 0, equal_sign_index);
	value = ft_strchr(line, '=') + 1;

	assert(key);
	assert(value);
	node = env_node_create(key, value, SCOPE_ENVIRON);
	assert(node);

	if (!key || !value || !node)
	{
		env_node_destroy(&node);
		node = NULL;
	}
	free(key);
	return (node);
}

t_bool	env_insert_from_line(const char *envline, void *hm_store)
{
	t_env_node	*node;

	assert(envline);
	assert(hm_store);
	node = env_node_create_from_line(envline);
	if (!node || !hm_set(hm_store, node->key, node))
		return (FALSE);
	return (TRUE);
}

void	*env_bootstrap_from_environ(const char **env)
{
	void		*hm_store;
	int		index;

	assert(env);
	hm_store = hm_new(HASHMAP_SIZE);
	if (!hm_store)
		return (NULL);
	index = 0;
	while(env[index])
	{
		if (!env_insert_from_line(env[index], hm_store))
		{
			hm_destroy(hm_store, env_node_destroy_hm);
			return (NULL);
		}
		index++;
	}
	return (hm_store);
}

char	*env_get(t_env *env, const char *key)
{
	t_env_node	*node;

	assert(env);
	assert(key);
	node = env_get_node_for_key(env, key);
	return ((node != NULL)? node->value : NULL);
}

t_bool	env_set(t_env *env, const char *key, char *value, e_scope scope)
{
	assert(env);
	assert(key);
	assert(value);

	t_env_node	*node;

	node = env_node_create(key, value, scope);
	if (node && hm_set(env->hm_store, key, node))
		return (TRUE);
	env_node_destroy(&node);
	return (FALSE);
}

t_env_node	*env_get_node_for_key(t_env *env, const char *key)
{
	assert(env);
	assert(key);
	return ((t_env_node *)hm_get(env->hm_store, key));
}

t_bool	env_unset(t_env *env, const char *key)
{
	assert(env);
	assert(key);
	if (!env_get_node_for_key(env, key))
		return (FALSE);
	hm_remove(env->hm_store, key, env_node_destroy_hm);
	return (TRUE);
}

size_t	env_size(t_env *env)
{
	return (hm_size(env->hm_store));
}

t_env_node	*get_next_env_node(t_env *env)
{
	t_pair pair;

	pair = hm_get_seq(env->hm_store);
	return ((t_env_node *)pair.s.value);
}

char	**env_to_array(t_env *env, e_scope scope)
{
	char		**environ;
	size_t		index;
	size_t		count;
	t_env_node	*node;
	
	hm_get_seq(NULL); /* resets internal static */
	environ = (char **)ft_calloc(sizeof(char *), env_size(env) + 1);
	if (!environ)
		return (NULL);
	index = 0;
	count = 0;
	while (count < env_size(env))
	{
		node = get_next_env_node(env);
		assert(node);
		if(node->scope == scope)
		{
			environ[index] = ft_strjoin_multi(3, node->key, "=", node->value);
			node->environ_index = index;
			index++;
		}
		count++;
	}
	return (environ);
}
