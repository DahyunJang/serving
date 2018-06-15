#include "tensorflow_serving/gateway/handles.h"


namespace tensorflow {
namespace serving{

string Handles::DebugString() const
{
    string ret = "{{update_handles: ";
    for (auto handle :update_handles_){
        ret = strings::StrCat(ret, handle.first.DebugString(),
                              handle.second->DebugString(), ", ");
    }

    ret = strings::StrCat(ret, "}, {read_handles: ");

    for (auto handle :read_handles_){
        ret = strings::StrCat(ret, handle.first.DebugString(),
                              handle.second->DebugString(), ", ");
    }

    ret = strings::StrCat(ret, "}}");
    return ret;
}

bool Handles::AddHandle(const Model& model, SptrSN sn)
{
    mutex_lock l(mu_update_);
    //LOG(INFO) << "AddHandle " << model.DebugString() << sn->DebugString();
    /*
      mutlimap은 k-v가 완전히 같아도 들어가므로 체크 필요
    */
    auto range = update_handles_.equal_range(model);
    for(auto i = range.first; i != range.second; i++){
        if (i->second->GetIpPort() == sn->GetIpPort()){
            return false;
        }
    }

    update_handles_.insert(std::make_pair(model, sn));
    return true;
}



bool Handles::RemoveHandle(const Model& model, SptrSN sn)
{
    mutex_lock l(mu_update_);

    if (sn == nullptr){
        //LOG(INFO) << "RemoveHandle " << model.DebugString();
        update_handles_.erase(model);
        return true;//
    }
    else {
        //LOG(INFO) << "RemoveHandle " << model.DebugString() << sn->DebugString();
        auto range = update_handles_.equal_range(model);
        for(auto i = range.first; i != range.second; i++){
            if (i->second->GetIpPort() == sn->GetIpPort()){
                update_handles_.erase(i);
                return true;
            }
        }
    }

    return false;
}

void Handles::UpdateHandle()
{
    //LOG(INFO) << "UpdateHandle";
    /*
      read_handles에 update_handles를 복사하면되는데,
      락타임 최소화를 위해서 미리 카피를 한 뒤에 스왑.
      정상 동작하는지 확인하자..
    */
    /* copy contents memory to update_copy
       컨테이너 엘리먼트들은 힙에 있음...
    */
    mutex_lock l(mu_update_);
    HandleMap update_copy = update_handles_;

    /* update */
    {
        mutex_lock l(mu_read_);
        /* move copied contents to read_handles_ */
        /* swap은 무브로 구현됨. */
        read_handles_.swap(update_copy);
    }
}

const SptrSN Handles::GetSN(const Model& model)
{
    tf_shared_lock ls(mu_read_);

    //LOG(INFO) << "GetSN " << model.DebugString();

    auto range = read_handles_.equal_range(model);

    /* TODO 일단 모델은 하나만 로딩되는 것으로 가정하긴 함.
       select random? rr? 정책 없음
       state기반 변경이면 카운터가 있어야 하는데 모델에 넣어놔도 되나?
       암튼 나중에 수정...
       this is temporal code.
    */
    for(auto i = range.first; i != range.second; i++){
        return i->second;
    }

    return nullptr;
}

} //gateway
} //tensorflow
