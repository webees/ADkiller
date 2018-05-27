
#include "c_Process.h"

namespace cProcess {
	bool EnableDebugPriv() {
		HANDLE hToken;
		LUID seDebugNameValue;
		TOKEN_PRIVILEGES tkp;

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return false;

		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &seDebugNameValue)) {
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = seDebugNameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			CloseHandle(hToken);

		return true;
	}

	bool EnableShutdownPriv() {
		HANDLE hToken;
		LUID seDebugNameValue;
		TOKEN_PRIVILEGES tkp;

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return false;

		if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &seDebugNameValue)) {
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = seDebugNameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			CloseHandle(hToken);

		return true;
	}

	bool EnableBackupPriv() {
		HANDLE hToken;
		LUID seDebugNameValue;
		TOKEN_PRIVILEGES tkp;

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return false;

		if (!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &seDebugNameValue)) {
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = seDebugNameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			CloseHandle(hToken);

		return true;
	}

	bool EnableRestorePriv() {
		HANDLE hToken;
		LUID seDebugNameValue;
		TOKEN_PRIVILEGES tkp;

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return false;

		if (!LookupPrivilegeValue(NULL, SE_RESTORE_NAME, &seDebugNameValue)) {
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = seDebugNameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			CloseHandle(hToken);

		return true;
	}

	bool PsGetPidsByProcessName(std::vector<DWORD> &vDwPids, const std::string &processName) {
		vDwPids.clear();
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot) {
			return false;
		}

		PROCESSENTRY32 pe = { sizeof(pe) };
		for (BOOL fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) {
			if (!strcmpi(pe.szExeFile, processName.c_str())) {
				DWORD pid = pe.th32ProcessID;
				vDwPids.push_back(pid);
			}
		}
		CloseHandle(hSnapshot);

		if (vDwPids.size() != 0)
			return true;

		return false;
	}

	bool PsGetAllProcessName(std::vector<std::string> &vsProcessName) {
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot) {
			return false;
		}

		PROCESSENTRY32 pe = { sizeof(pe) };
		for (BOOL bOk = Process32First(hSnapshot, &pe); bOk; bOk = Process32Next(hSnapshot, &pe)) {
			vsProcessName.push_back(pe.szExeFile);
		}
		CloseHandle(hSnapshot);
		if (vsProcessName.size() != 0)
			return true;
		return false;
	}
}


