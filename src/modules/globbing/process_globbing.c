/*
** EPITECH PROJECT, 2020
** PSU_42sh_2019
** File description:
** Created: 2020-05-15T08:53:41+02:00 | Author: simon
*/

#include "globbing.h"

static int split_filepath(const char *str, char **ptr_path, char **ptr_regexp)
{
    int idx_slash = -1;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '/') {
            idx_slash = (int)i;
        }
    }
    if (idx_slash == -1) {
        *ptr_path = my_strdup("./");
    } else {
        *ptr_path = my_strndup(str, (idx_slash + 1));
    }
    *ptr_regexp = my_strdup(&str[idx_slash + 1]);
    if (!(*ptr_path) || !(*ptr_regexp)) {
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

static int replace_token(token_t *token_list, cmd_t *cmd, token_t *old_token)
{
    token_t *last = get_last_token(token_list);

    insert_new_token(old_token, token_list, last, cmd);
    free(old_token->token);
    free(old_token);
    return EXIT_SUCCESS;
}

static int process_filelist(file_list_t **ptr_filenames, token_t *tok,
cmd_t *cmd, char *path)
{
    file_list_t *filenames = *ptr_filenames;
    file_list_t *prev_node = NULL;
    file_list_t *del_node = NULL;
    token_t *token_list;

    for (file_list_t *ptr = filenames; ptr != NULL;) {
        if (ptr->name[0] == '.'/* IF Don't Match */) {
            del_node = ptr;
            ptr = ptr->next;
            remove_invalid_node(del_node, prev_node, ptr_filenames);
        } else {
            prev_node = ptr;
            ptr = ptr->next;
        }
    }
    token_list = file_list_to_token_list(*ptr_filenames, path);
    if (!token_list) {
        return EXIT_ERROR;
    }
    if (replace_token(token_list, cmd, tok) == EXIT_ERROR) {
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

int process_globbing(token_t *tok, cmd_t *cmd)
{
    char *path;
    char *regexp;
    file_list_t *filenames = NULL;
    int ret = EXIT_SUCCESS;

    if (split_filepath(tok->token, &path, &regexp) == EXIT_ERROR) {
        return EXIT_FAIL;
    }
    if (my_read_dir(&filenames, path) <= 0) {
        free(path);
        free(regexp);
        return EXIT_FAIL;
    }
    if (process_filelist(&filenames, tok, cmd, path) == EXIT_ERROR) {
        ret = EXIT_FAIL;
    }
    free(path);
    free(regexp);
    my_file_list_destroy(filenames);
    return ret;
}