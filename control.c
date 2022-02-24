#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coada.h"
#include "categorii.h"
#include "constants.h"

void store_new(Store* store,
               list* tendinte_list,
               list* documentare_list,
               list* tutorials_list,
               list* top_seriale_list,
               stiva* currently_watching_stiva,
               stiva* watch_later_stiva,
               stiva* history_stiva)
{
    // creeaza lista pentru categorii de seriale
    list_new(tendinte_list, sizeof (seriale), NULL);
    list_new(documentare_list, sizeof (seriale), NULL);
    list_new(tutorials_list, sizeof (seriale), NULL);
    list_new(top_seriale_list, sizeof (seriale), NULL);

    // creeaza o stiva currently_watching
    stiva_new(currently_watching_stiva, sizeof (seriale), NULL);

    // creeaza stiva watch_later
    stiva_new(watch_later_stiva, sizeof (seriale), NULL);

    // creeaza stiva history
    stiva_new(history_stiva, sizeof (seriale), NULL);

    // creeaza spatiu de stocare
    store->tendinte = tendinte_list;
    store->documentare = documentare_list;
    store->tutoriale = tutorials_list;
    store->top10 = top_seriale_list;
    store->currently_watching = currently_watching_stiva;
    store->watch_later = watch_later_stiva;
    store->history = history_stiva;
}

void store_destroy(Store* store)
{
    // distruge listele de seriale
    list_destroy(store->tendinte);
    list_destroy(store->documentare);
    list_destroy(store->tutoriale);
    list_destroy(store->top10);

    // distruge stiva
    stiva_destroy(store->currently_watching);
    stiva_destroy(store->watch_later);
    stiva_destroy(store->history);
}

void seriale_new(seriale* seriale, char* name, char* category, float rating)
{
    // init properties
    strcpy(seriale->nume, name);
    strcpy(seriale->categoria, category);
    seriale->rating = rating;
    seriale->top_position = 0;   // not in top10

    // creeaza stiva cu sezoane
    seriale->sezon = malloc(sizeof (coada));
    coada_new(seriale->sezon, sizeof (Season), NULL);
}

void top_seriale_new(seriale* seriale, int position, char* name, float rating)
{
    // init properties
    strcpy(seriale->nume, name);
    strcpy(seriale->categoria, CATEGORY_TOP_10);
    seriale->rating = rating;
    seriale->top_position = position;

    // creeaza coada cu sezoane
    seriale->sezon = malloc(sizeof (coada));
    coada_new(seriale->sezon, sizeof (Season), NULL);
}

void season_new(Season* season)
{
    // creeaza coada cu episoade
    season->episodes = malloc(sizeof (coada));
    coada_new(season->episodes, sizeof (Episode), NULL);
}

void episode_new(Episode* episode, int duration)
{
    episode->durata = duration;
    episode->timp_vizionat = 0;
}

void seriale_destroy(seriale* seriale)
{
    // elimina coada sezoane
    coada_destroy(seriale->sezon);
    free(seriale->sezon);
}

void season_destroy(Season* season)
{
    // elimina coada cu episoade
    coada_destroy(season->episodes);
    free(season->episodes);
}

// copiaza o instanta de serial in alta
void copy_seriale(seriale* dest, seriale* src)
{
    seriale_new(dest, src->nume, src->categoria, src->rating);
    while (coada_size(src->sezon) > 0)
    {
        // copy by seasons
        Season season;
        coada_deq(src->sezon, &season);
        coada_enq(dest->sezon, &season);
    }
}

// copiaza o instanta de serial in alta si sterge sursa
void move_seriale(seriale* dest, seriale* src)
{
    copy_seriale(dest, src);
    seriale_destroy(src);
}

