#include "serving_node.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::FilePredictRequest;
using tensorflow::serving::FilePredictResponse;
using tensorflow::serving::PredictionService;

namespace tensorflow {
namesapce serving{


/* ----------------------------------------------------------------- */
//version should be positive value. where should I check?
ModelId::ModelId(string& name, int64 version):
name_(name), version_(version){}


ModelId::ModelId(const ModelSpec& model_spec):
name_(model_spec.name()), version_(model_spec.version().value()){}


string ModelId::DebugString() const {
    return strings::StrCat("{name: ", name, " version: ", version, "}");
}

const string ModelId::GetName(){
    return name_;
}

const int64 ModelId::GetVersion(){
    return version_;
}

/* ----------------------------------------------------------------- */

/* server_port is [server ip]:[server] */
ServingNode::ServingNode(const string server_port,
                         const string default_model_base_path)
    :server_port_(server_port),
    default_model_base_path_(default_model_base_path){
    stub_(PredictionService::NewStub(
              grpc::CreateChannel(server_port,
                                  grpc::InsecureChannelCredentials())));
}


Status ServingNode::FilePredict(const ModelId& model_id,
                                const string& input_data_file_path,
                                const string& output_data_file_path){

    //ModelId model_id = ; get modelId from
    if (IsServingModel(model_id)){
        Status status;

        FilePredictRequest request;
        FilePredictResponse response;


        // TODO REQUIRED build request and response
        ModelSpec& model_spec = request.model_spec(); //
        model_spec.set;//
            reqeust.set_input_file_path(input_data_file_path);//
            reqeust.set_output_file_path(output_data_file_path);//
        //...

        status = FilePredict_(request, response);

        // TODO REQUIRED handle response?

        return status;
    }
    else{
        return tensorflow::errors::NotFound("Could not find the version ",
                                            model_spec.version().value(),
                                            "of model ",
                                            model_spec.name());
    }
}

Status ServingNode::LoadSpecificModel(const ModelId& model_id,
                                      const string& model_base_path
                                      = default_model_base_path_) {
    /* Load only one model per once. */
    /* Only support specific load */

    if (!IsServingModel(model_id)){
        Status status;

        ReloadConfigRequest request;
        ReloadConfigResponse response;

        //request -> ModelServerConfig config ->
        //oneof Config ModelConfigList model_config_list

        ModelConfig* model_config = request.mutable_model;///


        /* TODO REQUIRED */
        model_config->set_name();
        model_config->;
        Model* model_vesion_policy = model_config->mutalbe_();//type ???
        // oneof policy_choice 처리?? -> specific-> repeated in64 versions
        model_version_policy->mutable_policy_choice()->mutable_specific()
            ->set_versions();// repeated 처리 ㅠㅠ

        status = HandleReloadConfigRequest_(request, &response);

        // TODO REQUIRED handle response?

        if (status.ok()){
            AddModelId(model_id);
        }

        return status;
    }
    else { // already servable
        return Status::OK();
    }
}


Status ServingNode::UnloadSpecificModel(const ModelId& model_id){
    if (IsServingModel(model_id)){
        Status status;

        ReloadConfigRequest request;
        ReloadConfigResponse response;

        /* Unload only one model per once. */

        //TODO REQUIRED build request
        //

        status = HandleReloadConfigRequest_(request, &response);

        // TODO REQUIRED handle response?

        if (status.ok()){
            RemoveModelId(model_id);
        }

        return status;
    }
    else {
        return Status::OK();
    }
}

//just forwrard request
Status ServingNode::FilePredict_(const FilePredictRequest &request,
                    FilePredictResponse *response){
    ClientContext context;
    return stub_->FilePredict(&context, request, response);
}


Status ServingNode::HandleReloadConfigRequest_(const ReloadConfigRequest& request,
                                  ReloadConfigResponse *response){
    ClientContext context;
    return stub_->HandleReloadConfigRequest(&context, reqeust, response);
}


void ServingNode::AddModelId(const ModelId& model_id)
{
    mutex_lock l(mu_);
    if (std::find(model_ids_.begin(), model_ids_.end(), model_id)
        == model_ids_.end()){
        model_ids_.push_back(model_id);
    }
}

void ServingNode::RemoveModelId(const ModelId& model_id)
{
    mutex_lock l(mu_);
    model_ids_.remove(model_ids_.begin(), model_ids_.end(), model_id);
}

bool ServingNode::IsServingModel(ModelId& model_id)
{
    mutex_lock l(mu_);
    model_ids_.find(model_ids_.begin(), model_ids_.end(), model_id);
}



} //serving
} // tensorflow

#endif SERVING_GATEWAY_SERVING_NODE_H_
