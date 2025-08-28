
#include "triangulator.hpp"

namespace triangulator_global{
    // Vector of all instantiated simplicial complexes:
    vector<SimpComp*> seededComplexes;

    string logFilename = "logfile.html"; // default logfile name
    int logLevel = LOG_WARN; // default loglevel

}

