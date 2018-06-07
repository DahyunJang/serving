#ifndef SERVING_GATEWAY_SERVING_NODE_H_
#define SERVING_GATEWAY_SERVING_NODE_H_

#include <string>
#include <vector>

#include "grpc++/create_channel.h"
#include "grpc++/security/credentials.h"

#include "tnesorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"

#include "gateway/model_id.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::FilePredictRequest;
using tensorflow::serving::FilePredictResponse;
using tensorflow::serving::PredictionService;

namespace tensorflow {
namesapce serving{

    /* 한 프로세스가 관리하는 모델은 모두 base path가 같다고 가정. */
/// ServingNode holds prediction service interface
/// it caches loaded models on serving_node
class ServingNode {
public:
    ServingNode(const string& server_port, const string& model_base_path_);

    friend bool operator==(const ServingNode &a, const ServingNode& b);
    friend bool operator!=(const ServingNode &a, const ServingNode& b);
    friend bool operator<(const ServingNode& a, const ServingNode& b);

    bool IsServingModel(ModelId& model_id)
        LOCKS_EXCLUDED(mu_);

    void AddModelId(const ModelId& model_id)
        LOCKS_EXCLUDED(mu_);

    void RemoveModelId(const ModelId& model_id)
        LOCKS_EXCLUDED(mu_);

    /* add, remove 모델로 수정된 모델리스트로 config reload */
    Status ReloadModels();

    Status UpdateModelStatus(const ModelId& model_id);

    /* UpdateModelStatus for all model_id */
    Status UpdateModelStatus();

    Status FilePredict(const ModelId& model_id,
                       const string& input_data_file_path,
                       const string& output_data_file_path);

private:
    const string server_port_;
    const string model_base_path_;

    // When ServinNode destructed, stub_ will removed automatically.
    std::unique_ptr<PredictionService::Stub> stub_;

    Status HandleReloadConfigRequest_(const ReloadConfigRequest& request,
                                      ReloadConfigResponse *response);

    Stauts GetModelStatus_();

    //just forwrard request
    Status FilePredict_(const FilePredictRequest &request,
                        FilePredictResponse *response);



    //This is kind a cache of model stauts in serving node.
    mutex mu_;
    std::vector<ModelId> model_ids_ GUARDED_BY(mu_);
}


bool operator==(const ServingNode &a, const ServingNode& b){
    return (a.server_port_ == b.server_port_);
}

bool operator!=(const ServingNode &a, const ServingNode& b){
    return !(operator==(a,b));
}

bool operator<(const ServingNode& a, const ServingNode& b){
    return (a.server_port_ < b.server_port_);
}


} //serving
} // tensorflow

#endif //SERVING_GATEWAY_SERVING_NOD>E_H_
