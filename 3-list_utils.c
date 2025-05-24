/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3-list_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboulakn <hboulakn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:20:42 by hboulakn          #+#    #+#             */
/*   Updated: 2025/05/22 19:20:05 by hboulakn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *create_token(char *value, t_token_type type)
{
    t_token *new_token;
    t_quote_type quote_type;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return (NULL);
    new_token->value = ft_strdup(value);
    if (!new_token)
    {
        free(new_token);
        return (NULL);
    }
    new_token->type = type;
    new_token->quote_type = quote_type;
    new_token->next = NULL;
    return (new_token);
}

t_token	*last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens)
	{
		if (tokens->next == NULL)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

void	add_token_back(t_token **tokens, t_token *new_token)
{
	t_token	*last_tok;

	if (!tokens || !new_token)
		return ;
	if (*tokens)
	{
		last_tok = last_token(*tokens);
		last_tok->next = new_token;
	}
	else
		*tokens = new_token;
}

void free_token_list(t_token **tokens)
{
    t_token *current;
    t_token *next_node;

    if (!tokens || !*tokens)
        return;
    current = *tokens;
    while (current)
    {
        next_node = current->next;
        free(current->value);
        free(current);
        current = current->next;
    }
    *tokens = NULL;
}