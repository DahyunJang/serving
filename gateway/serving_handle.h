#ifndef SERVING_GATEWAY_SERVING_HANDLE_H_
#define SERVING_GATEWAY_SERVING_HANDLE_H_


#include <vector>

#include "gateway/model_id.h"
#include "gateway/serving_node.h"
#include "gateway/serving_nodes.h"
#include "gateway/serving_node_selector.h"
#include "gateway/serving_node_selector_factory.h"

namespace tensorflow {
namesapce serving{

class ServingHandle{
    /* serving handel should be lock free */
    /* lockign mechanism is up on ServingHandlesmanager*/
    /* Rather than modify it, you should make new for lock-free. */
    /* TODO WANRING 모니터 만들 때도 모디파이 없이 새로 만들 것  */
public:
    ServingHandle(const ModelId& model_id);

    /* 모델 아이디에 대해 IsServing같은 체크가 없으니 사용시 유의할 것*/
    void AddServingNode(SptrServingNode sp_serving_node);
    /* TODO */
    void RemoveServingNode(const SptrServingNode& sp_serving_node);

    bool IsEmpty();

    friend bool operator==(const ServingHandle &a, const ServingHandle& b);
    friend bool operator!=(const ServingHandle &a, const ServingHandle& b);
    friend bool operator<(const ServingHandle &a, const ServingHandle& b);


private:
    const ModelId model_id_;
    ServingNodeSelector selector_;
    SptrServingNodes sp_serving_nodes_;
}


bool operator==(const ServingHandle& a, const ServingHandle& b){
    return (a.model_id_ == b.model_id_);
}

bool operator!=(const ServingHandle& a, const ServingHandle& b){
    return !(operator==(a,b));
}

bool operator<(const ServingHandle& a, const ServingHandle& b){
    return (a.modle_id_ < b.model_id_);
}


} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_H_
