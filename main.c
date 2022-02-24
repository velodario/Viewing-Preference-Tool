#include <stdlib.h>
#include <string.h>
#include "file_utils.h"
#include "commands.h"
#include "control.h"

#define ERR_EXIT -1
#define MAXCHAR 1024
//#define NO_STDOUT_REDIRECT  // for tests

bool print_seriale(void* data);

bool run_cmd(Store* store, const char* cmd, char** args, int args_count)
{
    if (strcmp(cmd, ADD_seriale_CMD) == 0)
    {
        return add_seriale_cmd(store, args, args_count);
    }
    else if (strcmp(cmd, ADD_SEASON_CMD) == 0)
    {
        return add_season_cmd(store, args, args_count);
    }
    else if (strcmp(cmd, ADD_seriale_TOP_CMD) == 0)
    {
        return add_top_seriale_cmd(store, args, args_count);
    }
    else if (strcmp(cmd, ADD_seriale_WATCH_LATER_CMD) == 0)
    {
        return watch_later_cmd(store, args, args_count);
    }
    else if (strcmp(cmd, WATCH_seriale_CMD) == 0)
    {
        return watch_seriale_cmd(store, args, args_count);
    }
    else if (strcmp(cmd, SHOW_seriale_LIST_CMD) == 0)
    {
        return show_seriale_list_cmd(store, args, args_count);
    }

    return false;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return ERR_EXIT;
    }

    // redirect stdout to out file
#ifndef NO_STDOUT_REDIRECT
    char* out_filename = argv[2];
    if (!redirect_stdout(out_filename))
    {
        return ERR_EXIT;
    }
#endif

    // open input file
    char* in_filename = argv[1];
    FILE* in_file = open_file(in_filename);
    if (in_file == NULL)
    {
        return ERR_EXIT;
    }

    // create storage
    Store store;
    list trends_list, documentary_list, tutorials_list, top10_list;
    stiva currently_watching_stiva, watch_later_stiva, history_stiva;
    store_new(&store,
              &trends_list, &documentary_list, &tutorials_list, &top10_list,
              &currently_watching_stiva, &watch_later_stiva, &history_stiva);

    // read input file by line
    char line[MAXCHAR];
    int line_n = 1;
    while (fgets(line, MAXCHAR, in_file) != NULL)
    {
        // get cmd string
        char cmd[MAXCHAR];
        sscanf(line, "%s *\n", cmd);

        // get cmd args
        int cmd_args_count = 0;
        char** cmd_args = parse_cmd_args(line, &cmd_args_count);
        if (cmd_args_count == 0)
        {
            if (cmd_args != NULL) { free(cmd_args); }
            fclose(in_file);
            return ERR_EXIT;
        }

        if (!is_cmd(cmd))
        {
            free(cmd_args);
            fclose(in_file);
            return ERR_EXIT;
        }

        run_cmd(&store, cmd, cmd_args, cmd_args_count);
        free(cmd_args);

        line_n++;
    }
    fclose(in_file);

    store_destroy(&store);

    return 0;
}

bool print_seriale(void* data)
{
    seriale serial = *(seriale*) data;

    int n_seasons = coada_size(serial.sezon);
    printf("name = %s, category = %s, rating = %.1f, seasons = %d\n",
           serial.nume, serial.categoria, serial.rating, n_seasons);
    int s;
    for (s = 0; s < n_seasons; s++)
    {
        Season season;
        coada_deq(serial.sezon, &season);

        int n_episodes = coada_size(season.episodes);
        printf("Seasons '%d' has '%d' episode(s):\n", s+1, n_episodes);
        int ep;
        for (ep = 0; ep < n_episodes; ep++)
        {
            Episode episode;
            coada_deq(season.episodes, &episode);

            printf("    Episode '%d' has duration %d minute(s), view time = %d minute(s)\n",
                   ep+1, episode.durata, episode.timp_vizionat);
        }
    }

    return true;
}
