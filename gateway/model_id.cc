#include "model_id.h"

/* ----------------------------------------------------------------- */
ModelId::ModelId(string& name):
    name_(name){}


string ModelId::DebugString() const {
    return strings::StrCat("{name: ", name, "}");
}
