#include<fcntl.h>
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

int	main(int argc, char *argv[], char *env[])
{
	int	fd;

	if (argc != 5)
	{
		write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", sizeof("Usage: ./pipex file1 cmd1 cmd2 file2\n"));
		return (1);
	}
	fd =open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	close(fd);
	return (0);
}
