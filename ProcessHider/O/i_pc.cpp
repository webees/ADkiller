
#include "i_pc.h"

namespace iPc {
	void init() {
		//net(); //网络
		cpu(); // 处理器
		//gpu(); // 显卡
		//ram(); // 内存
		//sys(); // 系统
		id(); // 唯一标识
		//conf(); // 本地配置
	}

	void conf() {
		CSimpleIni ini(true, false, false);
		ini.LoadFile(G_CONFIG_FILE.c_str());
		const char *user_id = ini.GetValue("", "user_id", "0");
		const char *pc_group_id = ini.GetValue("", "workgroup_id", "0");
		const char *remarks = ini.GetValue("", "remarks", "");
		const char *test_date = ini.GetValue("", "test_date", "");
		const char *test_remarks = ini.GetValue("", "test_remarks", "");
		G_PC["user_id"] = user_id;
		G_PC["workgroup_id"] = pc_group_id;
		G_PC["remarks"] = remarks;
		G_PC["test_date"] = test_date;
		G_PC["test_remarks"] = test_remarks;
	}

	void id() {
		printf_s("\n【ID获取】\n");
		pNetAdapterInfo* Adapters = new_pNetAdapterInfo();
		MD5 md5;
		md5.update(get_primary_pNetAdapterInfo(Adapters)->MAC + iware::cpu::model_name());
		G_PC["id"] = md5.toString();
		delete_pNetAdapterInfo(Adapters);
	}

	void cpu() {
		printf_s("\n【CPU识别】\n");
		MD5 md5;
		md5.update(iware::cpu::model_name());
		G_PC["cpu"]["id"] = md5.toString();
		G_PC["cpu"]["name"] = iware::cpu::model_name();
		G_PC["cpu"]["cores"] = iware::cpu::quantities().physical;
		G_PC["cpu"]["threads"] = iware::cpu::quantities().logical;
		G_PC["cpu"]["l2"] = iware::cpu::cache(2).size * iware::cpu::cache(2).associativity / 1024 / 1024;
		G_PC["cpu"]["l3"] = iware::cpu::cache(3).size / 1024 / 1024;
		G_PC["cpu"]["aes"] = iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t::aes);
	}

	void net() {
		json _resObj;
		string _method = "GET";
		string _url = "ip-api.com";
		string _path = "/json/?fields=25113";
		HttpClient http(_url);
		while (true) {
			try {
				auto res = http.request(_method, _path);
				_resObj = json::parse(res->content.string());
				printf_s("\n【%s】http://%s%s \n\n %s \n", _method.c_str(), _url.c_str(), _path.c_str(), _resObj.dump().c_str());
				if (_resObj["status"] == "success") break;
			} catch (...) {
				printf_s("\n【%s】【ERROR】https://%s%s \n\n", _method.c_str(), _url.c_str(), _path.c_str());
			}
			Sleep(9999);
		}
		G_PC["net_ip"] = _resObj["query"];  // IP
		G_PC["net_isp"] = _resObj["isp"]; // 运营商
		G_PC["net_country"] = _resObj["country"]; // 国家
		G_PC["net_region"] = _resObj["regionName"]; // 地区
		G_PC["net_city"] = _resObj["city"]; // 城市
	}

	void gpu() {
		const auto device_properties = iware::gpu::device_properties();
		if (!device_properties.empty()) {
			G_PC["gpu_0_name"] = device_properties[0].name;
			G_PC["gpu_0_ram"] = device_properties[0].memory_size;
			G_PC["gpu_1_name"] = device_properties[1].name;
			G_PC["gpu_1_ram"] = device_properties[1].memory_size;
		}
		const auto displays = iware::system::displays();
		if (!displays.empty()) {
			G_PC["gpu_wh"] = to_string(displays[0].width) + "x" + to_string(displays[0].height);
		}
	}

	void ram() {
		G_PC["ram_physical"] = iware::system::memory().physical_total;
	}

	void os() {
		const auto OS_info = iware::system::OS_info();
		G_PC["os_version"] = OS_info.name + " " + to_string(OS_info.major) + "." + to_string(OS_info.minor) + "." + to_string(OS_info.patch) + " build " + to_string(OS_info.build_number);
		// 获取计算机名
		const int MAX_BUFFER_LEN = 500;
		char  szBuffer[MAX_BUFFER_LEN];
		DWORD dwNameLen;
		dwNameLen = MAX_BUFFER_LEN;
		if (GetComputerName(szBuffer, &dwNameLen)) G_PC["os_pc_name"] = szBuffer;
	}
}