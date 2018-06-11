#include "gateway/model.h"

namespace tensorflow {
namespace serving{

Model::Model (const std::string& name, const std::string& model_path)
    :name_(name), model_path_(model_path){}

std::string Model::DebugString() const {
    return "{name: " + name_ + ", model_path:" + model_path_ + "}";
}

const std::string& Model::GetModelName() const{
    return name_;
}
const std::string& Model::GetModelPath() const{
    return model_path_;
}

}
}
