/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1-lexer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboulakn <hboulakn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:31:51 by hboulakn          #+#    #+#             */
/*   Updated: 2025/04/30 18:14:16boulakn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int process_operator(char *input, int i, int operator_length, t_token **tokens)
{
    char *operator_value;
    t_token_type operator_type;
    t_token *new_token;

    operator_value = ft_substr(input, i, operator_length);
    if (!operator_value)
    {
        printf("Error: Unknown operator type for '%s'\n", operator_value);
        return (-1);
    }
    operator_type = get_operator_type(operator_value);
    if (operator_type == ((t_token_type)-1))
    {
        printf("Error: Allocation error in process_operator type for '%s'\n", operator_value);
        free(operator_value);
        return (-1);
    }
    new_token = create_token(operator_value, operator_type);
    if (!new_token)
    {
        printf("Error: Allocating error in process_operator (token)\n");
        free(operator_value);
        return (-1);
    }
    free(operator_value);
    add_token_back(tokens, new_token);
    return (i + operator_length);
}

int is_alpha(char c)
{
    return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}

int process_complex_word(char *input, int i, t_token **tokens)
{
    char quote;
    char *tmp;
    char *buffer;
    char *new_buffer;
    int start;
    t_token *token;
    t_quote_type quote_type;

    buffer = NULL;
    tmp = NULL;
    quote_type = NO_QUOTE;
    while (input[i] && !is_whitespace(input[i]) && !get_operator_length(input, i))
    {
        if (input[i] == '\'' || input[i] == '\"')
        {
            quote = input[i];
            if (quote == '\'')
                quote_type = SINGLE_QUOTE;
            else if (quote == '\"')
                quote_type = DOUBLE_QUOTE;
            i++;
            start = i;
            while (input[i] && input[i] != quote)
                i++;
            if (!input[i])
            {
                ft_putendl_fd("Syntax Error: Unclosed quote", 2);
                if (buffer)
                    free(buffer);
                return (-1);
            }
            tmp = ft_substr(input, start, i - start);
            i++;
        }
        else
        {
            start = i;
            while (input[i] && input[i] != '\"' && input[i] != '\'' && !is_whitespace(input[i]) && !get_operator_length(input, i))
                i++;
            tmp = ft_substr(input, start, i - start);
        }
        if (!tmp)
        {
            if (buffer)
                free(buffer);
            return (-1);
        }
        if (buffer == NULL)
        {
            buffer = tmp;
        }
        else
        {
            new_buffer = ft_strjoin(buffer, tmp);
            free(buffer);
            free(tmp);
            buffer = new_buffer;
            if (!buffer)
                return (-1);
        }
    }
    if (!buffer)
        return  (i);
    token = create_token(buffer, TOKEN_WORD);
    if (!token)
    {
        free(buffer);
        return (-1);
    }
    free(buffer);
    token->quote_type = quote_type;
    add_token_back(tokens, token);
    return (i);
}

t_token *lexer(char *input)
{
    t_token *tokens_list;
    int i;
    int operator_length;

    if (!input)
        return (NULL);
    i = 0;
    tokens_list = NULL;
    while (input[i])
    {
        while (is_whitespace(input[i]))
            i++;
        if (!input)
            break;
        operator_length = get_operator_length(input, i);
        if (operator_length > 0)
            i = process_operator(input, i, operator_length, &tokens_list);
        else
            i = process_complex_word(input, i, &tokens_list);
        if (i == -1)
        {
            free_token_list(&tokens_list);
            return (NULL);
        }
    }
    return (tokens_list);
}

//cc 0-read_input.c 1-lexer.c 2-lexer_utils.c 3-list_utils.c 4-string_utils.c -lreadline