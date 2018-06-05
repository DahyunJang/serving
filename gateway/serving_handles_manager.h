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

    /* TEST 신경써서 할 것! */
/* ServingHandlesManager updates  */
// serving_node_pool가지고 불러야 함..
class ServingHandlesManager {
public:
    /* this is applied to new_ one. needs manual update to swap old one and new */
    /* Must call Update function after AddServingHandles to read new information */
    void AddServingHandles(const SptrServingNode& sp_serving_node)
        LOCKS_EXCLUDED(mu_new_);

    /* get from current */
    /*
       참고로 thread_annotation은 컴파일 체크용이고
       mu_current_는 shared로 걸리는데,
       내부에서 락을 잡으므로 priviously required는 아니고(shared_locks_required)
       다른 데서도 미리 락을 잡을 수 있으므로 locks_excluded도 필요 없음.
     */
    SptrServingNode GetServingNode (const ModelId& model_id);


    /* move new_serving_handles to currenst_serving_handles */
    void Update()
        LOCKS_EXCLUDED(mu_new_); // locks_excluded(mu_current_)?

private:

    /* nsync 라는 구글 락 라이브러리가 있음.
       tensorflow/core/platform/default/mutex.h 참고하면
       tensorflow에서 mutex_lock-> w 모드,
       tf_shared_lock -> r 모드 락으로 래퍼함.
     */
    mutex mu_current_;
    shared_ptr<ServingHandles> current_serving_handles
        GUARDED_BY(mu_current_);

    mutex mu_new_;
    shared_ptr<ServingHandles> new_serving_handles
        GUARDED_BY(mu_new_);
}


} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_MANAGER_H_
