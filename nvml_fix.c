#include <dlfcn.h>

#include "nvml.h"


#define FUNC(f) static typeof(f) * real_##f;
#define FUNC_v2(f) static typeof(f) * real_##f##_v2;
FUNC(nvmlInit)
FUNC_v2(nvmlInit)
FUNC(nvmlDeviceGetHandleByIndex)
FUNC_v2(nvmlDeviceGetHandleByIndex)
FUNC(nvmlDeviceGetHandleBySerial)
FUNC(nvmlDeviceGetHandleByUUID)
FUNC(nvmlDeviceGetHandleByPciBusId)
FUNC_v2(nvmlDeviceGetHandleByPciBusId)

#define LOAD(f) if (!(real_##f = dlsym(nvml, #f))) return NVML_ERROR_UNKNOWN
#define INIT(name) nvmlReturn_t name() \
{ \
	void *nvml = dlopen("libnvidia-ml.so." NVML_VERSION, RTLD_NOW); \
\
	LOAD(nvmlInit); \
	LOAD(nvmlInit_v2); \
	LOAD(nvmlDeviceGetHandleByIndex); \
	LOAD(nvmlDeviceGetHandleByIndex_v2); \
	LOAD(nvmlDeviceGetHandleBySerial); \
	LOAD(nvmlDeviceGetHandleByUUID); \
	LOAD(nvmlDeviceGetHandleByPciBusId); \
	LOAD(nvmlDeviceGetHandleByPciBusId_v2); \
\
	return real_##name(); \
}

INIT(nvmlInit)
INIT(nvmlInit_v2)

void fix_unsupported_bug(nvmlDevice_t device)
{
	unsigned int *fix = (unsigned int *)device;
#if defined(NVML_PATCH_319) || defined(NVML_PATCH_325)
# ifdef __i386__
	fix[201] = 1;
	fix[202] = 1;
# else
	fix[202] = 1;
	fix[203] = 1;
# endif
#elif defined(NVML_PATCH_331)
# ifdef __i386__
#  error "No i386 support for this version yet!"
# else
	fix[187] = 2;
	fix[188] = 1;
# endif
#else
# error "No valid NVML_PATCH_* option specified!"
#endif
}

#define GET_HANDLE_BY(name, type) \
nvmlReturn_t nvmlDeviceGetHandleBy##name(type x, nvmlDevice_t *device) \
{ \
	nvmlReturn_t ret; \
\
	if (!real_nvmlDeviceGetHandleBy##name) \
		return NVML_ERROR_UNINITIALIZED; \
\
	ret = real_nvmlDeviceGetHandleBy##name(x, device); \
	if (ret != NVML_SUCCESS) \
		return ret; \
\
	fix_unsupported_bug(*device); \
	return NVML_SUCCESS; \
}

GET_HANDLE_BY(Index, unsigned int)
GET_HANDLE_BY(Index_v2, unsigned int)
GET_HANDLE_BY(Serial, const char *)
GET_HANDLE_BY(UUID, const char *)
GET_HANDLE_BY(PciBusId, const char *)
GET_HANDLE_BY(PciBusId_v2, const char *)
