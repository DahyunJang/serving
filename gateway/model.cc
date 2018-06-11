#include "gateway/model.h"

namespace tensorflow {
namesapce serving{

Model::Model (const string& name, const string& model_path)
    :name_(name), model_path_(model_path){}

string Model::DebugString() const {
    return strings::StrCat("{name: ", name_, ", model_path:", model_path_, "}");
}

const string& Model::GetModelName(){
    return name_;
}
const string& Model::GetModelPath(){
    return model_path_;
}

}
}
