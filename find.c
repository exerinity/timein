#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "find.h"
#include "listofcities.h"

void strip_spaces(const char *src, char *dst, size_t max) {
    size_t j = 0;
    for (size_t i = 0; src[i] && j < max - 1; i++)
        if (src[i] != ' ') dst[j++] = src[i];
    dst[j] = '\0';
}

static int levenshtein(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b);
    int dp[la + 1][lb + 1];

    for (int i = 0; i <= la; i++) dp[i][0] = i;
    for (int j = 0; j <= lb; j++) dp[0][j] = j;

    for (int i = 1; i <= la; i++)
        for (int j = 1; j <= lb; j++)
            dp[i][j] = tolower(a[i-1]) == tolower(b[j-1])
                ? dp[i-1][j-1]
                : 1 + (dp[i-1][j] < dp[i][j-1]
                    ? (dp[i-1][j] < dp[i-1][j-1] ? dp[i-1][j] : dp[i-1][j-1])
                    : (dp[i][j-1] < dp[i-1][j-1] ? dp[i][j-1] : dp[i-1][j-1]));

    return dp[la][lb];
}

const CityTZ *find_city(const char *query, int *out_dist) {
    const CityTZ *best = NULL;
    int best_dist = 9999;

    for (int i = 0; cities[i].city != NULL; i++) {
        char city_nospace[256];
        strip_spaces(cities[i].city, city_nospace, sizeof(city_nospace));
        int d = levenshtein(query, city_nospace);
        if (d < best_dist) {
            best_dist = d;
            best = &cities[i];
        }
    }

    if (out_dist) *out_dist = best_dist;
    return best;
}

int parse_offset(const char *query, int *out_hours) {
    const char *p = query;
    int sign = 1;

    if (*p == '+') { sign = 1; p++; }
    else if (*p == '-') { sign = -1; p++; }

    if (*p == '\0') return 0;
    for (const char *c = p; *c; c++)
        if (*c < '0' || *c > '9') return 0;

    int h = atoi(p) * sign;
    if (h < -12 || h > 14) return 0;

    *out_hours = h;
    return 1;
}