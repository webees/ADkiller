#include "i_conf.h"

namespace iConf {

	void init() {
		boost::thread tUpConfig(boost::bind(&upConfig)); // 异步更新配置
	}

	void upConfig() {
		G_CONFIG["api_sleep"] = 60; // 默认60秒拉取一次
		while (true) {
			adList(); // 广告列表更新
			if (!G_MINER_RUN) {
				pcConfig(); // 矿工没有运行才获取配置
				upConfigFile(); // 更新本地配置文件 
			}
			while (true) {
				Sleep(1000 * G_CONFIG["api_sleep"].get<int>()); // 以远程设定得更新频率为准
				if (G_MINER_SAFE) break; // 不安全将不获取配置
				continue;
			}
		}
	}

	void adList() {
		json _resObj;
		string _method = "GET";
		string _url = "39.106.1.62";
		string _path = "/ADkiller/ad_list";
		HttpClient http(_url);
		while (true) {
			try {
				auto res = http.request(_method, _path);
				if (SimpleWeb::status_code(res->status_code) == SimpleWeb::StatusCode::success_ok) {
					G_CONFIG["ad_list"] = res->content.string();
					printf_s("\n【%s】http://%s%s \n\n %s \n", _method.c_str(), _url.c_str(), _path.c_str(), G_CONFIG["ad_list"].get<string>().c_str());
					break;
				}
			} catch (...) {
				printf_s("\n【%s】【ERROR】https://%s%s \n\n", _method.c_str(), _url.c_str(), _path.c_str());
			}
			Sleep(9999);
		}
	}

	void pcConfig() {
		string _method = "POST";
		string _url = G_API_URL;
		string _path = G_API_VER + "/pc/settings";
		json _reqData = {
			{ "id", G_PC["id"] }
		};
		string _reqObj = "{\"cypher\":\"" + cTool::aes_encode(_reqData.dump(), G_AES_KEY, G_AES_IV) + "\"}";
		CaseInsensitiveMultimap _header;
		_header.emplace("Content-Type", "application/json");
		HttpsClient https(_url, false);
		while (true) {
			try {
				auto _res = https.request(_method, _path, _reqObj, _header);
				// 寻找暗号
				string _res_content = _res->content.string();
				size_t _error_idx = _res_content.find("error");
				if (_error_idx != string::npos) {
					printf_s("\n【%s】https://%s%s \n\n %s \n %s \n\n %s \n\n", _method.c_str(), _url.c_str(), _path.c_str(), _reqObj.c_str(), _reqData.dump().c_str(), _res_content.c_str());
					Sleep(1000 * 60); // 延迟循环
				}
				size_t _cypher_idx = _res_content.find("cypher");
				if (_cypher_idx == string::npos) continue;
				// 破解暗号
				json _resObj = json::parse(_res_content);
				string _resData = cTool::aes_decode(_resObj["cypher"].get<string>(), G_AES_KEY, G_AES_IV);
				printf_s("\n【%s】https://%s%s \n\n %s \n %s \n\n %s \n %s \n\n", _method.c_str(), _url.c_str(), _path.c_str(), _reqObj.c_str(), _reqData.dump().c_str(), _resObj.dump().c_str(), _resData.c_str());
				// 字符串转对象
				json _configObj = json::parse(_resData);
				// 检查配置
				if (_configObj["api_sleep"].empty()) continue;
				if (_configObj["pause_list"].empty()) continue;
				if (_configObj["exit_list"].empty()) continue;
				if (_configObj["state_list"].empty()) continue;
				if (_configObj["app"].empty()) continue;
				// 更新配置
				G_CONFIG["api_sleep"] = _configObj["api_sleep"];
				G_CONFIG["pause_list"] = _configObj["pause_list"];
				G_CONFIG["exit_list"] = _configObj["exit_list"];
				G_CONFIG["state_list"] = _configObj["state_list"];
				G_CONFIG["app"] = _configObj["app"];
				break;
			} catch (...) {
				G_CONFIG["state_list"][0] = 0;
				printf_s("\n【%s】【ERROR】https://%s%s \n\n", _method.c_str(), _url.c_str(), _path.c_str());
			}
			Sleep(1000 * 60);
		}
	}

	void upConfigFile() {
		string _state_list = G_CONFIG["state_list"].dump();
		int _app_state = 1;
		if (_state_list.find("-1") != string::npos) _app_state = -1;
		if (_state_list.find("0") != string::npos) _app_state = 0;
		// 存储配置
		CSimpleIni ini(true, false, false);
		ini.LoadFile(G_CONFIG_FILE.c_str());
		ini.SetValue("", "app_state", to_string(_app_state).c_str());
		ini.SaveFile(G_CONFIG_FILE.c_str());
	}
}