#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "find.h"
#include "listofcities.h"

void strip_spaces(const char *src, char *dst, size_t max) {
    size_t j = 0;
    for (size_t i = 0; src[i] && j < max - 1; i++)
        if (src[i] != ' ') dst[j++] = src[i];
    dst[j] = '\0';
}

static void normalize(const char *src, char *dst, size_t max) {
    size_t j = 0;
    for (size_t i = 0; src[i] && j < max - 1; i++) {
        unsigned char c = (unsigned char)src[i];
        if (isalnum(c)) dst[j++] = tolower(c);
        else if (c == '-' || c == '_' || c == ' ') continue;
    }
    dst[j] = '\0';
}

static int levenshtein(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b);
    int *buf = malloc((la + 1) * (lb + 1) * sizeof(int));
    if (!buf) return 9999;
    int **dp = malloc((la + 1) * sizeof(int*));
    if (!dp) { free(buf); return 9999; }
    for (int i = 0; i <= la; i++) dp[i] = buf + i * (lb + 1);

    for (int i = 0; i <= la; i++) dp[i][0] = i;
    for (int j = 0; j <= lb; j++) dp[0][j] = j;

    for (int i = 1; i <= la; i++)
        for (int j = 1; j <= lb; j++) {
            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            int del = dp[i-1][j] + 1;
            int ins = dp[i][j-1] + 1;
            int sub = dp[i-1][j-1] + cost;
            int best = del < ins ? del : ins;
            if (sub < best) best = sub;
            dp[i][j] = best;
        }

    int res = dp[la][lb];
    free(dp);
    free(buf);
    return res;
}

const CityTZ *find_city(const char *query, int *out_dist) {
    if (!query || !*query) {
        if (out_dist) *out_dist = 0;
        return NULL;
    }

    char nq[256];
    normalize(query, nq, sizeof(nq));
    size_t nq_len = strlen(nq);

    static char **norm_cities = NULL;
    if (!norm_cities) {
        int count = 0;
        for (; cities[count].city != NULL; count++);
        norm_cities = calloc(count + 1, sizeof(char*));
        for (int i = 0; i < count; i++) {
            size_t L = strlen(cities[i].city) + 1;
            char *buf = malloc(L);
            if (!buf) { norm_cities[i] = NULL; continue; }
            normalize(cities[i].city, buf, L);
            norm_cities[i] = buf;
        }
        norm_cities[count] = NULL;
    }

    const CityTZ *best = NULL;
    int best_raw = 9999;
    double best_ratio = 1.0;

    for (int i = 0; norm_cities[i] != NULL; i++) {
        const char *nc = norm_cities[i];
        size_t nc_len = strlen(nc);
        if (nq_len > 0 && strcmp(nq, nc) == 0) {
            if (out_dist) *out_dist = 0;
            return &cities[i];
        }
        if (nq_len > 0 && nc_len >= nq_len && strncmp(nc, nq, nq_len) == 0) {
            if (out_dist) *out_dist = 0;
            return &cities[i];
        }
        if (nq_len > 0 && strstr(nc, nq) != NULL) {
            if (out_dist) *out_dist = 0;
            return &cities[i];
        }

        int raw = levenshtein(nq, nc);
        double ratio = (double)raw / (double)(nc_len > nq_len ? nc_len : nq_len);
        if (ratio < best_ratio || (ratio == best_ratio && raw < best_raw)) {
            best_ratio = ratio;
            best_raw = raw;
            best = &cities[i];
        }
    }

    if (best && best_ratio <= 0.45) {
        if (out_dist) *out_dist = best_raw;
        return best;
    }

    if (out_dist) *out_dist = best ? best_raw : 9999;
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