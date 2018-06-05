#include "serving_node_selector_factory.h"

static ServingNodeSelector
ServingNodeSelectorFactory::CreateByType(ServingNodeSelectorType type){
    switch(type) {
    case ServingNodeSelectorType::RR :
        return ServingNodeSelectorRR();
    }
}
