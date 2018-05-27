#include "i_active.h"

namespace iActive {

	void init() {
		pcActivated(G_PC.dump()); // ×¢²á¿Í»§¶Ë
	}

	void pcActivated(string _reqData) {
		string _method = "POST";
		string _url = G_API_URL;
		string _path = G_API_VER + "/pc/activated";
		string _reqObj = "{\"cypher\":\"" + cTool::aes_encode(_reqData, G_AES_KEY, G_AES_IV) + "\"}";
		CaseInsensitiveMultimap _header;
		_header.emplace("Content-Type", "application/json");
		HttpsClient https(_url, false);
		while (true) {
			try {
				auto _res = https.request(_method, _path, _reqObj, _header);
				// Ñ°ÕÒ°µºÅ
				string _res_content = _res->content.string();
				size_t _error_idx = _res_content.find("error");
				if (_error_idx != string::npos) {
					printf_s("\n¡¾%s¡¿https://%s%s \n\n %s \n %s \n\n %s \n\n", _method.c_str(), _url.c_str(), _path.c_str(), _reqObj.c_str(), _reqData.c_str(), _res_content.c_str());
					Sleep(1000 * 60); // ÑÓ³ÙÑ­»·
				}
				size_t _cypher_idx = _res_content.find("cypher");
				if (_cypher_idx == string::npos) continue;
				//	ÆÆ½â°µºÅ
				json _resObj = json::parse(_res_content);
				string _resData = cTool::aes_decode(_resObj["cypher"].get<string>(), G_AES_KEY, G_AES_IV);
				printf_s("\n¡¾%s¡¿https://%s%s \n\n %s \n %s \n\n %s \n %s \n\n", _method.c_str(), _url.c_str(), _path.c_str(), _reqObj.c_str(), _reqData.c_str(), _resObj.dump().c_str(),  _resData.c_str());
				break;
			} catch (...) {
				printf_s("\n¡¾%s¡¿¡¾ERROR¡¿https://%s%s \n\n", _method.c_str(), _url.c_str(), _path.c_str());
			}
			Sleep(1000 * 60);
		}
	}
}