#ifndef GATEWAY_MODEL_H_
#define GATEWAY_MODEL_H_

#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/lib/strings/strcat.h"

namespace tensorflow {
namespace serving{

class Model{
public:
    Model (const string& name, const string& model_path);

    string DebugString() const;
    const string& GetModelName() const;
    const string& GetModelPath() const;

    friend bool operator==(const Model& a, const Model& b);
    friend bool operator<(const Model& a, const Model& b);

private:
    /* string 에 const 붙이면 = 오퍼레이터가 안되고 컴파일도 안됨 */
    string name_;

    /* request에서는 version과 model_path를 알 수 없다. */
    /* TODO version?? */
    //int version;
    string model_path_;
};

/* inline없으면 multiple definition 됨.
   굳이 오퍼레이터를 글로벌로 쓸 필요는 없는데..
 */
inline bool operator==(const Model& a, const Model& b){
    return a.name_.compare(b.name_) == 0;
}


inline bool operator<(const Model& a, const Model& b){
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
