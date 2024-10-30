/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    logging.c
 * @brief   Function defination of logging framework
 *****************************************************************************************/
#include "logging.h"
#include <stdarg.h> // For variable arguments

// Global log level (default: LOG_DEBUG)
static LogLevel current_log_level = LOG_DEBUG;

// Set the minimum log level to display
void set_log_level(LogLevel level) {
    current_log_level = level;
}

// Get timestamp with milliseconds (e.g., "2024-10-31 12:00:01.123")
static const char* get_timestamp() {
    static char buffer[24]; // Holds timestamp: "YYYY-MM-DD HH:MM:SS.mmm"
    struct timeval tv;
    gettimeofday(&tv, NULL); // Get current time with microseconds

    // Convert to local time
    struct tm *t = localtime(&tv.tv_sec);

    // Format: "YYYY-MM-DD HH:MM:SS.mmm"
    snprintf(buffer, sizeof(buffer),
             "%02d:%02d:%02d.%03ld",
             t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);

    return buffer;
}

// Core logging function
void log_message(LogLevel level, const char *format, ...) {
    if (level < current_log_level) return; // Skip messages below current log level

    // Print timestamp and log level (aligned)
    printf("%s [%s] ", get_timestamp(), LOG_LEVEL_NAMES[level]);

    // Handle variable arguments (like printf)
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");

    // Exit on fatal errors
    if (level == LOG_FATAL) exit(EXIT_FAILURE);
}
