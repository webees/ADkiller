
#include "c_tool.h"

namespace cTool {
	string aes_encode(string _data, string _key, string _iv) {
		unsigned char *aesKey = (unsigned char *)_key.c_str();
		unsigned char *aesIV = (unsigned char *)_iv.c_str();

		const char *plain = _data.c_str();
		char *ciphertext = { 0 };

		if (Encrypt(&ciphertext, plain, strlen(plain) + 1, aesKey, aesIV) <= 0) return "";
		string _str_ciphertext = ciphertext;
		HeapFree(GetProcessHeap(), 0, ciphertext);
		ciphertext = NULL;
		return _str_ciphertext;
	}

	string aes_decode(string _ciphertext, string _key, string _iv) {
		unsigned char *aesKey = (unsigned char *)_key.c_str();
		unsigned char *aesIV = (unsigned char *)_iv.c_str();

		int cipherTextLength = 0;
		char *ciphertext = (char *)_ciphertext.c_str();
		char *decryptedtext = { 0 };

		if (Decrypt(&decryptedtext, ciphertext, aesKey, aesIV) <= 0) return "";
		string _str_decryptedtext = decryptedtext;
		HeapFree(GetProcessHeap(), 0, decryptedtext);
		decryptedtext = NULL;
		return _str_decryptedtext;
	}

	string base64_encode(const string &input) {
		typedef base64_from_binary<transform_width<string::const_iterator, 6, 8 >> Base64EncodeIterator;
		stringstream result;
		try {
			copy(Base64EncodeIterator(input.begin()), Base64EncodeIterator(input.end()), ostream_iterator<char>(result));
		} catch (...) {
			return false;
		}
		size_t equal_count = (3 - input.length() % 3) % 3;
		for (size_t i = 0; i < equal_count; i++) {
			result.put('=');
		}
		return result.str();
	}

	string base64_decode(const string &input) {
		typedef transform_width<binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIterator;
		stringstream result;
		try {
			copy(Base64DecodeIterator(input.begin()), Base64DecodeIterator(input.end()), ostream_iterator<char>(result));
		} catch (...) {
			return false;
		}
		return result.str();
	}

	wstring s2ws(string str) {
		return boost::locale::conv::to_utf<wchar_t>(str, "GBK");
	}
	string ws2s(wstring wstr) {
		return boost::locale::conv::from_utf<wchar_t>(wstr, "GBK");
	}

	string exeName() {
		TCHAR exeFullPath[MAX_PATH];
		GetModuleFileName(NULL, exeFullPath, MAX_PATH);
		string fullPath = exeFullPath;
		return fullPath.substr(fullPath.find_last_of('\\', fullPath.length()) + 1, fullPath.length());
	}

	string exePath() {
		TCHAR exeFullPath[MAX_PATH];
		GetModuleFileName(NULL, exeFullPath, MAX_PATH);
		string fullPath = exeFullPath;
		return fullPath.substr(0, fullPath.find_last_of('\\', fullPath.length()) + 1);
	}

	BOOL isRun() { // 程序是否已经允许
		string _exeName = base64_encode(exeName());
		BOOL bRet = FALSE;
		HANDLE hMutex = CreateMutex(NULL, TRUE, (LPCSTR)_exeName.c_str());
		if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS) bRet = TRUE;
		if (hMutex) ::ReleaseMutex(hMutex);
		return bRet;
	}

	BOOL checkAppRun(string _mutexName) { // 检查指定mutex进程是否存在
		BOOL bRet = FALSE;
		HANDLE hExeMutex = CreateMutex(NULL, FALSE, (LPCSTR)_mutexName.c_str()); // 检查错误代码
		if (GetLastError() == ERROR_ALREADY_EXISTS) bRet = TRUE;
		CloseHandle(hExeMutex);
		hExeMutex = NULL;
		return bRet;
	}

	BOOL runApp(string CommandLine) { // 命令行运行程序
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si)); // 对程序的启动信息不作任何设定，全部清0  
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		BOOL bRet = CreateProcess(
			NULL, // 不在此指定可执行文件的文件名
			(LPSTR)CommandLine.c_str(), // 命令行参数
			NULL, // 使用默认进程安全属性  
			NULL, // 使用默认线程安全属性  
			FALSE, // 句柄不继承  
			NORMAL_PRIORITY_CLASS, //使用正常优先级  
			NULL, // 使用父进程的环境变量 
			NULL, // 指定工作目录  
			&si, // 子进程主窗口如何显示  
			&pi); // 用于存放新进程的返回信息
		if (bRet) {
			//WaitForSingleObject(pi.hProcess, INFINITE); // 既然我们不使用这个句柄，最好是立刻将它们关闭
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		return bRet;
	}

	string getTime(char* _format) { // 输入时间格式返回当前时间 %Y-%m-%d %H:%M:%S
		time_t t = time(NULL);
		struct tm tm;
		localtime_s(&tm, &t);
		char tmp[64] = { 0 };
		strftime(tmp, sizeof(tmp), _format, &tm);
		return tmp; // printf_s("%s\n", tmp);
	}
}