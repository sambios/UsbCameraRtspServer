# UsbCameraRtspServer

本仓库的 `live/` 目录内包含 live555 源码，CMake 会自动从源码编译 live555 依赖（UsageEnvironment、BasicUsageEnvironment、groupsock、liveMedia），无需系统预先安装 live555。

## 依赖
- **CMake** ≥ 3.2
- **GCC/Clang** 支持 C++14
- **FFmpeg 开发库**：`libavcodec`、`libavformat`、`libavutil`、`libswscale`、`libavfilter`、`libavdevice`
- **OpenSSL 开发库**：`libssl`、`libcrypto`

在 Ubuntu/Debian 上可执行：
```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential cmake pkg-config \
  libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavfilter-dev libavdevice-dev \
  libssl-dev
```

如果你的 FFmpeg 或 OpenSSL 安装在非标准路径，请设置 `PKG_CONFIG_PATH` 或在 CMake 配置时通过 `-DCMAKE_PREFIX_PATH=...` 传入额外前缀路径。

## 编译
推荐在工程内使用构建目录：
```bash
cd UsbCameraRtspServer
mkdir -p build
cmake -S . -B build
cmake --build build -j
```

编译完成后，二进制位置：`UsbCameraRtspServer/build/ffmpegH264RtspServer`

## 运行示例
当前程序支持从 H.264 裸码流文件读取，再通过 RTSP 服务输出。

- **从 H.264 文件推流**：
```bash
./build/ffmpegH264RtspServer -i /path/to/video.h264
# 或
./build/ffmpegH264RtspServer --h264file=/path/to/video.h264
```

启动成功后，可在 VLC 等播放器中打开：
```
rtsp://127.0.0.1:8554/stream
```

提示：如果你使用的是大码率/分辨率文件，建议确认本机网络和播放器缓存设置。

## 安装
本项目未提供系统级安装目标。常见做法：
- **直接使用构建产物**：将 `build/ffmpegH264RtspServer` 拷贝到你的部署目录并运行。
- 如需系统安装，可手动复制：
```bash
sudo install -m 0755 build/ffmpegH264RtspServer /usr/local/bin/
```

如果你需要 CMake `install()` 支持，请告知，我可在 `CMakeLists.txt` 中添加安装规则（包括可执行文件与头文件/库的安装）。

## 常见问题
- **链接错误找不到 FFmpeg/OpenSSL**：
  - 确认已安装对应 `-dev` 包。
  - 非标准路径安装时，使用：
    ```bash
    cmake -S . -B build -DCMAKE_PREFIX_PATH="/opt/ffmpeg;/opt/openssl"
    ```
- **`res_*` 未解析（resolver 相关）**：
  - 某些环境可能需要链接 `-lresolv`。如果出现此类错误，请反馈，我会在 CMake 中补充。

## 目录结构
- `UsbCameraRtspServer/`
  - 源码：`CameraDevice.cpp`、`FfmpegEncoder.cpp`、`FfmpegFrameSource.cpp`、`FfmpegH264LiveServerMediaSubsession.cpp`、`UsbCameraOnDemandRTSPServer.cpp`、`main.cpp`
  - 依赖：`live/`（内置 live555 源码）
  - 构建脚本：`CMakeLists.txt`

## 版权与许可
- 本项目中的 live555 相关代码遵循其各自的开源许可，详见 `live/COPYING` 与 `live/COPYING.LESSER`。
