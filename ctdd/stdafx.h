#pragma once

//#define Py_LIMITED_API 3

#ifdef __TDDPY__
#include <Python.h>
#include <torch/python.h>
#endif

#include <cmath>
#include <stdio.h>
#include <iostream>
#include <complex>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/container_hash/hash_fwd.hpp>
#include <string>
#include <algorithm>
#include <type_traits>
#include <vector>
#include <assert.h>
#include <chrono>

#include <torch/torch.h>
#include <torch/script.h>

// resource management

#ifdef __WIN__
#define NOMINMAX
#include <windows.h>  
#include <psapi.h>  
//#include <tlhelp32.h>
#include <direct.h>
#include <process.h>
#endif

#ifdef __LINUX__
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <unistd.h>
#endif




// multi-thread
#include <shared_mutex>
#include <atomic>
#include "ThreadPool.h"

#include "simpletools.h"
#include "config.h"
#include "CUDAcpl.h"
