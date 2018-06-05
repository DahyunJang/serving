#include "model_id.h"

/* ----------------------------------------------------------------- */
ModelId::ModelId(string& name, ModelStatus status):
    name_(name), status_(status){}


bool ModelId::bool IsServable(){
    return (this.status == ModelStatus::Servable);
}


void SetStatus(ModelStatus new_status){
    this.status = new_status;
}

string ModelId::DebugString() const {
    return strings::StrCat("{name: ", name, "}");
}
