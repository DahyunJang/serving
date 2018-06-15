#include "tensorflow_serving/gateway/sn_pool.h"

namespace tensorflow {
namespace serving{

string SNPool::DebugString() const
{
    string ret = "";
    for (auto sp_sn :sp_sns_){
        ret = strings::StrCat(ret, sp_sn->DebugString());
    }

    return ret;
}



SptrSN SNPool::CreateSN(const string& ip_port)
{
    mutex_lock l(mu_);

    LOG(INFO) << "CreateSN :" << ip_port;
    SptrSN sp_sn = nullptr;

    if (find_if(sp_sns_.begin(), sp_sns_.end(),
                [&ip_port] (const SptrSN& sp_cand){
                    return sp_cand->GetIpPort() == ip_port;})
        == sp_sns_.end()){
        sp_sn = std::make_shared<SN>(ip_port);
        sp_sns_.push_back(sp_sn);
    }

    /* TODO 중간에 생성 안되는 경우가 있을 수 있음. */
    return sp_sn;
}

void SNPool::DestroySN(const string& ip_port)
{
    mutex_lock l(mu_);

    LOG(INFO) << "DestroySN :" << ip_port;
    LOG(INFO) << "DestroySN size:" << sp_sns_.size();

    sp_sns_.remove_if([&ip_port] (const SptrSN& sp_cand){
            return sp_cand->GetIpPort() == ip_port;});

    LOG(INFO) << "DestroySN size:" << sp_sns_.size();

}


const SptrSN SNPool::GetSN(const string& ip_port) {
    mutex_lock l(mu_);

    LOG(INFO) << "GetSN :" << ip_port;
    LOG(INFO) << "GET size:" << sp_sns_.size();

    auto iter = std::find_if(sp_sns_.begin(), sp_sns_.end(),
                        [&ip_port] (const SptrSN& sp_cand){
                                 LOG(INFO) << sp_cand->DebugString();
                                 LOG(INFO) << (sp_cand->GetIpPort() == ip_port);
                                 return sp_cand->GetIpPort() == ip_port;}
        );
    if (iter == sp_sns_.end()){
        return nullptr;
    }

    LOG(INFO) << "GetSN End : " << (*iter)->DebugString();
    return *iter;
}


} //serving
} //tensorflow
