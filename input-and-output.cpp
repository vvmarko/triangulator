
#include "triangulator.hpp"
int logLevel = LOG_WARN; // default logLevel

void log_report(int type, string message){
    if(type <= logLevel){
        if(type == LOG_ERROR) cerr << "ERROR:   " << message << endl;
        if(type == LOG_WARN) cerr << "WARNING: " << message << endl;
        if(type == LOG_INFO) cerr << "INFO:    " << message << endl;
        if(type == LOG_DEBUG) cerr << "DEBUG:   " << message << endl;
    }
};

void error(string message){
    cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nERROR:   " << message
            << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    exit(1); //TODO: Free all resources
};

void save_complex_to_xml_file(SimpComp* simpComp, const string& filename)
{
    
    // TODO: Memory leaking problems.

    using namespace rapidxml;

    int currentMaxid = UniqueIDColor::next_free_uid_number;
    UniqueIDColor::colorize_entire_complex(simpComp);

    xml_document<> treeXml;
    xml_node<>* base = treeXml.allocate_node(node_element, simpComp->name.c_str());
    treeXml.append_node(base);

    string nameString = "name";
    char* nameCString = treeXml.allocate_string(nameString.c_str(), nameString.length() + 1);
    xml_node<>* nameNode = treeXml.allocate_node(node_element, nameCString, simpComp->name.c_str());
    base->append_node(nameNode);

    string dimensionString = "dimension";
    char* dimensionCString = treeXml.allocate_string(dimensionString.c_str(), dimensionString.length() + 1);
    char* DCString = treeXml.allocate_string(to_string(simpComp->D).c_str(), to_string(simpComp->D).length() + 1);
    xml_node<>* dimensionNode = treeXml.allocate_node(node_element, dimensionCString, DCString);
    base->append_node(dimensionNode);

    
    vector<xml_node<>*> levels = get_element_levels_as_xml_nodes(simpComp, treeXml);
    for (auto nd : levels) base->append_node(nd);

    string ndStr = "ksimplex";
    char* ndCstr = treeXml.allocate_string(ndStr.c_str(), ndStr.length() + 1);

    string idStr;
    char* idCstr;
    xml_attribute<>* idAttr;
    string idAttrStr = "id";
    char* idAttrCstr = treeXml.allocate_string(idAttrStr.c_str(), idAttrStr.length() + 1);

    string colorString = "color";
    char* colorCString = treeXml.allocate_string(colorString.c_str(), colorString.length() + 1);
    string colorIdString = "color_type";
    char* colorIdCString = treeXml.allocate_string(colorIdString.c_str(), colorIdString.length() + 1);
    xml_attribute<> *color_id_attr;

    string colorTypeStr;
    string colorValStr;
    string levelString = "self_level";
    char* levelCStr = treeXml.allocate_string(levelString.c_str(), levelString.length() + 1);
    for (unsigned int lvl = 0; lvl < simpComp->elements.size(); lvl++) {
        for (auto ks : simpComp->elements[lvl]) {
            xml_node<>* ksNd = treeXml.allocate_node(node_element, ndCstr);

            idStr = ks->colors.back()->get_color_value_as_str();
            idCstr = treeXml.allocate_string(idStr.c_str(), idStr.length() + 1);
            idAttr = treeXml.allocate_attribute(idAttrCstr, idCstr);
            ksNd->append_attribute(idAttr);

            base->append_node(ksNd);

            char* lvlCStr = treeXml.allocate_string(to_string(lvl).c_str(), to_string(lvl).length() + 1);
            xml_node<>* lvlNd = treeXml.allocate_node(node_element, levelCStr, lvlCStr);
            ksNd->append_node(lvlNd);

            for (auto it = ks->colors.begin(); it + 1 != ks->colors.end(); it++) {
                colorTypeStr = to_string((*it)->type);
                char* colorTypeCstr = treeXml.allocate_string(colorTypeStr.c_str(), colorTypeStr.length() + 1);
                colorValStr = (*it)->get_color_value_as_str();
                char* colorValCstr = treeXml.allocate_string(colorValStr.c_str(), colorValStr.length() + 1);
                xml_node<>* color_nd = treeXml.allocate_node(node_element, colorCString, colorValCstr);

                color_id_attr = treeXml.allocate_attribute(colorIdCString, colorTypeCstr);
                color_nd->append_attribute(color_id_attr);

                ksNd->append_node(color_nd);
            }

            vector<xml_node<>*> neighbors = get_element_levels_as_xml_nodes(ks->neighbors, treeXml);
            for (auto nd : neighbors) ksNd->append_node(nd);
        }
    }

    // We are done with temporary UniqueIDColors, so we delete them:
    for (auto lvl : simpComp->elements) {
        for (auto ks : lvl) {
            Color* toBeDeleted = ks->colors.back();
            ks->colors.pop_back();
            delete toBeDeleted;
        }
    }
    UniqueIDColor::next_free_uid_number = currentMaxid;

    ofstream outputFile;
    outputFile.open(filename);
    outputFile << treeXml;
    outputFile.close();
}


