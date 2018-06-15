#ifndef GATEWAY_SN_H_
#define GATEWAY_SN_H_

//temporal. change it to log
#include <list>

#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/thread_annotations.h"

#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

#include "tensorflow_serving/gateway/model.h"

using tensorflow::serving::PredictRequest;
using tensorflow::serving::PredictResponse;
using tensorflow::serving::PredictionService;




namespace tensorflow {
namespace serving{
// SN means "ServingNode"
class SN{
public:
    /* TODO 생성시 장애 발생 대응 */
    SN(const string& ip_port);

    /* 스텁 없는 객체임.
       ip_port로 키 비교를 하려고 만들어 놨음 ㅠㅠ
       이게 없으면 키 compare용 객체 만들 때 마다 stub이 생겨서.
     */
    SN(const string& ip_port, bool is_dummy);

    ~SN();

    string DebugString() const;

    const string& GetIpPort() const;

    /* load 가 실제 SN에서 로드된건 아니고 객체에 할당만 된 것. */
    bool hasModel(const Model& model);

    /* temporal */
    Status LoadModel(const Model& model)
        LOCKS_EXCLUDED(mu_);
    /* temporal */
    Status UnloadModel(const Model& model)
        LOCKS_EXCLUDED(mu_);

    /* sn 제거로 인한 모델 제거시/모니터에만 쓰여야 함. */
    /* 락 제어가 힘든 관계로 그냥 다 복사한다.
     */
    const std::list<Model> GetModels();

    /* const method !! */
    /* not yet implemeted */
    Status GetModelStatus(const Model& model) const;

    /* const method !! */
    /* not yet implemeted */
    Status Predict(const Model& model) const;

    friend bool operator==(const SN& a, const SN& b);
    friend bool operator<(const SN& a, const SN& b);

private:
    string ip_port_;
    std::unique_ptr<PredictionService::Stub> stub_;

    mutex mu_;
    std::list<Model> models_
        GUARDED_BY(mu_);
};


inline bool operator==(const SN &a, const SN& b){
    return a.ip_port_.compare(b.ip_port_) == 0;
}


inline bool operator<(const SN& a, const SN& b){
    return a.ip_port_.compare(b.ip_port_) < 0;
}


using SptrSN = std::shared_ptr<SN>;


} //serving
} //tensorflow


#endif
