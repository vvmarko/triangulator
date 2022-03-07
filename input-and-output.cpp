
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


void save_complex_to_xml_file(SimpComp* simpComp, const string& filename)
{
    // TODO: Memory leaking problems.

    using namespace rapidxml;

    int currentMaxid = UniqueIDColor::next_free_uid_number;
    UniqueIDColor::colorize_entire_complex(simpComp);

    xml_document<> treeXml;
    xml_node<>* base = treeXml.allocate_node(node_element, simpComp->name.c_str());
    treeXml.append_node(base);

    char* nameCString = "name";
    xml_node<>* nameNode = treeXml.allocate_node(node_element, nameCString, simpComp->name.c_str());
    base->append_node(nameNode);

    char* dimensionCString = "dimension";
    const char* DCString = treeXml.allocate_string(to_string(simpComp->D).c_str(), to_string(simpComp->D).length() + 1);
    xml_node<>* dimensionNode = treeXml.allocate_node(node_element, dimensionCString, DCString);
    base->append_node(dimensionNode);

    vector<xml_node<>*> levels = get_element_levels_as_xml_nodes(simpComp, treeXml);
    for (auto nd : levels) base->append_node(nd);

    string ndStr;
    string colorTypeStr;
    string colorValStr;
    char* levelCStr = "level";
    for (unsigned int lvl = 0; lvl < simpComp->elements.size(); lvl++) {
        for (auto ks : simpComp->elements[lvl]) {
            ndStr = "ksimplex " + ks->colors.back()->get_color_value_as_str();
            char* ndCstr = treeXml.allocate_string(ndStr.c_str(), ndStr.length() + 1);
            xml_node<>* ksNd = treeXml.allocate_node(node_element, ndCstr);
            base->append_node(ksNd);

            char* lvlCStr = treeXml.allocate_string(to_string(lvl).c_str(), to_string(lvl).length() + 1);
            xml_node<>* lvlNd = treeXml.allocate_node(node_element, levelCStr, lvlCStr);
            ksNd->append_node(lvlNd);

            for (auto it = ks->colors.begin(); it + 1 != ks->colors.end(); it++) {
                colorTypeStr = to_string((*it)->type);
                char* colorTypeCstr = treeXml.allocate_string(colorTypeStr.c_str(), colorTypeStr.length() + 1);
                colorValStr = (*it)->get_color_value_as_str();
                char* colorValCstr = treeXml.allocate_string(colorValStr.c_str(), colorValStr.length() + 1);
                xml_node<>* color_nd = treeXml.allocate_node(node_element, colorTypeCstr, colorValCstr);

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


    ofstream outputFile;
    outputFile.open(filename);
    outputFile << treeXml;
    outputFile.close();
}


vector<rapidxml::xml_node<>*> get_element_levels_as_xml_nodes(SimpComp* simpComp, rapidxml::memory_pool<>& mp)
{
    using namespace rapidxml;

    vector<xml_node<>*> nodesXml;

    string lvlStr;
    string simpsStr;
    bool first;

    for (unsigned int lvl = 0; lvl < simpComp->elements.size(); lvl++) {
        // TODO: Memory leaking problems.

        lvlStr = "level " + to_string(lvl);
        char* lvlCstr = mp.allocate_string(lvlStr.c_str(), lvlStr.length() + 1);

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

        nodesXml.push_back(mp.allocate_node(node_element, lvlCstr, simpsCstr));
    }

    return nodesXml;
}
