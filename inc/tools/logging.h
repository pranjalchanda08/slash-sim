/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    logging.h
 * @brief   Declarations of logging framework
 *****************************************************************************************/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> // For gettimeofday()

// Log levels
typedef enum {
    LOG_DEBUG,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} LogLevel;

// Log level names (fixed width for alignment)
static const char *LOG_LEVEL_NAMES[] = {
    "DEBUG", "WARN ", "ERROR", "FATAL"
};

// Global log level control
void set_log_level(LogLevel level);

// Core log message function
void log_message(LogLevel level, const char *format, ...);

// Macros for easier logging with the appropriate log level
#define LOG_DEBUG(fmt, ...) log_message(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  log_message(LOG_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) log_message(LOG_ERROR, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) log_message(LOG_FATAL, fmt, ##__VA_ARGS__)
