#pragma once
#include "stdafx.h"
#include "tdd.hpp"

namespace mng {

	inline void print_resource_state() {
		std::cout << std::endl;
		auto all_tdds_w = tdd::TDD<wcomplex>::get_all_tdds();
		std::cout << "wcomplex tdd number: " << all_tdds_w.size() << std::endl;
		auto table_w = node::Node<wcomplex>::get_unique_table();
		int ref_max = 0, ref_min = (std::numeric_limits<int>::max)();
		int zero_ref_count = 0;
		for (auto&& pair : table_w) {
			auto count = pair.second->get_ref_count();
			ref_max = ref_max <= count ? count : ref_max;
			ref_min = ref_min >= count ? count : ref_min;
			if (count == 0) zero_ref_count++;
		}
		std::cout << "wcomplex node number: " << table_w.size() << std::endl;
		std::cout << "wcomplex max reference: " << ref_max << std::endl;
		std::cout << "wcomplex min reference: " << ref_min << std::endl;
		std::cout << "wcomplex 0-ref node number: " << zero_ref_count << std::endl;



		auto all_tdds_t = tdd::TDD<CUDAcpl::Tensor>::get_all_tdds();
		std::cout << "CUDAcpl::Tensor tdd number: " << all_tdds_t.size() << std::endl;
		auto table_t = node::Node<CUDAcpl::Tensor>::get_unique_table();
		ref_max = 0;
		ref_min = (std::numeric_limits<int>::max)();
		zero_ref_count = 0;
		for (auto&& pair : table_t) {
			auto count = pair.second->get_ref_count();
			ref_max = ref_max <= count ? count : ref_max;
			ref_min = ref_min >= count ? count : ref_min;
			if (count == 0) zero_ref_count++;
		}
		std::cout << "CUDAcpl::Tensor node number: " << table_t.size() << std::endl;
		std::cout << "CUDAcpl::Tensor max reference: " << ref_max << std::endl;
		std::cout << "CUDAcpl::Tensor min reference: " << ref_min << std::endl;
		std::cout << "CUDAcpl::Tensor 0-ref node number: " << zero_ref_count << std::endl;
		std::cout << std::endl;
	}

	inline void clear_garbage() {
		cache::clean_garbage(cache::Global_Cache<wcomplex>::sum_cache);
		cache::clean_garbage(cache::Global_Cache<wcomplex>::trace_cache);
		cache::clean_garbage(cache::Cont_Cache<wcomplex, wcomplex>::cont_cache);
		cache::clean_garbage(cache::Cont_Cache<wcomplex, CUDAcpl::Tensor>::cont_cache);
		cache::clean_garbage(cache::Global_Cache<CUDAcpl::Tensor>::sum_cache);
		cache::clean_garbage(cache::Global_Cache<CUDAcpl::Tensor>::trace_cache);
		cache::clean_garbage(cache::Cont_Cache<CUDAcpl::Tensor, wcomplex>::cont_cache);
		cache::clean_garbage(cache::Cont_Cache<CUDAcpl::Tensor, CUDAcpl::Tensor>::cont_cache);
		node::Node<wcomplex>::clean_garbage();
		node::Node<CUDAcpl::Tensor>::clean_garbage();
	}

	inline void clear_cache() {
		// wcomplex
		cache::Global_Cache<wcomplex>::CUDAcpl_cache.first.lock();
		cache::Global_Cache<wcomplex>::CUDAcpl_cache.second.clear();
		cache::Global_Cache<wcomplex>::CUDAcpl_cache.first.unlock();

		cache::Global_Cache<wcomplex>::sum_cache.first.lock();
		cache::Global_Cache<wcomplex>::sum_cache.second.clear();
		cache::Global_Cache<wcomplex>::sum_cache.first.unlock();


		cache::Global_Cache<wcomplex>::trace_cache.first.lock();
		cache::Global_Cache<wcomplex>::trace_cache.second.clear();
		cache::Global_Cache<wcomplex>::trace_cache.first.unlock();

		cache::Cont_Cache<wcomplex, wcomplex>::cont_cache.first.lock();
		cache::Cont_Cache<wcomplex, wcomplex>::cont_cache.second.clear();
		cache::Cont_Cache<wcomplex, wcomplex>::cont_cache.first.unlock();

		cache::Cont_Cache<wcomplex, CUDAcpl::Tensor>::cont_cache.first.lock();
		cache::Cont_Cache<wcomplex, CUDAcpl::Tensor>::cont_cache.second.clear();
		cache::Cont_Cache<wcomplex, CUDAcpl::Tensor>::cont_cache.first.unlock();

		// CUDAcpl::Tensor
		cache::Global_Cache<CUDAcpl::Tensor>::CUDAcpl_cache.first.lock();
		cache::Global_Cache<CUDAcpl::Tensor>::CUDAcpl_cache.second.clear();
		cache::Global_Cache<CUDAcpl::Tensor>::CUDAcpl_cache.first.unlock();

		cache::Global_Cache<CUDAcpl::Tensor>::sum_cache.first.lock();
		cache::Global_Cache<CUDAcpl::Tensor>::sum_cache.second.clear();
		cache::Global_Cache<CUDAcpl::Tensor>::sum_cache.first.unlock();


		cache::Global_Cache<CUDAcpl::Tensor>::trace_cache.first.lock();
		cache::Global_Cache<CUDAcpl::Tensor>::trace_cache.second.clear();
		cache::Global_Cache<CUDAcpl::Tensor>::trace_cache.first.unlock();

		cache::Cont_Cache<CUDAcpl::Tensor, wcomplex>::cont_cache.first.lock();
		cache::Cont_Cache<CUDAcpl::Tensor, wcomplex>::cont_cache.second.clear();
		cache::Cont_Cache<CUDAcpl::Tensor, wcomplex>::cont_cache.first.unlock();

		cache::Cont_Cache<CUDAcpl::Tensor, CUDAcpl::Tensor>::cont_cache.first.lock();
		cache::Cont_Cache<CUDAcpl::Tensor, CUDAcpl::Tensor>::cont_cache.second.clear();
		cache::Cont_Cache<CUDAcpl::Tensor, CUDAcpl::Tensor>::cont_cache.first.unlock();
	}



