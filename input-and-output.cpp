
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

    int current_maxid = UniqueIDColor::next_free_uid_number;
    UniqueIDColor::colorize_entire_complex(simpComp);

    xml_document<> tree_xml;
    xml_node<>* base = tree_xml.allocate_node(node_element, simpComp->name.c_str());
    tree_xml.append_node(base);

    char* nameCString = "name";
    xml_node<>* nameNode = tree_xml.allocate_node(node_element, nameCString, simpComp->name.c_str());
    base->append_node(nameNode);

    char* dimensionCString = "dimension";
    const char* DCString = tree_xml.allocate_string(to_string(simpComp->D).c_str(), to_string(simpComp->D).length() + 1);
    xml_node<>* dimensionNode = tree_xml.allocate_node(node_element, dimensionCString, DCString);
    base->append_node(dimensionNode);

    vector<xml_node<>*> levels = get_element_levels_as_xml_nodes(simpComp, tree_xml);
    for (auto nd : levels) base->append_node(nd);

    string nd_str;
    string color_type_str;
    string color_val_str;
    char* levelCStr = "level";
    for (unsigned int lvl = 0; lvl < simpComp->elements.size(); lvl++) {
        for (auto ks : simpComp->elements[lvl]) {
            nd_str = "ksimplex " + ks->colors.back()->get_color_value_as_str();
            char* nd_cstr = tree_xml.allocate_string(nd_str.c_str(), nd_str.length() + 1);
            xml_node<>* ks_nd = tree_xml.allocate_node(node_element, nd_cstr);
            base->append_node(ks_nd);

            char* lvlCStr = tree_xml.allocate_string(to_string(lvl).c_str(), to_string(lvl).length() + 1);
            xml_node<>* lvlNd = tree_xml.allocate_node(node_element, levelCStr, lvlCStr);
            ks_nd->append_node(lvlNd);

            for (auto it = ks->colors.begin(); it + 1 != ks->colors.end(); it++) {
                color_type_str = to_string((*it)->type);
                char* color_type_cstr = tree_xml.allocate_string(color_type_str.c_str(), color_type_str.length() + 1);
                color_val_str = (*it)->get_color_value_as_str();
                char* color_val_cstr = tree_xml.allocate_string(color_val_str.c_str(), color_val_str.length() + 1);
                xml_node<>* color_nd = tree_xml.allocate_node(node_element, color_type_cstr, color_val_cstr);

                ks_nd->append_node(color_nd);
            }

            vector<xml_node<>*> neighbors = get_element_levels_as_xml_nodes(ks->neighbors, tree_xml);
            for (auto nd : neighbors) ks_nd->append_node(nd);
        }
    }

    // We are done with temporary UniqueIDColors, so we delete them:
    for (auto lvl : simpComp->elements) {
        for (auto ks : lvl) {
            Color* to_be_deleted = ks->colors.back();
            ks->colors.pop_back();
            delete to_be_deleted;
        }
    }


    ofstream output_file;
    output_file.open(filename);
    output_file << tree_xml;
    output_file.close();
}


vector<rapidxml::xml_node<>*> get_element_levels_as_xml_nodes(SimpComp* simpComp, rapidxml::memory_pool<>& mp)
{
    using namespace rapidxml;

    vector<xml_node<>*> nodes_xml;

    string lvl_str;
    string simps_str;
    bool first;

    for (unsigned int lvl = 0; lvl < simpComp->elements.size(); lvl++) {
        // TODO: Memory leaking problems.

        lvl_str = "level " + to_string(lvl);
        char* lvl_cstr = mp.allocate_string(lvl_str.c_str(), lvl_str.length() + 1);

        simps_str = "";
        first = true;
        for (auto ksimp : simpComp->elements[lvl]) {
            if (!first) {
                simps_str += ",";
            }
            else first = false;
            simps_str += ksimp->colors.back()->get_color_value_as_str();      // Assuming no sorting is done.
        }
        char* simps_cstr = mp.allocate_string(simps_str.c_str(), simps_str.length() + 1);

        nodes_xml.push_back(mp.allocate_node(node_element, lvl_cstr, simps_cstr));
    }

    return nodes_xml;
}
