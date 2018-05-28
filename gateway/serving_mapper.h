#ifndef SERVING_GATEWAY_SERVING_MAPPER_H_
#define SERVING_GATEWAY_SERVING_MAPPER_H_


#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"

#include "gateway/serving_node.h"
#include "gateway/serving_node_pool.h"
#include "gateway/serving_mapper.h"



namespace tensorflow {
namesapce serving{


// For now, assume there is only one node for one model


class ServingMapper {
public:

    Status AddServingPair (ServingNode& serving_node, ModelSpec &model_spec){
        // add itself to serving_map_
    }
    //remove itself from serving_map_
    Status RemoveServingPair (ServingNode& serving_node, ){

    }

    Status AddServingPairOfNode (ServingNode& serving_node) {
        for () { //for each model in serving node
            if () { // if it is not in serving map
                AddServingPair(serving_node, model_spec);
            }
        }
    }

    Status RemoveServingPairOfNode (const ServingNode& serving_node){

    }

    ServingNode GetServingNode (const ModelId& model_id){

    }

private:
    //TODO
    map<> serving_pair;

}



} //namespace serving
} //namespace tensorflow

#endif SERVING_GATEWAY_SERVING_MAPPER_H_
