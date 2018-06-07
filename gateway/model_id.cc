#include "model_id.h"

/* ----------------------------------------------------------------- */
ModelId::ModelId(string& name, const ServingNodeSelectorType selector_type):
    name_(name), selector_type_(selector_type){}

ServingNodeSelectorType ModelId::GetSelectorType(){
    return selctor_type_;
}

string ModelId::DebugString() const {
    return strings::StrCat("{name: ", name, "}");
}
