/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboulakn <hboulakn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:50:13 by hboulakn          #+#    #+#             */
/*   Updated: 2025/05/22 19:58:14 by hboulakn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int contains_dollar(char *str)
{
	while (*str)
	{
		if (*str == '$')
			return (1);
		str++;
	}
	return (0);
}

char *expand_variables(char *str, int *i)
{
	int start;
	int var_len;
	char *var_name;
	char *var_value;
	char *value;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(10));
	}
	var_len = 0;
	while (str[*i + var_len] && (ft_isalnum(str[*i + var_len]) || str[*i + var_len] == '_'))
		var_len++;
	var_name = ft_substr(str, start, var_len);
	if (!var_name)
		return (NULL);
	var_value = getenv(var_name);
	if (var_value)
		value = ft_strdup(var_value);
	else
		value = ft_strdup("");
	if (!value)
		return (free(var_name), NULL);
	free(var_name);
	*i += var_len;
	return (value);
}

char *perform_expansion(char *input, t_quote_type quote_type)
{
	int i;
	char *result;
	char *var_value;
	char *tmp;
	char buffer[2];

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while(input[i])
	{
		if (input[i] == '$' && quote_type != SINGLE_QUOTE)
		{
			i++;
			var_value = expand_variables(input, &i);
			if (!var_value)
				return (free(result), NULL);
			tmp = ft_strjoin(result, var_value);
			free(var_value);
			if (!tmp)
				return (free(result), NULL);
			free(result);
			result = tmp;
		}
		else
		{
			buffer[0] = input[i++];
			buffer[1] = '\0';
			tmp = ft_strjoin(result, buffer);
			if (!tmp)
				return (free(result), NULL);
			free(result);
			result = tmp;
		}
	}
	return (result);
}

int expand_tokens(t_token *tokens)
{
	char *expanded;

	while (tokens)
	{
		if (tokens->quote_type != SINGLE_QUOTE && contains_dollar(tokens->value))
		{
			expanded = perform_expansion(tokens->value, tokens->quote_type);
			if (!expanded)
				return (0);
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
	return (1);
}
