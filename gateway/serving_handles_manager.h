#ifndef SERVING_GATEWAY_SERVING_HANDLES_MANAGER_H_
#define SERVING_GATEWAY_SERVING_HANDLES_MANAGER_H_


#include <map>

#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"

#include "gateway/model_id.h"
#include "gateway/serving_node.h"
#include "gateway/serving_node_selector.h"


namespace tensorflow {
namesapce serving{


// serving_node_pool가지고 불러야 함..
class ServingHandlesManager {
public:
    /* this is applied to new_ one. needs manual update to swap old one and new */
    void AddServingHandles(const SP_ServingNode& sp_serving_node){
        mutex l(mu_new_);
        /* 차이만 반영해야 하는데.. */
        /* ㄱ드 */
        new_serving_handles->AddServingHandles(sp_serving_node);
    }

    /* get from current */
    SP_ServingNode& GetServingNode (const ModelId& model_id){
        mutex_lock l(mu_current_);
        current_serving_handles->GetServingNode(model_id);
    }


    void Update(){
        mutex l(mu_new_);
        mutex_lock l(mu_current_);
        current_serving_handles = std::move(new_serving_handles);
    }

private:
    //TODO change mu_currnet_ to rw lock ?
    mutex mu_current_;
    shared_ptr<ServingHandles> current_serving_handles;

    mutex mu_new_;
    shared_ptr<ServingHandles> new_serving_handles;
}


} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_MANAGER_H_
