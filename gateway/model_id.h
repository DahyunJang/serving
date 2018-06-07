#ifndef SERVING_GATEWAY_MODEL_ID_H_
#define SERVING_GATEWAY_MODEL_ID_H_

#include <string>


#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"

#include "gateway/serving_node_selector.h"

/* TODO  ModelId -> ServingModel로 이름 변경하기 */
/* TODO  map의 key로 사용할 건데.. 추가 작업 필요하지? */

namespace tensorflow {
namesapce serving{

Class ModelId{
public:
    ModelId(string& name);

    /* for container.. */
    friend bool operator==(const ModelId &a, const ModelId& b);
    friend bool operator!=(const ModelId &a, const ModelId& b);

    /* for container.. */
    friend bool operator<(const ModelId &a, const ModelId& b);

private:
    const string name_;            /* model_name */
};

bool operator==(const ModelId &a, const ModelId& b){
    return (a.name_ == b.name_);
}

bool operator!=(const ModelId &a, const ModelId& b){
    return !(operator==(a,b));
}

bool operator<(const ModelId &a, const ModelId& b){
    return (a.name_ < b.name_);
}

} //serving
} // tensorflow

#endif //SERVING_GATEWAY_MODEL_ID_H_