// adauga serial in lista, returneaza - pozitia elementului nou
int add_seriale(seriale* serial, Store* store)
{
    list* seriale_list = find_category_list(serial->categoria, store);
    if (seriale_list == NULL) { return -1; }

    int pos = list_size(seriale_list)+1; // daca element cu cel mai mic rating

    int i = 1;
    listNode* node = seriale_list->head;
    while (node != NULL)
    {
        seriale* current = (seriale*) node->data;
        // ordinea descrescătoare după rating
        if (current->rating < serial->rating)
        {
            // poziția curentă a elementului
            pos = i;
            break;
        }
        // în cazul unei rating egal, creste după nume
        else if (current->rating == serial->rating)
        {
            if (strcmp(current->nume, serial->nume) > 0)
            {
                pos = i;
                break;
            }
        }
        // next
        node = node->next;
        i++;
    }
    // insert element
    list_insert(seriale_list, pos, serial);

    return pos;
}

// elimina serial din lista de categorii
bool remove_seriale_from_category(int index, char* category, Store* store)
{
    if (index == -1) { return false; }

    list* seriale_list = NULL;
    if (strcmp(category, CATEGORY_TENDINTE) == 0)
    {
        seriale_list = store->tendinte;
    }
    else if (strcmp(category, CATEGORY_DOCUMENTARE) == 0)
    {
        seriale_list = store->documentare;
    }
    else if (strcmp(category, CATEGORY_TUTORIALE) == 0)
    {
        seriale_list = store->tutoriale;
    }
    else if (strcmp(category, CATEGORY_TOP_10) == 0)
    {
        seriale_list = store->top10;
    }

    if (seriale_list != NULL)
    {
        return list_remove(seriale_list, index);
    }

    return false;
}

bool remove_seriale_from_currently_watching(char* name, Store* store)
{
   
    int seriale_index = -1;
    seriale* seriale = find_seriale_by_name(name, store, &seriale_index, false, false, true, false, false);
    if (seriale != NULL && seriale_index != -1)
    {
        return list_remove(store->currently_watching->list, seriale_index);;
    }

    return false;
}

bool remove_seriale_from_watch_later(char* name, Store* store)
{
     // gaseste serial
    int seriale_index = -1;
    seriale* seriale = find_seriale_by_name(name, store, &seriale_index, false, false, false, true, false);
    if (seriale != NULL && seriale_index != -1)
    {
        return list_remove(store->watch_later->list, seriale_index);;
    }

    return false;
}

// insert seriale to top10
void add_top_seriale(seriale* seriale, list* top_seriale_list)
{
    strcpy(seriale->categoria, CATEGORY_TOP_10);  //schimba category
    list_insert(top_seriale_list, seriale->top_position, seriale);

    if (list_size(top_seriale_list) > MAX_TOP_POSITION)
    {
        list_remove(top_seriale_list, MAX_TOP_POSITION);
    }
}

// adaugă sezon la serial
void add_season(Season* season, seriale* seriale)
{
    coada_enq(seriale->sezon, season);
}

// adaugă episod la sezon
void add_episode(Episode* episode, Season* season)
{
    coada_enq(season->episodes, episode);
}

// obținem pointer la listă pentru categorie
list* find_category_list(char* category, Store* store)
{
    // find category list
    if (strcmp(category, CATEGORY_TENDINTE) == 0)
    {
        return store->tendinte;
    }
    else if (strcmp(category, CATEGORY_DOCUMENTARE) == 0)
    {
        return store->documentare;
    }
    else if (strcmp(category, CATEGORY_TUTORIALE) == 0)
    {
        return store->tutoriale;
    }
    else if (strcmp(category, CATEGORY_TOP_10) == 0)
    {
        return store->top10;
    }
    else if (strcmp(category, CATEGORY_WATCHING) == 0)
    {
        return store->currently_watching->list;
    }
    else if (strcmp(category, CATEGORY_LATER) == 0)
    {
        return store->watch_later->list;
    }
    else if (strcmp(category, CATEGORY_HISTORY) == 0)
    {
        return store->history->list;
    }

    return false;
}

