/* Copyright 2018 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow_serving/servables/tensorflow/file_predict_util.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

#include "tensorflow/cc/saved_model/signature_constants.h"
#include "tensorflow/contrib/session_bundle/signature.h"
#include "tensorflow/core/framework/tensor.pb.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/protobuf/named_tensor.pb.h"
#include "tensorflow_serving/servables/tensorflow/util.h"

namespace tensorflow {
namespace serving {
namespace {


bool ReadInputsFromFile(const string &filename, TensorMap *inputs){
    std::ifstream in_stream(filename);
    if (in_stream.is_open()){
        return inputs->ParseFromIstream(&in_stream);
    }
    return false;
}

bool WriteOutputsToFile(const string &filename, const TensorMap &outputs){
    std::ofstream out_stream(filename);
    if (out_stream.is_open()){
        return outputs.SerializeToOstream(&out_stream);
    }
    return false;
}


// Returns the keys in the map as a comma delimited string. Useful for debugging
// or when returning error messages.
// e.g. returns "key1, key2, key3".
string MapKeysToString(const google::protobuf::Map<string, tensorflow::TensorInfo>& map) {
  string result = "";
  for (const auto& i : map) {
    if (result.empty()) {
      result += i.first;
    } else {
      result += ", " + i.first;
    }
  }
  return result;
}

// Validate a SignatureDef to make sure it's compatible with prediction, and
// if so, populate the input and output tensor names.
Status PreProcessFilePrediction(const SignatureDef& signature,
                            const FilePredictRequest& request,
                                std::vector<std::pair<string, Tensor>>* inputs,
                            std::vector<string>* output_tensor_names,
                            std::vector<string>* output_tensor_aliases) {
  if (signature.method_name() != kPredictMethodName &&
      signature.method_name() != kClassifyMethodName &&
      signature.method_name() != kRegressMethodName) {
    return errors::Internal(strings::StrCat(
        "Expected prediction signature method_name to be one of {",
        kPredictMethodName, ", ", kClassifyMethodName, ", ", kRegressMethodName,
        "}. Was: ", signature.method_name()));
  }

  // Verify and prepare input.
  // get input tensor from input_file_path.
  TensorMap request_inputs;
  if (ReadInputsFromFile(request.input_file_path(), &request_inputs)){
      return tensorflow::Status(tensorflow::error::INVALID_ARGUMENT,
                                "Can't read input data file:"
                                + request.input_file_path());
  }

  // Verify and prepare input.
  if (request_inputs.tensor_map().size() != signature.inputs().size()) {
    return tensorflow::Status(tensorflow::error::INVALID_ARGUMENT,
                              "input size does not match signature");
  }
  for (auto& input : request_inputs.tensor_map()) {
    const string& alias = input.first;
    auto iter = signature.inputs().find(alias);
    if (iter == signature.inputs().end()) {
      return tensorflow::Status(
          tensorflow::error::INVALID_ARGUMENT,
          strings::StrCat("input tensor alias not found in signature: ", alias,
                          ". Inputs expected to be in the set {",
                          MapKeysToString(signature.inputs()), "}."));
    }
    Tensor tensor;
    if (!tensor.FromProto(input.second)) {
      return tensorflow::Status(tensorflow::error::INVALID_ARGUMENT,
                                "tensor parsing error: " + alias);
    }
    inputs->emplace_back(std::make_pair(iter->second.name(), tensor));
  }

  // Prepare run target.
  std::set<string> seen_outputs;
  std::vector<string> output_filter(request.output_filter().begin(),
                                    request.output_filter().end());
  for (auto& alias : output_filter) {
    auto iter = signature.outputs().find(alias);
    if (iter == signature.outputs().end()) {
      return tensorflow::Status(
          tensorflow::error::INVALID_ARGUMENT,
          strings::StrCat("output tensor alias not found in signature: ", alias,
                          " Outputs expected to be in the set {",
                          MapKeysToString(signature.outputs()), "}."));
    }
    if (seen_outputs.find(alias) != seen_outputs.end()) {
      return tensorflow::Status(tensorflow::error::INVALID_ARGUMENT,
                                "duplicate output tensor alias: " + alias);
    }
    seen_outputs.insert(alias);
    output_tensor_names->emplace_back(iter->second.name());
    output_tensor_aliases->emplace_back(alias);
  }
  // When no output is specified, fetch all output tensors specified in
  // the signature.
  if (output_tensor_names->empty()) {
    for (auto& iter : signature.outputs()) {
      output_tensor_names->emplace_back(iter.second.name());
      output_tensor_aliases->emplace_back(iter.first);
    }
  }
  return Status::OK();
}

// Validate results and populate a FilePredictResponse.
Status PostProcessFilePredictionResult(
    const SignatureDef& signature,
    const std::vector<string>& output_tensor_aliases,
    const std::vector<Tensor>& output_tensors,
    const FilePredictRequest &request, FilePredictResponse* response) {
  // Validate and return output.
  if (output_tensors.size() != output_tensor_aliases.size()) {
    return tensorflow::Status(tensorflow::error::UNKNOWN,
                              "FilePredict internal error");
  }

  TensorMap response_outputs;
  for (int i = 0; i < output_tensors.size(); i++) {
    output_tensors[i].AsProtoField(
        &((*response_outputs.mutable_tensor_map())[output_tensor_aliases[i]]));
  }
  if (!WriteOutputsToFile(request.output_file_path(), response_outputs)){
      return tensorflow::Status(tensorflow::error::INVALID_ARGUMENT,
                                "Can't write output data file:"
                                + response->output_file_path());
  }

  return Status::OK();
}

}  // namespace

Status RunFilePredict(const RunOptions& run_options,
                  const MetaGraphDef& meta_graph_def,
                  const optional<int64>& servable_version,
                  Session* session,
                  const FilePredictRequest& request,
                  FilePredictResponse* response) {
  // Validate signatures.
  const string signature_name = request.model_spec().signature_name().empty()
                                    ? kDefaultServingSignatureDefKey
                                    : request.model_spec().signature_name();
  auto iter = meta_graph_def.signature_def().find(signature_name);
  if (iter == meta_graph_def.signature_def().end()) {
    return errors::FailedPrecondition(strings::StrCat(
        "Serving signature key \"", signature_name, "\" not found."));
  }
  SignatureDef signature = iter->second;

  MakeModelSpec(request.model_spec().name(), signature_name,
                servable_version, response->mutable_model_spec());

  std::vector<std::pair<string, Tensor>> input_tensors;
  std::vector<string> output_tensor_names;
  std::vector<string> output_tensor_aliases;
  TF_RETURN_IF_ERROR(PreProcessFilePrediction(signature, request, &input_tensors,
                                          &output_tensor_names,
                                          &output_tensor_aliases));
  std::vector<Tensor> outputs;
  RunMetadata run_metadata;
  TF_RETURN_IF_ERROR(session->Run(run_options, input_tensors,
                                  output_tensor_names, {}, &outputs,
                                  &run_metadata));

  return PostProcessFilePredictionResult(signature, output_tensor_aliases, outputs,
                                         request, response);
}

}  // namespace serving
}  // namespace tensorflow
