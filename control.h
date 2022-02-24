#ifndef TEST_H
#define TEST_H
#include "seriale.h"
#include "season.h"
#include "episod.h"
#include "store.h"

void store_new(Store* store,
               list* trends_list,
               list* documentary_list,
               list* tutorials_list,
               list* top_seriale_list,
               stiva* currently_watching_stiva,
               stiva* watch_later_stiva,
               stiva* history_stiva);
void store_destroy(Store* store);

void seriale_new(seriale* seriale, char* name, char* category, float rating);
void top_seriale_new(seriale* seriale, int position, char* name, float rating);
void season_new(Season* season);
void episode_new(Episode* episode, int duration);

void seriale_destroy(seriale* seriale);
void season_destroy(Season* season);

void copy_seriale(seriale* dest, seriale* src);
void move_seriale(seriale* dest, seriale* src);

int add_seriale(seriale* seriale, Store* store);
bool remove_seriale_from_category(int index, char* category, Store* store);
bool remove_seriale_from_currently_watching(char* name, Store* store);
bool remove_seriale_from_watch_later(char* name, Store* store);

void add_top_seriale(seriale* seriale, list* top_seriale_list);
void add_season(Season* season, seriale* seriale);
void add_episode(Episode* episode, Season* season);

list* find_category_list(char* category, Store* store);
seriale* find_seriale_by_name(char* name, Store* store, int* index,
                            bool in_usual_categories, bool in_top,
                            bool currently_watching, bool watch_later, bool history);
seriale* find_seriale_in_list(char* name, list* seriale_list, int* index);

void watch_seriale(seriale* seriale, int duration, Store* store);

void print_category(char* category, Store* store, bool reverse);

#endif // TEST_H
