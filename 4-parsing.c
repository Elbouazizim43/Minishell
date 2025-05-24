/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4-parsing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboulakn <hboulakn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:19:55 by hboulakn          #+#    #+#             */
/*   Updated: 2025/05/19 19:08:38by hboulakn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void free_args(char **args)
{
	int i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void free_redirections(t_redir *redir)
{
	t_redir *tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->filename)
			free(tmp->filename);
		free(tmp);
	}
}

void free_commands(t_command *command)
{
	t_command *tmp;

	while (command)
	{
		tmp = command;
		command = command->next;
		free_args(tmp->args);
		free_redirections(tmp->redirs);
		free(tmp);
	}
}

int ft_error(char *message)
{
	ft_putendl_fd(message, 2);
	return (-1);
}

t_command *initialize_command_struct(void)
{
	t_command *command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->redirs = NULL;
	command->next = NULL;
	return (command);
}

int add_arg_to_command(t_command *command, char *arg)
{
	int i;
	char **new_args;
	int j;

	i = 0;
	if (command->args)
		while(command->args[i])
			i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (0);
	j =  0;
	while (j < i)
	{
		new_args[j] = command->args[j];
		if (!new_args[j])
		{
			free_args(new_args);
			return (0);
		}
		j++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		free(new_args);
		return (0);
	}
	new_args[i + 1] = NULL;
	if (command->args)
		free(command->args);
	command->args = new_args;
	return (1);
}

int add_redirection(t_redir **list, t_redir_type type, char *filename)
{
	t_redir *new;
	t_redir *last;

	new = malloc(sizeof(t_redir));
	if (!new)
	{
		free(filename);
		return (0);
	}
	new->type = type;
	new->filename = ft_strdup(filename);
	if (!new->filename)
	{
		free(new);
		return (0);
	}
	new->next = NULL;
	if (*list == NULL)
		*list = new;
	else
	{
		last = *list;
		while (last->next)
			last = last->next;
		last->next = new;
	}
	return (1);
}

int is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC || type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT);
}


int handle_token_word(t_command *command, t_token *token)
{
	if (add_arg_to_command(command, token->value) == 0)
		return (0);
	return (1);
}

t_redir_type get_redirection_type(t_token_type type)
{
	if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUTPUT);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	if (type == TOKEN_REDIR_IN)
		return (REDIR_INPUT);
	return (REDIR_HEREDOC);
}

int handle_redirections(t_token **token, t_redir **redirs)
{
	t_redir_type type;
	char *filename;

	type = get_redirection_type((*token)->type);
	*token = (*token)->next;
	if  (!(*token) || (*token)->type != TOKEN_WORD)
		return (ft_error("Syntax error after redirection"));
	filename = ft_strdup((*token)->value);
	if (!filename || !add_redirection(redirs, type, filename))
		return (0);
	return (1);
}

int handle_pipe_token(t_command **command_ptr)
{
	t_command *new_command;

	new_command = initialize_command_struct();
	if (!new_command)
		return (0);
	(*command_ptr)->next = new_command;
	*command_ptr = new_command; //move the pointer to the next command
	return (1);
}

int handle_token(t_token **token, t_command **command)
{
	if (!token || !(*token))
		return (1);
	if ((*token)->type == TOKEN_WORD)
	{
		if (!handle_token_word(*command, *token))
			return (0);
	}
	else if (is_redirection((*token)->type))
	{
		if (!handle_redirections(token, &((*command)->redirs)))
			return (0);
	}
	else if ((*token)->type == TOKEN_PIPE)
	{
		if (!handle_pipe_token(command))
			return (0);
	}
	return (1);
}

t_command *parse_tokens(t_token **tokens)
{
	t_command *current_command;
	t_command *head;
	t_token *token;

	if (!tokens || !(*tokens))
		return (NULL);
	current_command = initialize_command_struct();
	if (!current_command)
		return (NULL);
	head = current_command;
	token = *tokens;
	while (token)
	{
		if (!handle_token(&token, &current_command))
		{
			free_commands(head);
			return (NULL);
		}
		token = token->next;
	}
	return (head);
}