// căuta serial după nume în întregul spațiu de stocare
seriale* find_seriale_by_name(char* name, Store* store, int* index,
                            bool in_usual_categories, bool in_top,
                            bool in_currently_watching, bool in_watch_later, bool in_history)
{
    seriale* seriale = NULL;
    if (in_usual_categories)
    {
        // căutare în tendințe
        seriale = find_seriale_in_list(name, store->tendinte, index);
        if (seriale != NULL) { return seriale; }

        // căutare în documentar
        seriale = find_seriale_in_list(name, store->documentare, index);
        if (seriale != NULL) { return seriale; }

        // căutare în tutoriale
        seriale = find_seriale_in_list(name, store->tutoriale, index);
        if (seriale != NULL) { return seriale; }
    }

    // search in top10
    if (in_top)
    {
        seriale = find_seriale_in_list(name, store->top10, index);
        if (seriale != NULL) { return seriale; }
    }

    // search in currently_watching
    if (in_currently_watching)
    {
        seriale = find_seriale_in_list(name, store->currently_watching->list, index);
        if (seriale != NULL) { return seriale; }
    }

    // search in watch_later
    if (in_watch_later)
    {
        seriale = find_seriale_in_list(name, store->watch_later->list, index);
        if (seriale != NULL) { return seriale; }
    }

    // search in history
    if (in_history)
    {
        seriale = find_seriale_in_list(name, store->history->list, index);
        if (seriale != NULL) { return seriale; }
    }

    return NULL;
}

// căuta serial în listă după nume
seriale* find_seriale_in_list(char* name, list* seriale_list, int* index)
{
    listNode* node = seriale_list->head;
    int i = 0;
    while (node != NULL)
    {
        // find by name
        seriale* serial = (seriale*) node->data;
        if (strcmp(serial->nume, name) == 0)
        {
            *index = i;
            return serial;
        }

        node = node->next;  // go to next
        i++;
    }
    *index = -1;
    return NULL;
}

// watch seriale with duration(minute)
void watch_seriale(seriale* seriale, int duration, Store* store)
{
    // 'watch' episodes
    Season* curr_season = NULL;
    Episode* curr_episode = NULL;
    int view_time = duration;
    while (view_time > 0)
    {
        // primim sezonul curent ca pointer
        curr_season = (Season*) seriale->sezon->list->head->data;

        // primim episodul curent ca pointer
        curr_episode = (Episode*) curr_season->episodes->list->head->data;

        // watch partly
        if (curr_episode->durata - curr_episode->timp_vizionat > view_time)
        {
            curr_episode->timp_vizionat += view_time;
            break;
        }

        // watch entire and continue
        view_time -= (curr_episode->durata - curr_episode->timp_vizionat);
        curr_episode->timp_vizionat = curr_episode->durata;

        Episode prev_episode;
        coada_deq(curr_season->episodes, &prev_episode);

        // urmatorul sezon
        if (coada_size(curr_season->episodes) == 0)
        {
            Season prev_season;
            coada_deq(seriale->sezon, &prev_season);
        }

        // check if seriale watched fully
        if (coada_size(seriale->sezon) == 0)
        {
            // move to history
            stiva_push(store->history, seriale);
            remove_seriale_from_currently_watching(seriale->nume, store);

            // output
            printf("Serialul %s a fost vizionat integral.\n", seriale->nume);
            return;
        }
    }
}

void print_category(char* category, Store* store, bool reverse)
{
    list* seriale_list = find_category_list(category, store);
    if (seriale_list == NULL) { return; }

    printf("Categoria %s: [", category);

    if (reverse)
    {
        int k = list_size(seriale_list);
        while (k > 0)
        {
            seriale* seriale = list_at(seriale_list, k);

            // print
            char* delimiter = "";
            if (k > 1) { delimiter = ", "; }
            printf("(%s, %.1f)%s", seriale->nume, seriale->rating, delimiter);

            // go to next(reverse)
            k--;
        }
    }
    else
    {
        int k = 0;
        listNode* node = seriale_list->head;
        while (node != NULL)
        {
            seriale* serial = (seriale*) node->data;

            char* delimiter = "";
            if (node->next != NULL) { delimiter = ", "; }
            printf("(%s, %.1f)%s", serial->nume, serial->rating, delimiter);

            node = node->next;  // go to next
            k++;
        }
    }
    printf("].\n");
}
