/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0-read_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboulakn <hboulakn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:24:45 by hboulakn          #+#    #+#             */
/*   Updated: 2025/05/22 18:24:49 by hboulakn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>

#define GREEN "\001\033[0;32m\002"
#define RESET "\001\033[0m\002"

char *read_input(void)
{
    char *line;

    line = readline(GREEN "minishell> " RESET);
    if (!line || strncmp(line, "exit", 5) == 0)
    {
        printf("Exit...\n");
        exit(0);
    }
    if (*line != '\0')
        add_history(line);
    return (line);
}

void print_tokens(t_token *list)
{
    printf("--- Tokens ---\n");
    int pos = 0;
    t_token *current = list;

    while (current)
    {
        printf("Token %d: Type=", pos);
        switch (current->type)
        {
            case TOKEN_WORD: printf("WORD"); break;
            case TOKEN_PIPE: printf("PIPE"); break;
            case TOKEN_REDIR_IN: printf("REDIR_IN"); break;
            case TOKEN_REDIR_OUT: printf("REDIR_OUT"); break;
            case TOKEN_REDIR_APPEND: printf("REDIR_APPEND"); break;
            case TOKEN_REDIR_HEREDOC: printf("REDIR_HEREDOC"); break;
            default: printf("UNKNOWN"); break;
        }
        printf(", Value=%s, Quote=", current->value ? current->value : "(null)");
        // ADDED: Print quote type since create_token now handles it
        switch (current->quote_type)
        {
            case NO_QUOTE: printf("NONE"); break;
            case SINGLE_QUOTE: printf("SINGLE"); break;
            case DOUBLE_QUOTE: printf("DOUBLE"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n");
        current = current->next;
        pos++;
    }
    printf("--------------\n");
}

void print_commands(t_command *cmd)
{
    int i = 0;

    while (cmd)
    {
        printf("Command %d:\n", i);
        // Print arguments
        if (cmd->args)
        {
            for (int j = 0; cmd->args[j]; j++)
                printf("  Arg[%d]: %s\n", j, cmd->args[j]);
        }
        else
        {
            printf("  No args\n");
        }
        // Print redirections if any
        t_redir *redir = cmd->redirs;
        while (redir)
        {
            printf("  Redirection: type=%d, file=%s\n", redir->type, redir->filename);
            redir = redir->next;
        }
        cmd = cmd->next;
        i++;
    }
}

char *find_path(char *cmd, char *path)
{
    char **spath = ft_split(path, ':');


    int i = 0;
    while (spath[i])
    {
        char *tcmd = ft_strjoin(spath[i], "/");
        tcmd = ft_strjoin(tcmd, cmd);
        if (access(tcmd, F_OK) != -1)
        {
            return tcmd;
        }
        i++;
    }

    return NULL;
}
void set_input (t_redir *redirs)
{
   int fd = open(redirs->filename , O_RDONLY );
    if (fd < 0 )
    {
        perror("failed");
        return;
    }
    if (dup2(fd , STDIN_FILENO) < 0)
        perror("failed");
    close (fd);
}

void set_redir(t_redir *redirs)
{   
    while(redirs)
    {
        if (redirs->type == REDIR_INPUT)
            set_input(redirs);
            
        redirs = redirs->next; 
        
    }

}
void exec_cmds(t_command *cmds, char **env)
{
    // TODO  : handel fork if it faild
    int pid = fork();

    if (pid  == 0) {

        char *path = getenv("PATH");
        char *cmd = find_path(cmds->args[0], path);
        set_redir(cmds->redirs);
        execve(cmd, cmds->args, env);
        exit(1);
    }
    else {
        wait(NULL);
    }

}

int main(int ac, char **av, char **env)
{
    char *line;
    t_token *tokens;
    t_command *commands;

    while (1)
    {
        line = read_input();
        if (!line || *line == '\0')
        {
            free(line);
            continue;
        }

        tokens = lexer(line);
        if (!tokens)
        {
            printf("Error: Lexer error occurred.\n");
            free(line);
            continue;
        }

        if (!check_syntax(tokens))
        {
            printf("Error: Syntax error occurred.\n");
            free_token_list(&tokens);
            free(line);
            continue; 
        }

        
        if (!expand_tokens(tokens)) 
        {
            printf("Error: Expansion failed.\n");
            free_token_list(&tokens);
            free(line);
            continue;
        }

        /* print_tokens(tokens); */

        commands = parse_tokens(&tokens);
        if (!commands)
        {
            printf("Error: Parsing error occurred.\n");
            free_token_list(&tokens);
            free(line);
            continue; 
        }

        exec_cmds(commands, env);
        /* print_commands(commands); */
        free_commands(commands);
        free_token_list(&tokens); 
        free(line);
    }

    return (0);
}
