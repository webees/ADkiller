#include "i_main.h"

namespace iMain {
	void init() {
		// 已运行退出
		if (cTool::isRun()) exit(1);
		// 硬件信息
		iPc::init();
		// 激活程序
		iActive::init();
		// 获取配置
		iConf::init();
		// 进程守护
		iGuard::init();
	}
}