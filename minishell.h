/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboulakn <hboulakn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:58:02 by hboulakn          #+#    #+#             */
/*   Updated: 2025/05/15 15:53:40y hboulakn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <string.h>
#include "libft/libft.h"



typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE, // |
    TOKEN_REDIR_IN, //<
    TOKEN_REDIR_OUT, //>
    TOKEN_REDIR_APPEND, //>>
    TOKEN_REDIR_HEREDOC //<<
} t_token_type;

typedef enum quote_type
{
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} t_quote_type;

typedef struct s_token
{
    char *value;
    t_token_type type;
    t_quote_type quote_type;
    struct s_token *next;
} t_token;


typedef enum e_redir_type
{
    REDIR_INPUT, // <
    REDIR_OUTPUT, //>
    REDIR_APPEND,//>>
    REDIR_HEREDOC // <<
} t_redir_type;

typedef struct s_redir
{
    t_redir_type type;
    char *filename;
    struct s_redir *next;
} t_redir;

typedef struct s_command
{
    char **args;
    t_redir *redirs;
    struct s_command *next;
} t_command;

/*0-read_input.c*/
char *read_input(void);

/* 1-lexer.c*/
t_token *lexer(char *input);
int process_operator(char *input, int i, int operator_length, t_token **tokens);
t_token_type get_operator_type(char *operator_value);

/*2-lexer_utils.c*/
int is_whitespace(char c);
int get_operator_length(char *input, int i);

/*3-list_utils.c*/
t_token *create_token(char *value, t_token_type type);
t_token	*last_token(t_token *tokens);
void	add_token_back(t_token **tokens, t_token *new_token);
void free_token_list(t_token **tokens);

/*4-parsing_input.c*/

t_command *parse_tokens(t_token **tokens);
int handle_token(t_token **token, t_command **command);
int handle_pipe_token(t_command **command_ptr);
int handle_redirections(t_token **token, t_redir **redirs);
t_redir_type get_redirection_type(t_token_type type);
int handle_token_word(t_command *command, t_token *token);
int is_redirection(t_token_type type);
int add_redirection(t_redir **list, t_redir_type type, char *filename);
int add_arg_to_command(t_command *command, char *arg);
t_command *initialize_command_struct(void);
int ft_error(char *message);
void free_commands(t_command *command);
void free_redirections(t_redir *redir);
void free_args(char **args);

/*5-syntax_error.c*/
int check_syntax(t_token *tokens);

/*expansion.c*/
int expand_tokens(t_token *tokens);

#endif
