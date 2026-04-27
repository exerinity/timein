#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "find.h"
#include "listofcities.h"

static const char *day_night_icon(int hour) {
    if (hour >= 6 && hour < 18)
        return "\x1b[33m🌣\x1b[0m";
    else
        return "\x1b[90m☽\x1b[0m";
}

static void print_about(void) {
    printf("timein is a tool for quickly resolving the time in another city or offset, by exerinity, written in C. learn more:\n");
    printf("https://github.com/exerinity/timein\n");
}

static void print_time_for_offset(int offset_hours) {
    time_t now = time(NULL);
    now += offset_hours * 3600;
    struct tm *t = gmtime(&now);

    const char *days[]   = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    const char *months[] = {"January","February","March","April","May","June",
                            "July","August","September","October","November","December"};

    int hour12 = t->tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    const char *ampm = t->tm_hour < 12 ? "am" : "pm";

    char label[16];
    if (offset_hours >= 0)
        snprintf(label, sizeof(label), "UTC+%d", offset_hours);
    else
        snprintf(label, sizeof(label), "UTC%d", offset_hours);

    printf("%s: %s, %s %d, %d - %d:%02d:%02d %s / %02d:%02d:%02d %s\n",
        label, days[t->tm_wday], months[t->tm_mon], t->tm_mday, t->tm_year + 1900,
        hour12, t->tm_min, t->tm_sec, ampm,
        t->tm_hour, t->tm_min, t->tm_sec,
        day_night_icon(t->tm_hour));
}

static void print_time_for_tz(const char *city, const char *tz) {
    setenv("TZ", tz, 1);
    tzset();

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    const char *days[]   = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    const char *months[] = {"January","February","March","April","May","June",
                            "July","August","September","October","November","December"};

    int hour12 = t->tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    const char *ampm = t->tm_hour < 12 ? "am" : "pm";

    printf("%s: %s, %s %d, %d - %d:%02d:%02d %s / %02d:%02d:%02d %s\n",
        city, days[t->tm_wday], months[t->tm_mon], t->tm_mday, t->tm_year + 1900,
        hour12, t->tm_min, t->tm_sec, ampm,
        t->tm_hour, t->tm_min, t->tm_sec,
        day_night_icon(t->tm_hour));
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "about") == 0) {
        print_about();
        return 0;
    }

    if (argc < 2) {
        fprintf(stderr, "usage: timein <city|offset>\nExamples: timein tokyo / timein +10 / timein 3 / timein -4\n");
        return 1;
    }

    char input[256] = {0};
    for (int i = 1; i < argc; i++) {
        if (i > 1) strncat(input, " ", sizeof(input) - strlen(input) - 1);
        strncat(input, argv[i], sizeof(input) - strlen(input) - 1);
    }

    int offset;
    if (parse_offset(input, &offset)) {
        print_time_for_offset(offset);
        return 0;
    }

    char query[256];
    strip_spaces(input, query, sizeof(query));

    int dist;
    const CityTZ *best = find_city(query, &dist);

    if (!best) {
        fprintf(stderr, "city not found\n");
        return 1;
    }

    if (dist > 4)
        fprintf(stderr, "closest find: %s (distance %d -- try again?)\n",
                best->city, dist);

    print_time_for_tz(best->city, best->tz);
    return 0;
}