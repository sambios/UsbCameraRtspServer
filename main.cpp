#include "CameraDevice.hpp"
//#include "opencv2/opencv.hpp"
#include "UsbCameraOnDemandRTSPServer.h"

int main(int argc, char *argv[])
{
    /*CameraDevice dev;
    dev.Open("dshow", "video=Integrated Camera", 640, 480, 30);
    while (1) {

        AVFrame *frame = dev.Capture();
        cv::Mat image = avframe_to_cvmat(frame);
        cv::imshow("test", image);
        cv::waitKey(20);
    }*/

    UsbCameraOnDemandRTSPServer server;
#ifdef _WIN32
    server.SetH264File("c:\\hsyuan\\testfiles\\yanxi-1080p.264");
#else
    server.SetH264File("~/testfiles/yanxi-720-2m.264");
#endif 
    server.StartServer();
    

    return 0;
}
