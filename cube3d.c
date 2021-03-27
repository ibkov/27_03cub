# include "includes/cube3d.h"


int main(int argc, char **argv)
{
    if (argc == 3 && !ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4) && \
      !ft_strncmp(argv[2], "--save", 6))
      ft_putstr_fd("ok\n", 1);
    else if (argc == 2 && !ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4))
      ft_putstr_fd("ok\n", 1);
    else
      ft_putstr_fd("\033[31m\nERROR\nMAIN ARGUMENTS\n\n\033[0m", 2);
}