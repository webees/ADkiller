#pragma once
#include <boost/thread.hpp>
#include "lib/md5.h"
#include "lib/json.h"
#include "lib/SimpleWebServer/client_http.hpp"
#include "lib/SimpleWebServer/client_https.hpp"
#include "lib/config.h"
#include "c_tool.h"
#include "i_global.h"
using namespace std;
using json = nlohmann::json;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;
using HttpsClient = SimpleWeb::Client<SimpleWeb::HTTPS>;
using CaseInsensitiveMultimap = SimpleWeb::CaseInsensitiveMultimap;
namespace iConf {
	void init();
	void upConfig();
	void upConfigFile();
	void pcConfig();
	void adList();
}
