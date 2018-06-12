#include "gateway/model.h"


namespace tensorflow {
namespace gateway{

Model::Model (const string& name, const string& model_path)
    :name_(name), model_path_(model_path){}

string Model::DebugString() const {
    return strings::StrCat(
        "{name: ", name_, ", model_path:", model_path_, "}");
}

const string& Model::GetModelName() const{
    return name_;
}
const string& Model::GetModelPath() const{
    return model_path_;
}

}
}
