#include "CameraDevice.hpp"
#include "UsbCameraOnDemandRTSPServer.h"
#include "getopt.h"

int main(int argc, char *argv[])
{
    std::string h264file;
    int ch = 0, option_index;
    struct option long_options[] =
            {

                    {"h264file", optional_argument, NULL, 0},
                    {0,          0, 0,                    0},

            };

    while ((ch = getopt_long(argc, argv, "i:", long_options, &option_index)) != -1)
    {

        switch (ch)
        {
            case 0:
                if (optarg) {
                    if (option_index == 0) {
                        h264file = optarg;
                    }
                }

                break;
            case 'i':
                h264file = optarg;
                break;
        }
    }

    UsbCameraOnDemandRTSPServer server;
#ifdef _WIN32
    server.SetH264File(h264file);
#else
    server.SetH264File(h264file);
#endif 
    server.StartServer();
    

    return 0;
}
