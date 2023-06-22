
#include "triangulator.hpp"
int logLevel = LOG_WARN; // default logLevel

void log_report(int type, string message){
    if(type <= logLevel){
      if(type == LOG_PANIC) cerr << "\033[31;1mPANIC:   " << message << "\033[0m" << endl;
      if(type == LOG_ERROR) cerr << "\033[31;1mERROR:   " << message << "\033[0m" << endl;
      if(type == LOG_WARN) cerr << "\033[1mWARNING: " << message << "\033[0m" << endl;
      if(type == LOG_INFO) cerr << "INFO:    " << message << endl;
      if(type == LOG_DEBUG) cerr << "DEBUG:   " << message << endl;
    }
}

void error(string message){
    cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nERROR:   " << message
            << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    exit(1); //TODO: Free all resources
}

/**
 * @brief Saves given simplicial complex to .xml file for later reading.
 * 
 * @param simpComp Simplicial complex to be saved.
 * @param filename Name of the file to be saved into. Will be created if doesn't exist, and rewritten otherwise.
 */
void save_complex_to_xml_file(SimpComp* simpComp, const string& filename)
{
    using namespace rapidxml;

    // Create unique ids to serve as identifiers for simplices
    int currentMaxid = UniqueIDColor::next_free_uid_number;
    UniqueIDColor::colorize_entire_complex(simpComp);

    // This is the document and the base xml node
    xml_document<> treeXml;
    xml_node<>* base = treeXml.allocate_node(node_element, simpComp->name.c_str());
    treeXml.append_node(base);

    /*
     * The system for saving nodes is following:
     *
     * <node_name attr_name=attr_value> node_value </node_name>
     * 
     * node_name and attr_name is the same for all the nodes of the same
     * type. Values depend on the particular structure we are trying to
     * save.
     * 
     * In general, complex is saved in the .xml file in the format:
     * 
     * <"complex_name">
     *      <name> "complex_name" </name>
     *      <dimension> "D" </dimension>
     *      <topology> "topology of the complex" </topology>
     *      [<level lvl="l"> "simplices_id_list" </level> ...]
     *      [
     *      <ksimplex id="ksimplex_ID">
     *          <self_level> "ksimplex_lvl" </self_level>
     *          [<color color_type="color_type_id"> "color_value" </color> ...]
     *          [<level lvl="lvl"> "neighbours_at_lvl" </level> ...]
     *      </ksimplex>
     *      ]
     * </complex_name>
     * 
     * Variables are denoted as "variable_name", everything else is verbatim.
    */

    // Node called name keeps the name of the complex
    string nameString = "name";
    // rapidxml uses these C-style strings:
    char* nameCString = treeXml.allocate_string(nameString.c_str(), nameString.length() + 1);
    xml_node<>* nameNode = treeXml.allocate_node(node_element, nameCString, simpComp->name.c_str());
    base->append_node(nameNode);

    // Node called dimension keeps complex's dimension
    string dimensionString = "dimension";
    char* dimensionCString = treeXml.allocate_string(dimensionString.c_str(), dimensionString.length() + 1);
    char* DCString = treeXml.allocate_string(to_string(simpComp->D).c_str(), to_string(simpComp->D).length() + 1);
    xml_node<>* dimensionNode = treeXml.allocate_node(node_element, dimensionCString, DCString);
    base->append_node(dimensionNode);

    // Node called topology keeps the topology of the complex
    string topologyString = "topology";
    // rapidxml uses these C-style strings:
    char* topologyCString = treeXml.allocate_string(topologyString.c_str(), topologyString.length() + 1);
    xml_node<>* topologyNode = treeXml.allocate_node(node_element, topologyCString, simpComp->topology.c_str());
    base->append_node(topologyNode);

    // We iterate through all the levels and get ids of simplices at that level
    vector<xml_node<>*> levels = get_element_levels_as_xml_nodes(simpComp, treeXml);
    for (auto nd : levels) base->append_node(nd);

    // Simplex node name
    string ndStr = "ksimplex";
    char* ndCstr = treeXml.allocate_string(ndStr.c_str(), ndStr.length() + 1);

    // Simplex node's id attribute
    string idStr;               // <- attr_value
    char* idCstr;
    xml_attribute<>* idAttr;
    string idAttrStr = "id";    // <- attr_name (for storing simplex's temporary id)
    char* idAttrCstr = treeXml.allocate_string(idAttrStr.c_str(), idAttrStr.length() + 1);

    // Color node
    string colorString = "color";           // <- node_name
    char* colorCString = treeXml.allocate_string(colorString.c_str(), colorString.length() + 1);
    string colorIdString = "color_type";    // <- attr_type (for storing color type id)
    char* colorIdCString = treeXml.allocate_string(colorIdString.c_str(), colorIdString.length() + 1);
    xml_attribute<> *color_id_attr;

    string colorTypeStr;                    // <- attr_value for color_type attribute
    string colorValStr;                     // <- node_value for color node
    string levelString = "self_level";      // <- node_name
    char* levelCStr = treeXml.allocate_string(levelString.c_str(), levelString.length() + 1);

    for (unsigned int lvl = 0; lvl < simpComp->elements.size(); lvl++) {    // Iterate through levels
        for (auto ks : simpComp->elements[lvl]) {                           // Iterate through complexes
            xml_node<>* ksNd = treeXml.allocate_node(node_element, ndCstr);

            // Get this simplex's temporary unique id.
            idStr = ks->colors.back()->get_color_value_as_str();
            idCstr = treeXml.allocate_string(idStr.c_str(), idStr.length() + 1);
            idAttr = treeXml.allocate_attribute(idAttrCstr, idCstr);
            ksNd->append_attribute(idAttr);

            base->append_node(ksNd);

            // Save simplex's level
            char* lvlCStr = treeXml.allocate_string(to_string(lvl).c_str(), to_string(lvl).length() + 1);
            xml_node<>* lvlNd = treeXml.allocate_node(node_element, levelCStr, lvlCStr);
            ksNd->append_node(lvlNd);

            for (auto it = ks->colors.begin(); it + 1 != ks->colors.end(); it++) {  // Iterate through colours
                colorTypeStr = to_string((*it)->type);              // <- color_type attribute value
                char* colorTypeCstr = treeXml.allocate_string(colorTypeStr.c_str(), colorTypeStr.length() + 1);
                colorValStr = (*it)->get_color_value_as_str();      // <- color node value
                char* colorValCstr = treeXml.allocate_string(colorValStr.c_str(), colorValStr.length() + 1);
                xml_node<>* color_nd = treeXml.allocate_node(node_element, colorCString, colorValCstr);

                // Adding color_id attribute:
                color_id_attr = treeXml.allocate_attribute(colorIdCString, colorTypeCstr);
                color_nd->append_attribute(color_id_attr);

                ksNd->append_node(color_nd);
            }   // Iterate through colours

            // Get node's neighbours and place them into level nodes:
            vector<xml_node<>*> neighbors = get_element_levels_as_xml_nodes(ks->neighbors, treeXml);
            for (auto nd : neighbors) ksNd->append_node(nd);
        }   // Iterate through complexes
    }       // Iterate through levels

    // We are done with temporary UniqueIDColors, so we delete them:
    for (auto lvl : simpComp->elements) {
        for (auto ks : lvl) {
          // The following assumes that colors are added at the end of the ks->colors vector
          // by the funtion UniqueIDColor::colorize_entire_complex(simpComp) from the beginning
	  // of the function. This code can be hardened to explicitly find the UniqueID colors
	  // that have id above currentMaxid, and delete those, whereever they are in the vector.
	    Color* toBeDeleted = ks->colors.back();
            ks->colors.pop_back();
            delete toBeDeleted;
        }
    }
    UniqueIDColor::next_free_uid_number = currentMaxid;

    // Close and free memory:
    ofstream outputFile;
    outputFile.open(filename);
    outputFile << treeXml;
    outputFile.close();
    treeXml.clear();
}

