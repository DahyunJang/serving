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
    ModelId(string& name, int64 version):
    name_(name), version_(version){}

    ModelId(const ModelSpec& model_spec):
    name_(model_spec.name()), version_(model_spec.version().value()){}


    string DebugString() const {
        return strings::StrCat("{name: ", name, " version: ", version, "}");
    }

    /* for container.. */
    bool operator==(const ModelId& b) const{
        return ((this.name_ == b.name_)
                && (this.version_ == b.version_));
    }

    /* for container.. */
    bool operator<(const ModelId& b) const{
        return ((this.name_ < b.name_)|| /* string에 comparator 구현되어 있지? */
                (this.name_ == b.name_ &&
                 this.version_ < b.version_));
    }

    bool IsSameModelName(cons){
        return
    }
    const string GetName(){
        return name_;
    }

    const int64 GetVersion(){
        return version_;
    }
private:
    const string name_;            /* model_name */
    const int64 version_;          /* model_version */
};



/// ServingNode holds prediction service interface
/// it caches loaded models on serving_node
class ServingNode {
public:

    /* 중간에 뻑나면 어떻게 처리하지? */
    /* server_port is [server ip]:[server] */
    ServingNode(const string server_port)
        :server_port_(server_port){
        stub_(PredictionService::NewStub(
                  grpc::CreateChannel(server_port,
                                      grpc::InsecureChannelCredentials())));
    }

    bool operator==(const ServingNode& b){
        return (this.server_port_ == b.server_port_)
    }

    bool operator<(const ServingNode& b){
        return (this.server_port_ < b.server_port_)
    }

    Status FilePredict(const FilePredictRequest &request,
                       FilePredictResponse *response){
        const ModelSpec& model_spec = request.model_spec();
        const Model_id model_id(model_spec);
        if (IsServingModel(model_id)){
            return  FilePredict_(request, response);
        }
        else{
            return tensorflow::errors::NotFound("Could not find the version ",
                                                model_spec.version().value(),
                                                "of model ",
                                                model_spec.name());
        }
    }

    Status LoadModel(const ModelId& model_id) {
        if (!IsServingModel(model_id)){
            Status status;

            ReloadConfigRequest request;
            ReloadConfigResponse response;

            /* //TODO build request */
            /* ModelSpec* model_spec = request.mutable_model_spec(); */
            /* model_spec->set_name(model_id.GetName()); */
            /* model_spec->mutable_version()->set_value(model_id.GetVersion()); */

            status = HandleReloadConfigRequest_(request, &response);
            //TODO response 처리 ?
        }
        else { // already servable
            return Status::OK();
        }
    }

    Status UnloadModel(const ModelId& model_id){
        if (IsServingModel(model_id)){
            Status status;

            ReloadConfigRequest request;
            ReloadConfigResponse response;
            //TODO build request
            status = HandleReloadConfigRequest_(request, &response);
            //TODO response 처리 ?
        }
        else {
            return Status::OK();
        }
    }

private:
    const string server_port_;
    // When ServinNode destructed, stub_ will removed automatically.
    std::unique_ptr<PredictionService::Stub> stub_;

    //just forwrard request
    Status FilePredict_(const FilePredictRequest &request,
                              FilePredictResponse *response){
        ClientContext context;
        return stub_->FilePredict(&context, request, response);
    }

    Status GetModelStatus_(const GetModelStatusRequest& request,
                                 GetModelStatusResponse *response){
        ClientContext context;
        return stub_->GetModelStatus(&context, reqeust, response);
    }

    Status HandleReloadConfigRequest_(const ReloadConfigRequest& request,
                                            ReloadConfigResponse *response){
        ClientContext context;
        return stub_->HandleReloadConfigRequest(&context, reqeust, response);
    }


    void AddModelId(const ModelSpec& model_spec)
        LOCKS_EXCLUDED(mu_)
    {
        mutex_lock l(mu_);
        ModelId model_id(model_spec);
        model_ids_.push_back(std::move(model_id));
    }

    void RemoveModelId(const ModelSpec& model_spec)
        LOCKS_EXCLUDED(mu_)
    {
        mutex_lock l(mu_);
        ModelId model_id(model_spec);
        model_ids_.remove(model_ids_.begin(), model_ids_.end(), model_id);
    }

    Status UpdateModelIds(void)
        LOCKS_EXCLUDED(mu_)
    {
        Status status = ;
        int model_cnt = ;
        {
            mutex_lock l(mu_);
            modle_ids_.clear();
            for (int i = 0; i < model_cnt; i++ ) {
                AddModelId
            }
        }


    }

    bool IsServingModel(ModelId& model_id)
        LOCKS_EXCLUDED(mu_)
    {
        mutex_lock l(mu_);
        model_ids_.find(model_ids_.begin(), model_ids_.end(), model_id);
    }

    //This is kind a cache of model stauts in serving node.
    mutex mu_;
    std::vector<ModelId> model_ids_ GUARDED_BY(mu_);
}






} //serving
} // tensorflow

#endif SERVING_GATEWAY_SERVING_NODE_H_
