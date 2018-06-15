#include <gtest/gtest.h>

#include "tensorflow_serving/gateway/handles.h"

// 멀티 스레드 버전은 아님


namespace tensorflow{
namespace serving{


    TEST(Handles, Add) {
        Handles handles;

        const string ip_port1 = "192.168.4.19:8080";
        const string ip_port2 = "192.168.4.19:8081";

        auto sn1 = std::make_shared<SN>(ip_port1);
        auto sn1_2 = std::make_shared<SN>(ip_port1);
        auto sn2 = std::make_shared<SN>(ip_port2);

        const string model1 = "model1";
        const string model2 = "model2";


        // add
        EXPECT_EQ(handles.AddHandle(model1, sn1), true);
        EXPECT_EQ(handles.AddHandle(model1, sn1), false);
        EXPECT_EQ(handles.AddHandle(model1, sn2), true);
        EXPECT_EQ(handles.AddHandle(model1, sn1), false);

        EXPECT_EQ(handles.AddHandle(model2, sn1), true);
        EXPECT_EQ(handles.AddHandle(model2, sn1_2), false);
        EXPECT_EQ(handles.AddHandle(model2, sn2), true);
    }

    TEST(Handles, RemoveWithSn) {
        Handles handles;

        const string ip_port1 = "192.168.4.19:8080";
        const string ip_port2 = "192.168.4.19:8081";

        auto sn1 = std::make_shared<SN>(ip_port1);
        auto sn1_2 = std::make_shared<SN>(ip_port1);
        auto sn2 = std::make_shared<SN>(ip_port2);

        const string model1 = "model1";
        const string model2 = "model2";

        // remove
        EXPECT_EQ(handles.RemoveHandle(model1, sn1), false);
        EXPECT_EQ(handles.AddHandle(model1, sn1), true);
        EXPECT_EQ(handles.RemoveHandle(model1, sn1), true);
        EXPECT_EQ(handles.AddHandle(model1, sn1), true);
        EXPECT_EQ(handles.RemoveHandle(model1, sn1), true);
        EXPECT_EQ(handles.RemoveHandle(model1, sn1), false);
    }


    TEST(Handles, RemoveWithoutSn) {
        Handles handles;

        const string ip_port1 = "192.168.4.19:8080";
        const string ip_port2 = "192.168.4.19:8081";

        auto sn1 = std::make_shared<SN>(ip_port1);
        auto sn1_2 = std::make_shared<SN>(ip_port1);
        auto sn2 = std::make_shared<SN>(ip_port2);

        const string model1 = "model1";
        const string model2 = "model2";

        // remove
        EXPECT_EQ(handles.RemoveHandle(model1), true);
        EXPECT_EQ(handles.RemoveHandle(model2), true);
        EXPECT_EQ(handles.AddHandle(model1, sn1), true);
        EXPECT_EQ(handles.AddHandle(model1, sn2), true);
        EXPECT_EQ(handles.RemoveHandle(model1, sn1), true);
        EXPECT_EQ(handles.AddHandle(model1, sn1), true);
        EXPECT_EQ(handles.AddHandle(model1, sn2), false);
        EXPECT_EQ(handles.RemoveHandle(model1), true);
        EXPECT_EQ(handles.RemoveHandle(model1,nullptr), true);
        EXPECT_EQ(handles.AddHandle(model1, sn2), true);
    }



    TEST(Handles, UpdateGetSN) {
        Handles handles;

        const string ip_port1 = "192.168.4.19:8080";
        const string ip_port2 = "192.168.4.19:8081";

        auto sn1 = std::make_shared<SN>(ip_port1);
        auto sn1_2 = std::make_shared<SN>(ip_port1);
        auto sn2 = std::make_shared<SN>(ip_port2);
        const string model1 = "model1";
        const string model2 = "model2";

        // add
        EXPECT_EQ(handles.AddHandle(model1, sn1), true);
        EXPECT_EQ(handles.AddHandle(model1, sn2), true);
        EXPECT_EQ(handles.AddHandle(model2, sn1), true);

        EXPECT_EQ(handles.GetSN(model1), nullptr);
        EXPECT_EQ(handles.GetSN(model2), nullptr);
        handles.UpdateHandle();

        EXPECT_EQ(handles.GetSN(model1), sn2); // last inserted
        EXPECT_EQ(handles.GetSN(model2), sn1);
    }

}
}