/**
 * @brief Create a list of level xml nodes containing the simplices at each level.
 * 
 * @details Each level node has an attribute lvl storing its dimension. IDs of simplices at
 * a given level are stored as node values.
 * 
 * @param simpComp Simplicial complex to be examined.
 * @param mp Rapidxml memory pool used for saving values.
 * @return vector<rapidxml::xml_node<>*> of nodes for each level.
 */
vector<rapidxml::xml_node<>*> get_element_levels_as_xml_nodes(SimpComp* simpComp, rapidxml::memory_pool<>& mp)
{
    using namespace rapidxml;

    vector<xml_node<>*> nodesXml;       // <- for keeping all the nodes.

    string simpsStr;                    // <- for keeping IDs of simplices.
    bool first;

    xml_node<>* currentNode;
    string lvlStr = "level";
    char* lvlCstr = mp.allocate_string(lvlStr.c_str(), lvlStr.length() + 1);

    string lvlAttrName = "lvl";
    char * lvlAttrNameCstr = mp.allocate_string(lvlAttrName.c_str(), lvlAttrName.length() + 1);
    xml_attribute<> *attr;

    for (unsigned int lvl = 0; lvl < simpComp->elements.size(); lvl++) {    // Iterate through levels
        simpsStr = "";
        first = true;
        for (auto ksimp : simpComp->elements[lvl]) {    // Iterate through neighbours.
            if (!first) {
                simpsStr += ",";
            }
            else first = false;
            simpsStr += ksimp->colors.back()->get_color_value_as_str();      // ID is the most recently added color.
        }
        char* simpsCstr = mp.allocate_string(simpsStr.c_str(), simpsStr.length() + 1);
        
        currentNode = mp.allocate_node(node_element, lvlCstr, simpsCstr);

        char* lvlAttrVal = mp.allocate_string(to_string(lvl).c_str(), to_string(lvl).length() + 1);
        attr = mp.allocate_attribute(lvlAttrNameCstr, lvlAttrVal);
        currentNode->append_attribute(attr);

        nodesXml.push_back(currentNode);
    }

    return nodesXml;
}

