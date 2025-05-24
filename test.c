/* #include "minishell.h" */
/* #include <stdio.h> */
/* #include <string.h> */
/**/
/**/
/**/
/* void handle_mixed_quote(char *input, int i) */
/* { */
/*     int start; */
/*     int end; */
/*     char buffer[100]; */
/*     char second[100]; */
/*     int length; */
/*     int j; */
/**/
/*     start = i; */
/*     length = strlen(input); */
/*     while (input[i]) */
/*     { */
/*         if (input[i] == '\"' || input[i] == '\'') */
/*             break; */
/*         i++; */
/*     } */
/*     end = i; */
/*     while (start < end) */
/*     { */
/*         buffer[start] = input[start]; */
/*         start++; */
/*     } */
/*     end++; */
/*     j = 0; */
/*     while(end < length - 1) */
/*         second[j++] = input[end++]; */
/*     second[j] = '\0'; */
/* } */
/**/
/* int main(int ac, char **av) */
/* { */
/*     int i; */
/*     // char str[] = "Hello\"World"; */
/*     (void)ac; */
/**/
/*     handle_mixed_quote(av[1], 0); */
/* } */
/**/
/**/
/* // int process_complex_word(char *input, int i, t_token **tokens) */
/* // { */
/* //     char quote; */
/* //     char *buffer = NULL; */
/* //     char *tmp = NULL; */
/* //     int start; */
/**/
/* //     while (input[i] && !is_whitespace(input[i]) && !get_operator_length(input, i)) */
/* //     { */
/* //         if (input[i] == '\'' || input[i] == '\"') */
/* //         { */
/* //             // Handle quoted part */
/* //             quote = input[i]; */
/* //             i++; // skip opening quote */
/* //             start = i; */
/**/
/* //             // Find closing quote */
/* //             while (input[i] && input[i] != quote) */
/* //                 i++; */
/**/
/* //             if (!input[i]) // if no closing quote found */
/* //             { */
/* //                 printf("Syntax Error: Unclosed quote '%c'\n", quote); */
/* //                 if (buffer) */
/* //                     free(buffer); */
/* //                 return -1; */
/* //             } */
/**/
/* //             // Extract the quoted string */
/* //             tmp = ft_substr(input, start, i - start); */
/* //             i++; // skip closing quote */
/* //         } */
/* //         else */
/* //         { */
/* //             // Handle unquoted part */
/* //             start = i; */
/* //             while (input[i] && input[i] != '\'' && input[i] != '\"'  */
/* //                 && !is_whitespace(input[i]) && !get_operator_length(input, i)) */
/* //                 i++; */
/**/
/* //             // Extract the unquoted string */
/* //             tmp = ft_substr(input, start, i - start); */
/* //         } */
/**/
/* //         if (!tmp) */
/* //         { */
/* //             if (buffer) */
/* //                 free(buffer); */
/* //             return -1; */
/* //         } */
/**/
/* //         if (buffer == NULL) */
/* //         { */
/* //             // First part: just assign tmp to buffer */
/* //             buffer = tmp; */
/* //         } */
/* //         else */
/* //         { */
/* //             // Join the new part to buffer */
/* //             char *new_buf = ft_strjoin(buffer, tmp); */
/* //             free(buffer); */
/* //             free(tmp); */
/* //             buffer = new_buf; */
/**/
/* //             if (!buffer) */
/* //                 return -1; */
/* //         } */
/* //     } */
/**/
/* //     if (!buffer) // "   | ls" */
/**/
/* //         return i; // no word found, just return current position */
/**/
/* //     t_token *token = create_token(buffer, TOKEN_WORD); */
/* //     if (!token) */
/* //     { */
/* //         free(buffer); */
/* //         return -1; */
/* //     } */
/**/
/* //     add_token_back(tokens, token); */
/* //     return i; */
/* // } */
