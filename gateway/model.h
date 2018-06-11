#ifndef GATEWAY_MODEL_H_
#define GATEWAY_MODEL_H_


namespace tensorflow {
namespace serving{

class Model {
public:
    Model (const string& name, const string& model_path);
    string DebugString() const;
    const string& GetModelName() const;
    const string& GetModelPath() const;

    friend bool operator==(const Model &a, const Model& b);
    friend bool operator!=(const Model &a, const Model& b);
    friend bool operator<(const Model &a, const Model& b);

private:
    const string name_;
    const string model_path_;
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
        return std::hash<string>{}(model.GetModelName());
    }
};


}
}
#endif
