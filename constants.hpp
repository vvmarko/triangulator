
int logLevel = 2;

#define LOG_QUIET 0 // quiet
#define LOG_ERROR 1 // errors
#define LOG_WARN 2 // errors and warnings
#define LOG_INFO 3 // errors, warnings and info
#define LOG_DEBUG 4 // errors, warnings, info and debug

// Special colors will have type between MIN_SPECIAL and MAX_SPECIAL:
#define MIN_SPECIAL 0
#define MAX_SPECIAL 127
// Built-in colors will have type between MIN_BUILT_IN and MAX_BUILT_IN:
#define MIN_BUILT_IN MAX_SPECIAL + 1
#define MAX_BUILT_IN 1023
// User defined colors will have type between MIN_USER_DEFINED and MAX_USER_DEFINED:
#define MIN_USER_DEFINED MAX_BUILT_IN + 1
#define MAX_USER_DEFINEED 65535

#define TYPE_BOUNDARY 0
#define TYPE_SCREEN_COORDINATE 128
#define TYPE_UNIQUE_ID 129
//#define TYPE_COUNTER 140
//#define TYPE_UNSIGNED_DOUBLE 265
//#define TYPE_TRIPLE_OF_INTEGERS 440
//#define TYPE_COORDINATES_FOR_EMBEDDING 813
