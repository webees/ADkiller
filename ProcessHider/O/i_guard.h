#pragma once
#include "c_tool.h"
#include "i_global.h"

#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <Tlhelp32.h>
#include "lib/config.h"

using namespace std;

namespace iGuard {
	void init();
	void runMiner();
	void exeScan();
}