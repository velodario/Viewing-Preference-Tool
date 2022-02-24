#include "requirements.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "constants.h"
#include "categorii.h"

bool is_seriale_name_valid(char* name)
{
    int len = strlen(name);
    return len >= MIN_SERIES_NAME_LEN && len <= MAX_SERIES_NAME_LEN;
}

bool is_category_id(char* category)
{
    if (strlen(category) == 1 && isdigit(category[0]) != 0)
    {
        int id = atoi(category);
        return id >= MIN_CATEGORY_ID && id <= MAX_CATEGORY_ID;
    }
    return false;
}

bool is_category(char* category)
{
    return is_category_id(category) ||
           strcmp(category, CATEGORY_TOP_10) == 0 ||
           strcmp(category, CATEGORY_LATER) == 0 ||
           strcmp(category, CATEGORY_WATCHING) == 0 ||
           strcmp(category, CATEGORY_HISTORY) == 0;
}

bool is_rating_valid(float rating)
{
    return rating >= MIN_SERIES_RATING && rating <= MAX_SERIES_RATING;
}

bool check_seasons_limit(int seasons)
{
    return seasons >= MIN_SEASONS_PER_SERIES && seasons <= MAX_SEASONS_PER_SERIES;
}

bool check_episodes_limit(int episodes)
{
    return episodes >= MIN_EPISODES_PER_SEASON && episodes <= MAX_EPISODES_PER_SEASON;
}

bool check_episode_len(int episode_len)
{
    return episode_len >= MIN_EPISODE_LEN && episode_len <= MAX_EPISODE_LEN;
}

bool check_top_position(int position)
{
    return position >= MIN_TOP_POSITION && position <= MAX_TOP_POSITION;
}

bool check_viewing_time(int viewing_time)
{
    return viewing_time >= MIN_VIEWING_TIME && viewing_time <= MAX_VIEWING_TIME;
}