	extern uint64_t vmem_limit;
	
#ifdef __WIN__
	extern HANDLE current_process;
#endif

	extern std::atomic<std::chrono::duration<double>> garbage_check_period;

	inline void get_current_process() {
#ifdef __WIN__
		current_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _getpid());
#endif
	}

	/// <summary>
	/// return the virtual memory, in Byte
	/// </summary>
	/// <returns></returns>
	inline uint64_t get_vmem() {
#ifdef __WIN__
		PROCESS_MEMORY_COUNTERS pmc;
		// get process hanlde by pid
		if (GetProcessMemoryInfo(current_process, &pmc, sizeof(pmc)))
		{
			return pmc.PagefileUsage;
		}
		return 0;
#endif

#ifdef __LINUX__
		char file_name[64] = { 0 };
		FILE* fd;
		char line_buff[512] = { 0 };
		std::sprintf(file_name, "/proc/%d/status", getpid());

		fd = fopen(file_name, "r");
		if (nullptr == fd)
			return 0;

		char name[64];
		int vmrss = 0;
		for (int i = 0; i < VMRSS_LINE - 1; i++)
			fgets(line_buff, sizeof(line_buff), fd);

		fgets(line_buff, sizeof(line_buff), fd);
		sscanf(line_buff, "%s %d", name, &vmrss);
		fclose(fd);

		// cnvert VmRSS from KB to MB
		return vmrss / 1024.0;
#endif
	}


	inline void cache_clear_check() {
		auto current_vmem = get_vmem();
		if (current_vmem > vmem_limit) {
#ifdef RESOURCE_OUTPUT
			std::cout << "cleaning garbage ...";
#endif
			clear_garbage();
#ifdef RESOURCE_OUTPUT
			std::cout << " done." << std::endl;
#endif


			// check whether further cleanning is needed
			auto new_vmem = get_vmem();
			if (new_vmem > vmem_limit) {
#ifdef RESOURCE_OUTPUT
				std::cout << "cleaning cache ...";
#endif
				clear_cache();
#ifdef RESOURCE_OUTPUT
				std::cout << " done." << std::endl;
#endif

				// if memory consumption still exceeds the limit, then throw the exception
				auto final_vem = get_vmem();
				if (final_vem > vmem_limit) {
#ifdef VMEM_SHUT_DOWN
#ifdef RESOURCE_OUTPUT
					std::cout << "memory consumption exceeds the limit after garbage collection, the calculation will be shut down" << std::endl;
#endif
					//throw std::exception("memory consumption exceeds the limit after garbage collection, the calculation will be shut down");
					exit(-1);
#endif
				}

			}
		}
	}


	inline void reset(int thread_num = DEFAULT_THREAD_NUM,
		bool device_cuda = false, bool double_type = true, double new_eps = DEFAULT_EPS,
		double gc_check_period = DEFAULT_MEM_CHECK_PERIOD, uint64_t vmem_limit_MB = DEFAULT_VMEM_LIMIT / 1024. / 1024.) {

		// reset the system first.
		tdd::TDD<wcomplex>::reset();
		tdd::TDD<CUDAcpl::Tensor>::reset();
		clear_garbage();
		clear_cache();

		vmem_limit = vmem_limit_MB * 1024. * 1024.;

		garbage_check_period.store(std::chrono::duration<double>{ gc_check_period });

		delete wnode::iter_para::p_thread_pool;
		wnode::iter_para::p_thread_pool = new ThreadPool(thread_num);

		CUDAcpl::reset(device_cuda, double_type);
		weight::EPS = new_eps;
	}

}