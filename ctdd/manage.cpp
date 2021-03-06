#include "manage.hpp"

uint64_t mng::vmem_limit = DEFAULT_VMEM_LIMIT;

#ifdef __WIN__
HANDLE mng::current_process;
#endif

std::atomic<std::chrono::duration<double>> mng::garbage_check_period{ std::chrono::duration<double> {DEFAULT_MEM_CHECK_PERIOD} };
