
#ifndef TRIANGULATOR_RAPIDXML_PATCH_H_INCLUDED
#define TRIANGULATOR_RAPIDXML_PATCH_H_INCLUDED

namespace rapidxml
{
	namespace internal
	{
		template<class OutIt, class Ch>	inline OutIt print_children(OutIt out, const xml_node<Ch>* node, int flags, int indent);
		template<class OutIt, class Ch>	inline OutIt print_element_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
		template<class OutIt, class Ch>	inline OutIt print_data_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
		template<class OutIt, class Ch>	inline OutIt print_cdata_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
		template<class OutIt, class Ch>	inline OutIt print_declaration_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
		template<class OutIt, class Ch>	inline OutIt print_comment_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
		template<class OutIt, class Ch>	inline OutIt print_doctype_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
		template<class OutIt, class Ch>	inline OutIt print_pi_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
	}
}

#endif
