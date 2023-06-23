

#ifndef _TRIANGULATOR_INPUT_AND_OUTPUT_HPP_
#define _TRIANGULATOR_INPUT_AND_OUTPUT_HPP_

// #####################
// Log level definitions
// #####################

#define LOG_QUIET 0 // quiet
#define LOG_PANIC 1 // terrible errors (like out of memory and such)
#define LOG_ERROR 2 // errors
#define LOG_WARN 3 // errors and warnings
#define LOG_INFO 4 // errors, warnings and info
#define LOG_DEBUG 5 // errors, warnings, info and debug

#endif

// #####################
// Function declarations
// #####################

void log_report(int type, string message);
void log_report_to_cerr(int type, string message);

void save_complex_to_xml_file(SimpComp* simpComp, const string& filename);
vector<rapidxml::xml_node<>*> get_element_levels_as_xml_nodes(SimpComp* simpComp, rapidxml::memory_pool<>& mp);

SimpComp* read_complex_from_xml_file( const string& filepath );
SimpComp* read_complex_from_xml_file( rapidxml::xml_document<>& file );
void read_level_node(rapidxml::xml_node<>* node, SimpComp* sc, int level, string delimiter = ",");
void read_ksimplex_node(rapidxml::xml_node<>* node, SimpComp* sc, string delimiter = ",");
void colorize_node(KSimplex* ks, rapidxml::xml_node<>* color_node);
vector<unsigned long>* parse_level(rapidxml::xml_node<>* node, string delimiter = ",");