/**
 * @brief Reads a complex from an xml file given by the filepath.
 * 
 * @param filepath Path of the file to be read.
 * @return SimpComp* Pointer to the loaded SimpComp.
 */
SimpComp* read_complex_from_xml_file( const string& filepath )
{
    // TODO: Error handling.
    using namespace rapidxml;

    file<> file(filepath.c_str()); // TODO: Check for exceptions.

    xml_document<> doc;
    doc.parse<0>(file.data());

    return read_complex_from_xml_file(doc);
}

/**
 * @brief Reads complex from a given rapidxml document.
 * 
 * @param doc Document to be read.
 * @return SimpComp* Pointer to the loaded SimpComp.
 */
SimpComp* read_complex_from_xml_file( rapidxml::xml_document<>& doc )
{
    using namespace rapidxml;

    // Reading basic SimpComp properties
    xml_node<>* current_node = doc.first_node()->first_node();

    string sc_name = current_node->value();             // Complex name.
    current_node = current_node->next_sibling();
    //    std::cout << "Parsing SimpComp " << sc_name << "." << std::endl;

    int sc_dimension = stoi(current_node->value());     // Complex dimension.
    //    std::cout << "Complex dimension: " << sc_dimension << "." << std::endl;
    current_node = current_node->next_sibling();

    string sc_topology = current_node->value();             // Complex topology.
    //    std::cout << "Complex topology: " << sc_topology << "." << std::endl;
    
    SimpComp* sc = new(nothrow) SimpComp(sc_name, sc_dimension);
    sc->topology = sc_topology;
    
    for (current_node = current_node->next_sibling();       // Read all other nodes
         current_node != 0;
         current_node = current_node->next_sibling())
        if ((std::string) current_node->name() == "level")
            read_level_node(current_node, sc, stoi(current_node->first_attribute()->value()));
        else if ((std::string) current_node->name() == "ksimplex")
            read_ksimplex_node(current_node, sc);

    // Remove temp colors:
    for (auto &lvl: sc->elements)
        for (auto &ks: lvl)
            ks->colors.erase(ks->colors.begin());
            // NOTE: This way of removing is inefficient, and some other way should be considered

    return sc;

}



