/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5-syntax_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboulakn <hboulakn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:13:36 by hboulakn          #+#    #+#             */
/*   Updated: 2025/05/20 19:47:35 by hboulakn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int check_syntax(t_token *tokens)
{
	int found_command;

	if (!tokens)
		return (1);
	if (tokens->type == TOKEN_PIPE)
		return (ft_error("Syntax error: unexpected token '|' at start"));
	found_command = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			if (!tokens->next)
				return (ft_error("Syntax error: unexpected token '|' at end"));
			if (tokens->next->type == TOKEN_PIPE)
				return (ft_error("Syntax error: consecutive '|'"));
			if (!found_command)
				return (ft_error("Syntax error: empty command before '|'"));
			found_command = 0;
		}
		else if (is_redirection(tokens->type))
		{
			if (!tokens->next || tokens->next->type != TOKEN_WORD)
				return (ft_error("Syntax error: redirection missign filename"));
		}
		else if (tokens->type == TOKEN_WORD)
			found_command = 1;
		tokens = tokens->next;
	}
	return (1);
}