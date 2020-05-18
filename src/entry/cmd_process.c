/*
** EPITECH PROJECT, 2020
** PSU_42sh_2019
** File description:
** Created: 2020-03-31T15:04:50+02:00 | Author: simon
*/

#include "shell.h"

static int cmd_process_command(shell_t *shell, cmd_t *cmd)
{
    char **warray_cmd;

    if (!load_magic_quote(shell, cmd))
        return EXIT_ERROR;
    warray_cmd = linked_list_to_warray(cmd);
    if (!warray_cmd)
        return puterr("warray cmd : error\n", EXIT_ERROR);
    if (shell_exec_cmd(shell, warray_cmd) == EXIT_ERROR) {
        return EXIT_ERROR;
    }
    word_array_destroy(warray_cmd);
    return EXIT_SUCCESS;
}

int cmd_process(shell_t *shell, cmd_t *cmd)
{
    redirect_t redirect;
    int ret;

    if (redirection_process(cmd, &redirect) != EXIT_SUCCESS) {
        clean_redirect(&redirect);
        shell->exit_status = ERROR_STATUS;
        return EXIT_SUCCESS;
    }
    // TODO : job control
    // <=====
    //
    ret = parenthesis_exec(shell, cmd);
    if (ret == EXIT_ERROR) {
        return EXIT_ERROR;
    } else if (ret == EXIT_FAIL) {
        if (cmd_process_command(shell, cmd) == EXIT_ERROR) {
            return EXIT_ERROR;
        }
    }
    clean_redirect(&redirect);
    return EXIT_SUCCESS;
}
