#ifndef GATEWAY_HANDLES_H_
#define GATEWAY_HANDLES_H_

#include <map>

#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/thread_annotations.h"

#include "tensorflow_serving/gateway/sn.h"


namespace tensorflow {
namespace serving{

//using HandleMap = std::unordered_multimap<Model, SptrSN, ModelHash>;
using HandleMap = std::unordered_multimap<string, SptrSN>;

class Handles{
public:

    string DebugString() const;

    bool AddHandle(const string& model_name, SptrSN sn)
        LOCKS_EXCLUDED(mu_update_);

    bool RemoveHandle(const string& model_name, SptrSN sn = nullptr)
        LOCKS_EXCLUDED(mu_update_);

    void UpdateHandle()
        LOCKS_EXCLUDED(mu_update_) LOCKS_EXCLUDED(mu_read_);

    /* lock 잡는 것 때문에 const로 못씀.. */
    const SptrSN GetSN(const string& model_name);

private:

    /* no wirte to current_hanldes. this is read only. */
    mutex mu_read_;
    HandleMap read_handles_
        GUARDED_BY(mu_read_);

    /* no read to update_handles. this is write only. */
    mutex mu_update_;
    HandleMap update_handles_
        GUARDED_BY(mu_update_);
};

} //serving
} //tensorflow


#endif
