#ifndef GATEWAY_SN_H_
#define GATEWAY_SN_H_

//temporal. change it to log
#include <iostream>
#include <string>


#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/thread_annotations.h"

#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

#include "gateway/model.h"

using tensorflow::serving::PredictRequest;
using tensorflow::serving::PredictResponse;
using tensorflow::serving::PredictionService;




namespace tensorflow {
namespace serving{
// SN means "ServingNode"
class SN{
public:
    /* TODO 생성시 장애 발생 대응 */
    SN(const std::string& ip_port);
    std::string DebugString() const;
    const std::string& GetIpPort() const;

    /* load 가 실제 SN에서 로드된건 아니고 객체에 할당만 된 것. */
    bool hasModel(const Model& model);

    /* temporal */
    Status LoadModel(const Model& model)
        LOCKS_EXCLUDED(mu_);
    /* temporal */
    Status UnloadModel(const Model& model)
        LOCKS_EXCLUDED(mu_);

    /* sn 제거로 인한 모델 제거시/모니터에만 쓰여야 함. */
    /* 락 제어가 힘든 관계로 그냥 벡터를 다 복사한다.
     */
    const std::vector<Model> GetModels();

    /* const method !! */
    /* not yet implemeted */
    Status GetModelStatus(const Model& model) const;

    /* const method !! */
    /* not yet implemeted */
    Status Predict(const Model& model) const;

private:
    const std::string ip_port_;
    std::unique_ptr<PredictionService::Stub> stub_;

    mutex mu_;
    std::vector<Model> models_
        GUARDED_BY(mu_);
};

using SptrSN = std::shared_ptr<SN>;


} //serving
} //tensorflow


#endif
