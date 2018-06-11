#ifndef GATEWAY_MODEL_H_
#define GATEWAY_MODEL_H_

#include <string>
#include "tensorflow/core/lib/strings/strcat.h"


namespace tensorflow {
namespace serving{

class Model {
public:
    Model (const string& name, const string& model_path);
    string DebugString() const;
    const string& GetModelName();
    const string& GetModelPath();

    friend bool operator==(const Model &a, const Model& b);
    friend bool operator!=(const Model &a, const Model& b);
    friend bool operator<(const Model &a, const Model& b);

private:
    const string name_;
    const string model_path_;
}


bool operator==(const Model &a, const Model& b){
    return a.name_ == b.name_;
}

bool operator!=(const Model& a, const Model& b){
    return !(operator==(a,b));
}

bool operator<(const Model& a, const Model& b){
    return a.name_.compare(b) < 0;
}

struct ModelHash {
    std::size_t operator () (const Model& model) const {
        return std::hash<std::string>{}(model.GetModelName());
    }
}


}
}
#endif
