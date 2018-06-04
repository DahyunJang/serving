#include "serving_node_selector_factory.h"

static std::unique_ptr<ServingNodeSelector>
ServingNodeSelectorFactory::Create(ServingNodeSelectorType type){

    unique_ptr<ServingNodeSelector> selector = nullptr;
    switch(type) {
    case ServingNodeSelectorType::RR :
        selector = make_unique<ServingNodeSelectorRR>();
        break;
    }

    return selector;
}
