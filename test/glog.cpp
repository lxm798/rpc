#include <glog/logging.h>

int main(int argc,char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO,"./myInfo_"); 
    FLAGS_logtostderr = false;
    LOG(INFO) << "Hello,GLOG!";
    return 0;
}
