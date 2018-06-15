#include <gtest/gtest.h>

#include "tensorflow_serving/gateway/request_manager.h"


namespace tensorflow{
namespace serving{

TEST(RequestManager, AddSNAndRemvoeSN) {
    RequestManager rqm;

    const string ip_port1 = "192.168.4.19:8080";
    const string ip_port2 = "192.168.4.19:8081";
    const string ip_port3 = "192.168.4.19:8082";

    EXPECT_EQ(rqm.AddSN(ip_port1), true);
    EXPECT_EQ(rqm.AddSN(ip_port2), true);

    EXPECT_EQ(rqm.RemoveSN(ip_port2), true);
    EXPECT_EQ(rqm.RemoveSN(ip_port3), false);
    EXPECT_EQ(rqm.AddSN(ip_port3), true);
    EXPECT_EQ(rqm.RemoveSN(ip_port3), true);
    EXPECT_EQ(rqm.RemoveSN(ip_port1), true);
    EXPECT_EQ(rqm.RemoveSN(ip_port1), false);
}


TEST(RequestManager, LoadUnloadSN) {
    RequestManager rqm;

    const string ip_port1 = "192.168.4.19:8080";
    const string ip_port2 = "192.168.4.19:8081";
    const string ip_port3 = "192.168.4.19:8082";
    const string model_name1 = "model1";
    const string model_path1 = "model1 path";
    const string model_name2 = "model2";
    const string model_path2 = "model2 path";
    const Model model1(model_name1, model_path1);
    const Model model2(model_name2, model_path2);


    EXPECT_EQ(rqm.AddSN(ip_port1), true);
    EXPECT_EQ(rqm.AddSN(ip_port2), true);

    EXPECT_EQ(rqm.LoadModel(model1, ip_port1), true);
    EXPECT_EQ(rqm.LoadModel(model1, ip_port2), true);
    EXPECT_EQ(rqm.LoadModel(model2, ip_port2), true);
    EXPECT_EQ(rqm.LoadModel(model2, ip_port3), false);

    EXPECT_EQ(rqm.UnloadModel(model1, ip_port2), true);
    EXPECT_EQ(rqm.UnloadModel(model2, ip_port2), true);
    EXPECT_EQ(rqm.UnloadModel(model2, ip_port3), false);
    EXPECT_EQ(rqm.LoadModel(model1, ip_port2), true);
    EXPECT_EQ(rqm.LoadModel(model2, ip_port2), true);

}

TEST(RequestManager, UpdateAndPredict) {
    RequestManager rqm;

    const string ip_port1 = "192.168.4.19:8080";
    const string ip_port2 = "192.168.4.19:8081";
    const string ip_port3 = "192.168.4.19:8082";
    const string model_name1 = "model1";
    const string model_path1 = "model1 path";
    const string model_name2 = "model2";
    const string model_path2 = "model2 path";
    const Model model1(model_name1, model_path1);
    const Model model2(model_name2, model_path2);

    EXPECT_EQ(rqm.AddSN(ip_port1), true);
    EXPECT_EQ(rqm.LoadModel(model1, ip_port1), true);

    EXPECT_EQ(rqm.Predict(model_name1), false);
    EXPECT_EQ(rqm.Predict(model_name2), false);

    rqm.UpdateHandles();

    EXPECT_EQ(rqm.Predict(model_name1), true);
    EXPECT_EQ(rqm.Predict(model_name2), false);
}

}
}