// TODO: Check if size_t is OK here.
// NOTE: Following two functions can be combined into single loop. Leaving as-is until I get a completely working version. -D.C.
// QUESTION: Coloring in these functions is dependent on implementation of functions from color.hpp header. This is bad uncoupling. Should this be changed?

/**
 * @brief Reads level node and creates simplices at level.
 * 
 * @param node Level node to be read.
 * @param sc SimpComp to add simplices to.
 * @param level Level at which to add simplices to.
 * @param delimiter Delimiter to split simplex ids. Defaults to ','.
 */
void read_level_node(rapidxml::xml_node<>* node, SimpComp* sc, int level, string delimiter)
{
    vector<unsigned long>* ids = parse_level(node, delimiter);

    for (auto id: *ids) {
        if (sc->find_KSimplex(id)) throw -1; //TODO: Exception handling.
        sc->create_ksimplex(level);
        sc->elements[level].back()->colors.push_back(new(nothrow) UniqueIDColor(id));
    }

    delete ids;
}

/**
 * @brief Reads ksimplex from node, filling its neighbours and colours.
 * 
 * @param node Node to be read from.
 * @param sc SimpComp to be written into.
 * @param delimiter Delimiter used for separating a list of neighbours. Defaults to ','.
 */
void read_ksimplex_node(rapidxml::xml_node<>* node, SimpComp* sc, string delimiter)
{
    KSimplex* current = sc->find_KSimplex(stoi(node->first_attribute()->value()));
    for (rapidxml::xml_node<>* child = node->first_node();      // Iterate through properties.
         child != 0;
         child = child->next_sibling())
        if ((std::string) child->name() == "color")
            colorize_node(current, child);
        else if ((std::string) child->name() == "level") {
            vector<unsigned long>* ids = parse_level(child, delimiter);
            for (auto id: *ids) current->add_neighbor(sc->find_KSimplex(id));
            delete ids;
        }
    
}

/**
 * @brief Uses color node to add a color to KSimplex.
 * 
 * @details Actually just a wrapper for Color::colorize_simplex_from_string().
 * 
 * @param ks KSimplex to be written into.
 * @param color_node Colour node to be read from.
 */
void colorize_node(KSimplex* ks, rapidxml::xml_node<>* color_node)
{
    int color_type = stoi(color_node->first_attribute()->value());
    string color_val = color_node->value();

    Color::colorize_simplex_from_string(ks, color_type, color_val);
}

/**
 * @brief Parses level node, returning a vector of KSimplex ids in the list.
 * 
 * @param node Level node containing a list of KSimplex ids.
 * @param delimiter Separator for the list. Defaults to ','.
 * @return vector<unsigned long>* A vector of ids.
 */
vector<unsigned long>* parse_level(rapidxml::xml_node<>* node, string delimiter)
{
    vector<unsigned long>* ids = new(nothrow) vector<unsigned long>();
    string elements = node->value();
    std::size_t pos;

    if (elements.empty()) return ids;

    while ( (pos = elements.find(delimiter)) != std::string::npos)
    {
        ids->push_back(stoi(elements.substr(0, pos)));
        elements.erase(0, pos + delimiter.length());
    }
    // Last element comes after the last delimiter:
    ids->push_back(stoi(elements.substr(0, pos)));
    
    return ids;
}
