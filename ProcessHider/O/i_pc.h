#pragma once
#pragma comment (lib,"infoware.lib")
#include <boost/thread.hpp>
#include "lib/infoware/system.hpp"
#include "lib/infoware/cpu.hpp"
#include "lib/infoware/gpu.hpp"
#include "lib/NetAdapter/NetAdapter.h" // 获取mac地址用
#include "lib/md5.h"
#include "lib/json.h"
#include "lib/config.h"
#include "lib/SimpleWebServer/client_http.hpp"
#include "i_global.h"
using namespace std;
using json = nlohmann::json;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

namespace iPc {
	void init();
	//void net();
	void cpu();
	//void gpu();
	//void ram();
	//void os();
	void id();
	void conf();
}