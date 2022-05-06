

#ifndef _TRIANGULATOR_INPUT_AND_OUTPUT_HPP_
#define _TRIANGULATOR_INPUT_AND_OUTPUT_HPP_

// #####################
// Log level definitions
// #####################

#define LOG_QUIET 0 // quiet
#define LOG_ERROR 1 // errors
#define LOG_WARN 2 // errors and warnings
#define LOG_INFO 3 // errors, warnings and info
#define LOG_DEBUG 4 // errors, warnings, info and debug

#endif

// #####################
// Function declarations
// #####################

void log_report(int type, string message);
void error(string message);

void save_complex_to_xml_file(SimpComp* simpComp, const string& filename);
vector<rapidxml::xml_node<>*> get_element_levels_as_xml_nodes(SimpComp* simpComp, rapidxml::memory_pool<>& mp);
