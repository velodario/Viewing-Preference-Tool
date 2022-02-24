#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "requirements.h"
#include "categorii.h"
#include "control.h"

// add <ID> <name> <rating> <#seasons> <#epSeason1> <#lengthEp1> <#lengthEp2> …
bool add_seriale_cmd(Store* store, char** args, int args_count)
{
    if (args_count < 4)
    {
            return false;
    }

    char* category = args[0];
    char* seriale_name = args[1];
    float rating = atof(args[2]);
    int seasons = atoi(args[3]);

    // make values checks
    if (!is_category_id(category))
    {
        return false;
    }

    if (!is_seriale_name_valid(seriale_name))
    {
        return false;
    }

    if (!is_rating_valid(rating))
    {
        return false;
    }

    if (!check_seasons_limit(seasons))
    {
        return false;
    }

    // create seriale
    seriale seriale;
    seriale_new(&seriale, seriale_name, category, rating);

    // parse seasons
    int args_pos = 4;   // next to first season count
    int s;
    for (s = 0; s < seasons; s++)
    {
        // get episodes count
        int episodes = atoi(args[args_pos]);
        if (!check_episodes_limit(episodes))
        {
            return false;
        }
        args_pos++;

        // create season
        Season season;
        season_new(&season);

        // parse episodes
        int ep;
        for (ep = 0; ep < episodes; ep++)
        {
            if (args_pos >= args_count)
            {
                season_destroy(&season);    // remove incorrect season
                return false;
            }

            int episode_len = atoi(args[args_pos]);
            if (!check_episode_len(episode_len))
            {
                season_destroy(&season);    // remove incorrect season
                return false;
            }
            Episode episode;
            episode_new(&episode, episode_len);
            add_episode(&episode, &season);

            args_pos++;
        }
        add_season(&season, &seriale);
    }
    int position = add_seriale(&seriale, store);

    printf("Serialul %s a fost adaugat la pozitia %d.\n", seriale_name, position);

    return true;
}

// add_sez <name> <#ep> <#lengthEp1> <#lengthEp2>
bool add_season_cmd(Store* store, char** args, int args_count)
{
    if (args_count < 3)
    {
        return false;
    }

    // check seriale name for length
    char* seriale_name = args[0];
    if (!is_seriale_name_valid(seriale_name))
    {
        return false;
    }

    // search seriale in categories, include top10
    int seriale_index;
    seriale* seriale = find_seriale_by_name(seriale_name, store, &seriale_index, true, true, true, true, false);
    if (seriale == NULL)
    {
        return false;
    }

    // check episodes in season count
    int episodes = atoi(args[1]);
    if (!check_episodes_limit(episodes))
    {
        return false;
    }

    // create season
    Season season;
    season_new(&season);

    int args_pos = 2;   // next to episode count
    int ep;
    for (ep = 0; ep < episodes; ep++)
    {
        // find arguments for episode
        int episode_len = atoi(args[args_pos]);
        if (!check_episode_len(episode_len))
        {
            season_destroy(&season);    // remove incorrect season
            return false;
        }

        // add episode
        Episode episode;
        episode_new(&episode, episode_len);
        add_episode(&episode, &season);

        args_pos++;
    }
    add_season(&season, seriale);

    printf("Serialul %s are un sezon nou.\n", seriale_name);

    return true;
}

// add_top <pos> <name> <rating> <#seasons> <#epSeason1> <#lengthEp1> <#lengthEp2> … <#epSeason2> <#lengthEp1> <#lengthEp2>
bool add_top_seriale_cmd(Store* store, char** args, int args_count)
{
    if (args_count < 4)
    {
        return false;
    }
    int position = atoi(args[0]);
    char* name = args[1];
    float rating = atof(args[2]);
    int seasons = atoi(args[3]);

    if (!check_top_position(position))
    {
        return false;
    }

    if (!is_seriale_name_valid(name))
    {
        return false;
    }

    if (!is_rating_valid(rating))
    {
        return false;
    }

    if (!check_seasons_limit(seasons))
    {
        return false;
    }

    // create seriale
    seriale seriale;
    top_seriale_new(&seriale, position, name, rating);

    // parse seasons
    int args_pos = 4;   // next to first season count
    int s;
    for (s = 0; s < seasons; s++)
    {
        // get episodes count
        int episodes = atoi(args[args_pos]);
        if (!check_episodes_limit(episodes))
        {
            return false;
        }
        args_pos++;

        // create season
        Season season;
        season_new(&season);

        // parse episodes
        int ep;
        for (ep = 0; ep < episodes; ep++)
        {
            if (args_pos >= args_count)
            {
                season_destroy(&season);    // remove incorrect season
                return false;
            }

            int episode_len = atoi(args[args_pos]);
            if (!check_episode_len(episode_len))
            {
                season_destroy(&season);    // remove incorrect season
                return false;
            }
            Episode episode;
            episode_new(&episode, episode_len);
            add_episode(&episode, &season);

            args_pos++;
        }
        add_season(&season, &seriale);
    }
    add_top_seriale(&seriale, store->top10);  // insert to top10

    // show top10
    print_category(CATEGORY_TOP_10, store, false);

    return true;
}