vector<rapidxml::xml_node<>*> get_element_levels_as_xml_nodes(SimpComp* simpComp, rapidxml::memory_pool<>& mp)
{
    using namespace rapidxml;

    vector<xml_node<>*> nodesXml;

    string simpsStr;
    bool first;

    xml_node<>* currentNode;
    string lvlStr = "level";
    char* lvlCstr = mp.allocate_string(lvlStr.c_str(), lvlStr.length() + 1);

    string lvlAttrName = "lvl";
    char * lvlAttrNameCstr = mp.allocate_string(lvlAttrName.c_str(), lvlAttrName.length() + 1);
    xml_attribute<> *attr;

    for (unsigned int lvl = 0; lvl < simpComp->elements.size(); lvl++) {
        // TODO: Memory leaking problems.

        simpsStr = "";
        first = true;
        for (auto ksimp : simpComp->elements[lvl]) {
            if (!first) {
                simpsStr += ",";
            }
            else first = false;
            simpsStr += ksimp->colors.back()->get_color_value_as_str();      // Assuming no sorting is done.
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

SimpComp* read_complex_from_xml_file( const string& filepath )
{
    // TODO: Error handling.
    using namespace rapidxml;

    file<> file(filepath.c_str()); // TODO: Check for exceptions.

    xml_document<> doc;
    doc.parse<0>(file.data());

    return read_complex_from_xml_file(doc);
}

SimpComp* read_complex_from_xml_file( rapidxml::xml_document<>& doc )
{
    using namespace rapidxml;

    // Reading basic SimpComp properties
    xml_node<>* current_node = doc.first_node()->first_node();

    string sc_name = current_node->value();
    current_node = current_node->next_sibling();
    std::cout << "Parsing SimpComp " << sc_name << "." << std::endl;

    int sc_dimension = stoi(current_node->value());
    std::cout << "Complex dimension: " << sc_dimension << "." << std::endl;

    SimpComp* sc = new SimpComp(sc_name, sc_dimension);

    // Read all other nodes
    vector<size_t>* simplices;
    bool first = true;
    for (current_node = current_node->next_sibling();
         current_node != 0;
         current_node = current_node->next_sibling())
        if ((std::string) current_node->name() == "level")
            read_level_node(current_node, sc, stoi(current_node->first_attribute()->value()));
        else if ((std::string) current_node->name() == "ksimplex")
            read_ksimplex_node(current_node, sc);

    // Remove temp colors:
    for (auto &lvl: sc->elements)
        for (auto &ks: lvl)
            if (!ks->colors.empty()) ks->colors.erase(ks->colors.begin());
            // This way of removing is inefficient, and some other way should be considered

    return sc;

}



// TODO: Check if size_t is OK here.
// NOTE: Following two functions can be combined into single loop. Leaving as-is until I get a completely working version. -D.C.
// QUESTION: Coloring in these functions is dependent on implementation of functions from color.hpp header. This is bad uncoupling. Should this be changed?
void read_level_node(rapidxml::xml_node<>* node, SimpComp* sc, int level, string delimiter)
{
    vector<unsigned long>* ids = parse_level(node, delimiter);

    for (auto id: *ids) {
        if (sc->find_KSimplex(id)) throw -1; //TODO: Exception handling.
        sc->create_ksimplex(level);
        sc->elements[level].back()->colors.push_back(new UniqueIDColor(id));
    }
}

void read_ksimplex_node(rapidxml::xml_node<>* node, SimpComp* sc, string delimiter)
{
    KSimplex* current = sc->find_KSimplex(stoi(node->first_attribute()->value()));

    for (rapidxml::xml_node<>* child = node->first_node();
         child != 0;
         child = child->next_sibling())
        /*
        // This part should not be neccessary...
        if ((std::string) child->name() == "self_level") {
            if (stoi(child->value()) != current->k) throw -1; // TODO: Exception
        }*/
        if ((std::string) child->name() == "color")
            colorize_node(current, child);
        else if ((std::string) child->name() == "level") {
            vector<unsigned long>* ids = parse_level(child, delimiter);
            for (auto id: *ids)
                current->add_neighbor(sc->find_KSimplex(id));
        }

    
}

void colorize_node(KSimplex* ks, rapidxml::xml_node<>* color_node)
{
    int color_type = stoi(color_node->first_attribute()->value());
    
    // This is a horrible solution. We should think of some other way of doing this
    Color* color;
    // NOTE: This seems as a bad solution.
    switch(color_type) {
    case TYPE_BOUNDARY:
        color = new BoundaryColor(stoi(color_node->value()));
        ks->colors.push_back(static_cast<BoundaryColor*>(color));
        break;
    case TYPE_UNIQUE_ID:
        color = new UniqueIDColor(stoi(color_node->value()));
        ks->colors.push_back(static_cast<UniqueIDColor*>(color));
        break;
    case TYPE_SCREEN_COORDINATE:
        ScreenCoordinateColor* scc = new ScreenCoordinateColor();
        scc -> set_color_value_from_str(color_node->value());
        ks->colors.push_back(scc);
        break;
    }
    
}

vector<unsigned long>* parse_level(rapidxml::xml_node<>* node, string delimiter)
{
    vector<unsigned long>* ids = new vector<unsigned long>();
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
