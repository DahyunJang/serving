#ifndef GATEWAY_SN_POOL_H_
#define GATEWAY_SN_POOL_H_

#include <list>
#include <algorithm>


#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/thread_annotations.h"

#include "tensorflow_serving/gateway/sn.h"

namespace tensorflow {
namespace serving{

class SNPool{
public:

    string DebugString() const;

    /* Create and add SptrSN to sp_sns_ */
    SptrSN CreateSN(const string& ip_port)
        LOCKS_EXCLUDED(mu_);

    /* Remove SptrSN from sp_sns_ and destroy by its nauture of shared ptr*/
    void DestroySN(const string& ip_port)
        LOCKS_EXCLUDED(mu_);

    /* before destroy... */
    const SptrSN GetSN(const string& ip_port)
        LOCKS_EXCLUDED(mu_);

private:
    mutex mu_;
    std::list<SptrSN> sp_sns_
        GUARDED_BY(mu_);

};

} //gateway
} //tensorflow


#endif
