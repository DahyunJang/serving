#ifndef GATEWAY_REQUEST_MANAGER_H_
#define GATEWAY_REQUEST_MANAGER_H_


#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/thread_annotations.h"


#include "gateway/sn_pool.h"
#include "gateway/handles.h"

namespace tensorflow {
namespace serving{


/* 음 적당한 이름이 없넹 */
/* 서버 풀에서 받아와서 로딩도 하고, 핸들 매니징도함 */
class RequestManager{
public:

    Status AddSN(const string& ip_port);
    Status RemoveSN(const string& ip_port);

    /* WARNING Don't forget to call Update to update handle!! */
    /* update only the modified result is changed (status::OK()) */
    void UpdateHandles();

    /* TODO 기존에 로드되지 않은 하나의 SN에 모델을 로드함. */
    Status LoadModel(const Model& model, SptrSN sn = nullptr);
    Status UnloadModel(const Model& model, SptrSN sn = nullptr);

    /* TODO not implemeted */
    /* unload model and load model on another SN */
    void MigrateHandle(const Model& model, SptrSN from);

    //TODO with context
    void Predict(Model& model) const;

private:
    Status AddHandleOfSN(const string& ip_port);
    Status RemoveHandleOfSN(const string& ip_port);

    SNPool sn_pool_;
    Handles handles_;
};


} //serving
} //tensorflow


#endif
