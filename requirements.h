#ifndef REQUIREMENTS_H
#define REQUIREMENTS_H
#include <stdbool.h>

bool is_seriale_name_valid(char* name);      // check seriale name length
bool is_category_id(char* category);        // check category ID
bool is_category(char* category);           // check category (include 'top10, later, watching, history')
bool is_rating_valid(float rating);         // check seriale rating
bool check_seasons_limit(int seasons);      // check seasons limit per seriale
bool check_episodes_limit(int episodes);    // check episodes limit per season
bool check_episode_len(int episode_len);    // check episode length in minutes
bool check_top_position(int position);      // check top10 position
bool check_viewing_time(int viewing_time);  // check seriale watching time

#endif // REQUIREMENTS_H
