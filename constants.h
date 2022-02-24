#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
● 1 <= număr de seriale <= 2048
● 1 <= număr de sezoane pe serial <= 10
● 1 <= număr de episoade pe sezon <= 20
● 1 <= durată episod <= 120
● 1 <= lungime nume serial <= 32
● 1.0 <= rating <= 10.0
*/

#define MIN_SERIES_NAME_LEN 1
#define MAX_SERIES_NAME_LEN 32

#define MIN_CATEGORY_ID 1
#define MAX_CATEGORY_ID 3

#define MIN_SERIES 1
#define MAX_SERIES 2048

#define MIN_SEASONS_PER_SERIES 1
#define MAX_SEASONS_PER_SERIES 10

#define MIN_EPISODES_PER_SEASON 1
#define MAX_EPISODES_PER_SEASON 20

#define MIN_EPISODE_LEN 1
#define MAX_EPISODE_LEN 120

#define MIN_SERIES_NAME_LEN 1
#define MAX_SERIES_NAME_LEN 32

#define MIN_SERIES_RATING 1.0
#define MAX_SERIES_RATING 10.0

#define MIN_TOP_POSITION 1
#define MAX_TOP_POSITION 10

#define MIN_VIEWING_TIME 1
#define MAX_VIEWING_TIME MAX_SEASONS_PER_SERIES*MAX_EPISODES_PER_SEASON*MAX_EPISODE_LEN

#endif // CONSTANTS_H
