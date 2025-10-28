#include<fcntl.h>
#include "libft/libft.h"
#include <unistd.h>

int     ft_strncmp(const char *s1, const char *s2, size_t n)
{
        size_t  i;

        i = 0;
        while (i < n && (s1[i] || s2[i]))
        {
                if (s1[i] != s2[i])
                        return ((unsigned char)s1[i] - (unsigned char)s2[i]);
                i++;
        }
        return (0);
}




void close_pipe(int	*pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	create_pipe(int	*pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

int	open_output_file(char	*filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		return (-1);
	}
	return (fd);
}

int	open_input_file(char	*filename)
{
	int	fd;

	fd  = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening input file");
		return (-1);
	}
	return (fd);
}

void execute_second_command(char **argv, int *pipe_fd, char **envp)
{
	int	fd_output;

	fd_output = open_output_file(argv[4]);
	dup2(fd_output, STDOUT_FILENO);
	close(fd_output);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execute_command(argv[3], envp); 
}
void	execute_first_command(char	**argv, int	*pipe_fd, char **envp)
{
	int	fd_input;
	
	fd_input = open_input_file(argv[1]);
	dup2(fd_input, STDIN_FILENO);
	close(fd_input);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execute_command(argv[2], envp);
}

char *get_path_from_env(char **envp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while(envp[i] != NULL)
	{
		if (!ft_strncmp(envp[i], "PATH=", sizeof("PATH=")))
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}	

char *find_commmand_path(char *cmd, char **envp)
{
	char **paths;
	char *path;
	char *full_path;
	char *temp;
	int	i;

	if (access(cmd, X_OK) == 0)
		return (strdup(cmd));
	path = get_path_from_env(envp);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(path[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

void	execute_command(char *cmd, char **envp)
{
	char **cmd_args;
	char *path;
	
	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args)
	{
		perror("Error parsing command");
		exit (EXIT_FAILURE);
	}
	path = find_command_path(cmd_args[0], envp);
	if (!path)
	{
		write(2, "Command not found: ", 19);
		write(2, cmd_args[0], ft_strlen(cmd_args[0]));
		write(2, "\n", 1);
		free_array(cmd_args);
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd_args, envp) == -1)
	{
		perror("execve");
		free(path);
		free_array(cmd_args);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	if (argc != 5)
	{
		write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 38);
		return (1);
	}
	create_pipe(pipe_fd);
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid1 == 0) 
	{
		execute_first_command(argv, pipe_fd, envp);
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid2 == 0)  
	{
		execute_second_command(argv, pipe_fd, envp);
	}
	close_pipe(pipe_fd);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
