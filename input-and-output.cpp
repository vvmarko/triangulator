
#include "triangulator.hpp"
int logLevel = 2; // default logLevel

void log_report(int type, string message){
    if(type <= logLevel){
        if(type == LOG_ERROR) cerr << "ERROR:   " << message << endl;
        if(type == LOG_WARN) cerr << "WARNING: " << message << endl;
        if(type == LOG_INFO) cerr << "INFO:    " << message << endl;
        if(type == LOG_DEBUG) cerr << "DEBUG:   " << message << endl;
    }
};

