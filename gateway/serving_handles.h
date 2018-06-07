#ifndef SERVING_GATEWAY_SERVING_HANDLE_H_
#define SERVING_GATEWAY_SERVING_HANDLE_H_

#include <vector>
#include "gateway/serving_handle.h"

using SptrServingHandle = std::shared_ptr<ServingHandle>;

/* ServingHandles holds model-servingnode information*/
/* K: model id, V1:vectors of serving nodes , V2: selector */
class ServingHandles {
public:

    /* 기존에 model_id가 같은 ServingHandle이 없어야 함. */
    void AddServingHandle(const ModelId& model_id);

    void AddServingNodeToServingHandle(const ModelId& model_id,
                                       SptrServingNode sp_serving_node);

    void AddServingNodeToServingHandle(SptrServingNode sp_serving_node);

    SptrServingNode GetServingNode (const ModelId& model_id);

private:
    std::vector<SptrServingHandle> sp_serving_handles;

    SptrServingHandle GetServingHandle(const ModelId& model_id);
}

#endif //SERVING_GATEWAY_SERVING_HANDLE_H_
