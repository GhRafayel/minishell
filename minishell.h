/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:42:27 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:45:28 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


# include "src/libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h> 
# include <sys/stat.h> 
# include <sys/wait.h>
# include <signal.h> 
# include <readline/readline.h> 
# include <readline/history.h>
# include <stdbool.h>
# include <limits.h>

extern volatile sig_atomic_t	g_signal;

# ifndef O_WRITE
#  define O_WRITE 1
# endif

# ifndef O_READ
#  define O_READ 0
# endif

enum e_struct
{
	SUCCESS = 0,
	MALLOC = 1,
	SYNTAX = 2,
	PIPE_FAILURE = 9,
	FORK_FAILURE = 10,
	UNKNOWN_ERROR = 13,
	SYNTAX_ERROR = 14,
	FILE_ERROR = 26
};

typedef struct s_env_list
{
	char				*key;
	char				*value;
	struct s_env_list	*next;
}	t_env_list;

typedef struct s_token
{
	char				*str;
	char				type;
	struct s_token		*next;
}	t_token;

typedef struct s_cmd
{
	struct s_token		*c_list;
	bool				type;
	char				*cmd;
	char				**arg;
	char				*h_name;
	char				*cmd_path;
	int					fd_out;
	int					fd_inp;
	int					pip_fd[2];
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_data
{
	bool				value;
	int					exit_status;
	char				*input;
	char				*prompt;
	char				**env_copy;
	int					status;
	char				**path;
	pid_t				last_pid;
	struct s_env_list	*env_list;
	struct s_cmd		*cmd;
}	t_data;

int			m_check_new_path(t_data *data, char *str);
int			m_check_commands(t_data *data);
int			expansion(t_data *data, t_cmd *pnt);
int			m_heredoc(t_data *data, t_token *p, t_cmd **pnt, bool val);
int			m_add_env_var(t_data *data, char *arg, bool value);
int			m_export_change_env(t_data *data, char *arg, bool value);
int			m_check_next_token(t_token *pnt, bool val);
int			m_check_slesh(t_data *data, t_cmd **pnt);
int			m_create_path(t_data *data, char *str);
int			m_cd(t_data *data, t_cmd *pnt);
int			m_malloc_exit(t_data *data);
int			m_builtin_exit(t_data *data, t_cmd *pnt);
int			m_is_built_in(t_data *data, t_cmd *t_pnt);
int			m_create_heredoc_file(t_data *data, t_cmd **t_pnt, bool val);
int			m_input_1(t_data *data, t_cmd **pnt, t_token *p, bool val);
int			m_execute_pipeline(t_data *data, t_cmd *pnt, t_cmd *prev);
int			m_exit(t_data *data);
int			m_init_data(t_data *data, char **env);
int			m_copy_env(t_data *data, char **env);
int			m_count_quote(char *input);
int			m_find_quote(t_data *data);
int			m_found_quote(t_cmd *t_pnt, int *start, bool *quote);
int			m_found_space(t_cmd *t_pnt, int *start, bool *quote, char c);
int			m_strcmp(const char *s1, const char *s2);
int			m_env_list_length(t_env_list *list);
int			m_check_built_in(t_data *data, t_cmd *pnt);
int			m_put_path(t_data *data, t_cmd **pnt);
int			m_clear_qoute_loop(t_data *data, t_token *pnt);
int			m_clear_qoute(t_data *data, t_token **list);
int			m_create_cmd(t_data *data, t_cmd **pnt);
int			m_cut_redirect(t_cmd *t_pnt, int *start, bool *quote, char c);
int			m_found_redirect(t_data *data, t_cmd *t, int *s, bool *q);
int			heredoc_event_hook(void);
int			m_heredoc_1(t_data *data, t_token *pnt, t_cmd **t_pnt, bool val);
int			m_heredoc_variable(t_data *data, t_token *pnt);
int			m_do_redirects(t_data *data, t_cmd **pnt, bool val);
int			m_append_and_out(t_data *data, t_cmd **t_pnt, t_token *p);
int			m_output_and_append_1(t_data *d, t_cmd **t, t_token *p, int n);
int			m_input(t_data *data, t_cmd **pnt, t_token *p, bool val);
int			m_export(t_data *data, t_cmd *t_pnt);
int			m_print_export(t_data *data);
int			m_unset(t_data *data, t_cmd *t_pne);
int			m_check_key(char *key);
int			m_copy_env_from_list(t_data *data);
int			m_env(t_data *data, t_cmd *pnt);
int			m_echo(t_cmd **t_pnt);
int			m_pwd(t_cmd *t_pnt);
int			*m_get_exit_status(int *exit_status);

void		m_add_env_back(t_env_list **lst, t_env_list *new);
void		m_free_env_list(t_env_list **env_list);
void		m_heredoc_error(t_data *data, t_cmd *pnt);
void		m_free_data(t_data *data);
void		m_free(char **str);
void		check_error_message(t_data *data);
void		m_delete_token_node(t_token **list, t_token *node);
void		m_free_token_list(t_token **c_list);
void		m_add_token_list_back(t_token **list, t_token *new_node);
void		m_free_cmd_list(t_cmd **cmd_list);
void		m_export_sort(t_data *data, char **env);
void		m_o_a_error(t_data *data, t_cmd **pnt, char *error, int index);
void		sigint_handler(int sig);
void		handle_signals_main(void);
void		handle_signals_heredoc(void);
void		signal_child(int sig);
void		m_print_exprot_error(t_data *data, char *s1, char *s2);

char		*m_find_variable_1(t_data *data, char *str, int *start);
char		*m_find_variable_2(t_data *data, char *str, int *start);
char		*m_heredoc_2(t_data *data, t_token *pnt, bool value);
char		*m_replace_variable(t_data *data, char *var_name);
char		*m_shlvl(t_data *data, char *str, bool val);
char		*m_find_variable(t_data *data, char *str);
char		*m_cut_until_end(char *str, int start);
char		*m_str_concat(char *str1, char *str2);
char		*m_get_prompt(void);
char		*m_trim(char *input);

t_cmd		*m_split_pips(t_data *data);
t_cmd		*m_new_cmd(t_data *data, char *str);
t_token		*m_new_token_node(t_token **list, char *str, char c, bool val);
t_env_list	*m_find_env_node(t_data *data, char *key, bool val);
t_env_list	*m_new_env_node(char *str);

#endif
