#pragma once
#include <boost/thread.hpp>
#include "lib/json.h"
#include "lib/SimpleWebServer/client_http.hpp"
#include "lib/SimpleWebServer/client_https.hpp"
#include "c_tool.h"
#include "i_global.h"
using namespace std;
using json = nlohmann::json;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;
using HttpsClient = SimpleWeb::Client<SimpleWeb::HTTPS>;
using CaseInsensitiveMultimap = SimpleWeb::CaseInsensitiveMultimap;

namespace iActive {
	void init();
	void pcActivated(string _data);
}
