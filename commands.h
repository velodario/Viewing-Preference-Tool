#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdbool.h>
#include "store.h"

#define MAX_CMD_ARGS 128         // max command arguments

#define ADD_seriale_CMD "add"
#define ADD_SEASON_CMD "add_sez"
#define ADD_seriale_TOP_CMD "add_top"
#define ADD_seriale_WATCH_LATER_CMD "later"
#define WATCH_seriale_CMD "watch"
#define SHOW_seriale_LIST_CMD "show"


bool add_seriale_cmd(Store* store, char** args, int args_count);
bool add_season_cmd(Store* store, char** args, int args_count);
bool add_top_seriale_cmd(Store* store, char** args, int args_count);
bool watch_seriale_cmd(Store* store, char** args, int args_count);
bool watch_later_cmd(Store* store, char** args, int args_count);
bool show_seriale_list_cmd(Store* store, char** args, int args_count);

bool is_cmd(const char* cmd);
char** parse_cmd_args(const char* line, int* args_count);

#endif // COMMANDS_H
