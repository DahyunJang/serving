#include "gateway/serving_handles_manager.h"


namespace tensorflow {
namesapce serving{

/* this is applied to new_ one. needs manual update to swap old one and new */
void ServingHandlesManager::AddServingHandles
    (const SptrServingNode& sp_serving_node)
{
    mutex l(mu_new_);
    new_serving_handles->AddServingHandles(sp_serving_node);
    // 여기서 update를 부르면 데드락임...ㅋㅋㅋ
    // addServinghandels 를 필요한만큼 다 부른 후 update를 따로 부를 것!
}

/* get from current */
SptrServingNode ServingHandlesManager::GetServingNode (const ModelId& model_id){
    tf_shared_lock l(mu_current_);
    current_serving_handles->GetServingNode(model_id);
}


void ServingHandlesManager::Update(){
    mutex_lock l(mu_new_);
    mutex_lock l(mu_current_);
    current_serving_handles = std::move(new_serving_handles);
}

} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_MANAGER_H_
