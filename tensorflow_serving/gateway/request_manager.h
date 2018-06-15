#ifndef GATEWAY_REQUEST_MANAGER_H_
#define GATEWAY_REQUEST_MANAGER_H_

#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/thread_annotations.h"


#include "tensorflow_serving/gateway/sn_pool.h"
#include "tensorflow_serving/gateway/handles.h"

namespace tensorflow {
namespace serving{


/* 음 적당한 이름이 없넹 */
/* 서버 풀에서 받아와서 로딩도 하고, 핸들 매니징도함 */
class RequestManager{
public:

    bool AddSN(const string& ip_port);
    bool RemoveSN(const string& ip_port);

    /* WARNING Don't forget to call Update to update handle!! */
    /* update only the modified result is changed (status::OK()) */
    void UpdateHandles();

    /* TODO 기존에 로드되지 않은 하나의 SN에 모델을 로드함. */
    bool LoadModel(const Model& model, const string& ip_port);
    bool UnloadModel(const Model& model, const string& ip_port);

    //TODO with context
    bool Predict(const string& model_name);

private:
    bool AddHandleOfSN(const string& ip_port);
    bool RemoveHandleOfSN(const string& ip_port);

    SNPool sn_pool_;
    Handles handles_;
};


} //gateway
} //tensorflow


#endif
