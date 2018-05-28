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


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::FilePredictRequest;
using tensorflow::serving::FilePredictResponse;
using tensorflow::serving::PredictionService;

namespace tensorflow {
namesapce serving{

Class ModelId{
public:
    ModelId(string& name, int64 version);

    ModelId(const ModelSpec& model_spec);


    /* for container.. */
    friend bool operator==(const ModelId &a, const ModelId& b);

    /* for container.. */
    friend bool operator<(const ModelId &a, const ModelId& b);

    const string GetName();

    const int64 GetVersion();

private:
    const string name_;            /* model_name */
    const int64 version_;          /* model_version */
};

bool operator==(const ModelId &a, const ModelId& b){
    return ((a.name_ == b.name_)
            && (a.version_ == b.version_));
}

bool operator<(const ModelId &a, const ModelId& b){
    return ((a.name_ < b.name_)|| /* string에 comparator 구현되어 있지? */
            (a.name_ == b.name_ &&
             a.version_ < b.version_));
}


/* ----------------------------------------------------------------------- */

/// ServingNode holds prediction service interface
/// it caches loaded models on serving_node
class ServingNode {
public:
    ServingNode(const string server_port);

    friend bool operator==(const ServingNode &a, const ServingNode& b);

    friend bool operator<(const ServingNode& a, const ServingNode& b);

    Status FilePredict(const FilePredictRequest &request,
                       FilePredictResponse *response);


    /* TODO Make sure the model server support this api.
       tensorflow only support latest version load in 'server_core'.
     */
    Status LoadSpecificModel(const ModelId& model_id);

    Status UnloadSpecificModel(const ModelId& model_id);

    /* TODO  load multiple models at once. */
    /* after call hadnel reaload config request if response containse error,
       should check model status before add model_ids_
     */

    /* TODO model stauts monitor
       There could be error of stub or of model stauts.
       monitoring function maybe needed.
     */
private:
    const string server_port_;
    const string default_model_base_path_;

    // When ServinNode destructed, stub_ will removed automatically.
    std::unique_ptr<PredictionService::Stub> stub_;

    //just forwrard request
    Status FilePredict_(const FilePredictRequest &request,
                        FilePredictResponse *response);

    Status HandleReloadConfigRequest_(const ReloadConfigRequest& request,
                                      ReloadConfigResponse *response);


    void AddModelId(const ModelId& model_id) LOCKS_EXCLUDED(mu_);

    void RemoveModelId(const ModelId& model_id) LOCKS_EXCLUDED(mu_);

    bool IsServingModel(ModelId& model_id) LOCKS_EXCLUDED(mu_);

    //This is kind a cache of model stauts in serving node.
    mutex mu_;
    std::vector<ModelId> model_ids_ GUARDED_BY(mu_);
}


bool operator==(const ServingNode &a, const ServingNode& b){
    return (a.server_port_ == b.server_port_);
}

bool operator<(const ServingNode& a, const ServingNode& b){
    return (a.server_port_ < b.server_port_);
}




} //serving
} // tensorflow

#endif SERVING_GATEWAY_SERVING_NODE_H_
