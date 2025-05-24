/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2-lexer_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboulakn <hboulakn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:38:05 by hboulakn          #+#    #+#             */
/*   Updated: 2025/05/15 15:57:26 by hboulakn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}

int get_operator_length(char *input, int i)
{
    if (!input)
        return (-1);
    if (input[i] == '|')
        return (1);
    if (input[i] == '>')
    {
        if (input[i + 1] == '>')
            return (2);
        return (1);
    }
    if (input[i] == '<')
    {
        if (input[i + 1] == '<')
            return (2);
        return (1);
    }
    return (0);
}


t_token_type get_operator_type(char *operator_value)
{
    if (ft_strncmp(operator_value, "|", 2) == 0) return (TOKEN_PIPE);
    if (ft_strncmp(operator_value, "<", 2) == 0) return (TOKEN_REDIR_IN);
    if (ft_strncmp(operator_value, ">", 2) == 0) return (TOKEN_REDIR_OUT);
    if (ft_strncmp(operator_value, "<<", 3) == 0) return (TOKEN_REDIR_HEREDOC);
    if (ft_strncmp(operator_value, ">>", 3) == 0) return (TOKEN_REDIR_APPEND);
    return (-1);
}
