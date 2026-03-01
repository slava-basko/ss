#ifndef SS_H_
#define SS_H_

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

#define ss_log(format, ...) ss_custom_log(__FILE__, __LINE__, format, ##__VA_ARGS__)

void ss_custom_log(const char* file, int line, const char* format, ...);

#ifdef SS_IMPLEMENTATION

void ss_custom_log(const char* file, int line, const char* format, ...) {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0) {
        tv.tv_sec = time(NULL);
        tv.tv_usec = 0;
    }

    struct tm *localTimeInfo = localtime(&tv.tv_sec);

    char timeBuffer[32];
    if (localTimeInfo) {
        char base[20];
        if (strftime(base, sizeof(base), "%Y-%m-%d %H:%M:%S", localTimeInfo) == 0) {
            strncpy(base, "0000-00-00 00:00:00", sizeof(base));
            base[sizeof(base) - 1] = '\0';
        }
        snprintf(timeBuffer, sizeof(timeBuffer), "%s.%06ld", base, (long)tv.tv_usec);
    } else {
        strncpy(timeBuffer, "0000-00-00 00:00:00.000000", sizeof(timeBuffer));
        timeBuffer[sizeof(timeBuffer) - 1] = '\0';
    }

    fprintf(stderr, "%s:%d: / %s / ", file, line, timeBuffer);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    if (format && format[0] != '\0' && format[strlen(format) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
}

#endif // SS_IMPLEMENTATION

#ifndef SS_UNSTRIP_PREFIX
    #define log ss_log
#endif

#endif // SS_H_
