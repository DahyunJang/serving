#ifndef GATEWAY_MODEL_H_
#define GATEWAY_MODEL_H_

#include <string>


namespace tensorflow {
namespace serving{

class Model {
public:
    Model (const std::string& name, const std::string& model_path);
    std::string DebugString() const;
    const std::string& GetModelName() const;
    const std::string& GetModelPath() const;

    friend bool operator==(const Model &a, const Model& b);
    friend bool operator!=(const Model &a, const Model& b);
    friend bool operator<(const Model &a, const Model& b);

private:
    const std::string name_;
    const std::string model_path_;
};


bool operator==(const Model &a, const Model& b){
    return a.name_ == b.name_;
}

bool operator!=(const Model& a, const Model& b){
    return !(operator==(a,b));
}

bool operator<(const Model& a, const Model& b){
    return a.name_.compare(b.name_) < 0;
}

struct ModelHash {
    std::size_t operator () (const Model& model) const {
        return std::hash<std::string>{}(model.GetModelName());
    }
};


}
}
#endif