bool watch_seriale_cmd(Store* store, char** args, int args_count)
{
    if (args_count != 2)
    {
        return false;
    }

    if (args_count != 2)
    {
        return false;
    }
    char* seriale_name = args[0];
    int view_time = atoi(args[1]);

    if (!is_seriale_name_valid(seriale_name))
    {
        return false;
    }

    if (!check_viewing_time(view_time))
    {
        return false;
    }

    // check seriale in categories
    int seriale_index = -1;
    seriale* seriale = find_seriale_by_name(seriale_name, store, &seriale_index, true, true, false, false, false);
    if (seriale != NULL)
    {
        stiva_push(store->currently_watching, seriale);
        remove_seriale_from_category(seriale_index, seriale->categoria, store);

        watch_seriale(seriale, view_time, store);

        return true;
    }
    // check currently_watching stiva
    seriale = find_seriale_by_name(seriale_name, store, &seriale_index, false, false, true, false, false);
    if (seriale != NULL)
    {
        //at the top of the stiva is the seriale watched most recently
        list_remove(store->currently_watching->list, seriale_index);
        stiva_push(store->currently_watching, seriale);

        watch_seriale(seriale, view_time, store);

        return true;
    }

    // check watch_later stiva
    seriale = find_seriale_by_name(seriale_name, store, &seriale_index, false, false, false, true, false);
    if (seriale != NULL)
    {
        stiva_push(store->currently_watching, seriale);
        remove_seriale_from_watch_later(seriale_name, store);

        watch_seriale(seriale, view_time, store);

        return true;
    }
    return false;
}

// later <name>
bool watch_later_cmd(Store* store, char** args, int args_count)
{
    if (args_count != 1)
    {
        return false;
    }
    char* seriale_name = args[0];
    if (!is_seriale_name_valid(seriale_name))
    {
        return false;
    }

    // search seriale in categories, include top10
    int seriale_index = -1;
    seriale* seriale = find_seriale_by_name(seriale_name, store, &seriale_index, true, true, false, false, false);
    if (seriale != NULL && seriale_index != -1)
    {
        // add to watch_later stiva
        stiva_push(store->watch_later, seriale);

        // remove from category list
        remove_seriale_from_category(seriale_index, seriale->categoria, store);

        // output position at watch_later stiva
        printf("Serialul %s se afla in coada de asteptare pe pozitia %d.\n", seriale_name, stiva_size(store->watch_later));

        return true;
    }

    // search in currently watching stiva
    seriale = find_seriale_by_name(seriale_name, store, &seriale_index, false, false, true, false, false);
    if (seriale != NULL && seriale_index != -1)
    {
        // add to watch_later stiva
        stiva_push(store->watch_later, seriale);

        // remove currently watching stiva
        remove_seriale_from_currently_watching(seriale_name, store);

        // output position at watch_later stiva
        printf("Serialul %s se afla in coada de asteptare pe pozitia %d.\n", seriale_name, stiva_size(store->watch_later));

        return true;
    }
    return false;
}

// show <category>
// category ∈ {1, 2, 3, top10, later, watching, history}
bool show_seriale_list_cmd(Store* store, char** args, int args_count)
{
    if (args_count != 1)
    {
        return false;
    }

    char* category = args[0];
    if (!is_category(category))
    {
        return false;
    }

    bool reverse = strcmp(category, CATEGORY_LATER) == 0;
    print_category(category, store, reverse);

    return true;
}

bool is_cmd(const char* cmd)
{
    return  strcmp(cmd, ADD_seriale_CMD) == 0 ||
            strcmp(cmd, ADD_SEASON_CMD) == 0 ||
            strcmp(cmd, ADD_seriale_TOP_CMD) == 0 ||
            strcmp(cmd, ADD_seriale_WATCH_LATER_CMD) == 0 ||
            strcmp(cmd, WATCH_seriale_CMD) == 0 ||
            strcmp(cmd, SHOW_seriale_LIST_CMD) == 0;
}

char** parse_cmd_args(const char* line, int* args_count)
{
    char* buff = strchr(line, ' ');
    if (!buff)
    {
        *args_count = 0;
        return NULL;
    }
    buff = buff + 1;                // remove space from begin
    buff[strcspn(buff, "\n")] = 0;  // remove \n from end

    // allocate buffer for command args
    char** args_buff = malloc((MAX_CMD_ARGS+1) * sizeof (char*));

    // parse separate arguments from line
    int count = 0;
    char* tok = strtok(buff, " ");
    while (tok != NULL)
    {
        args_buff[count] = tok;     // set arg
        tok = strtok(NULL, " ");    // parse next
        count++;

        // don't parse too much args
        if (count == MAX_CMD_ARGS) { break; }
    }

    size_t size = (count+1) * sizeof (char*);
    char** my_args = malloc(size);      // allocate args array
    memcpy(my_args, args_buff, size);   // copy from buffer
    *args_count = count;                // set args count

    // deallocate args buffer
    free(args_buff);

    return my_args;
}
