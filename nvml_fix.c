#include <dlfcn.h>

#if defined(NVML_PATCH_319) || defined(NVML_PATCH_325) || defined(NVML_PATCH_331)
#define NVML_V3
#include "nvml_v3.h"
#elif defined(NVML_PATCH_390) || defined(NVML_PATCH_396) || defined(NVML_PATCH_410) || defined(NVML_PATCH_415) || defined(NVML_PATCH_418) || defined(NVML_PATCH_430) || defined(NVML_PATCH_440) || defined(NVML_PATCH_450)
#define NVML_V9
#include "nvml_v9.h"
#else
#error "No valid NVML_PATCH_* option specified! Currently supported versions are: 450.x, 440.x, 430.x, 418.x, 415.x, 410.x, 396.x, 390.x, 331.x, 325.x (x86_64 and i386), and 319.x (x86_64 and i386)."
#endif

#define FUNC(f) static typeof(f) * real_##f;
#define FUNC_v2(f) static typeof(f) * real_##f##_v2;

#if defined(NVML_V3)
FUNC(nvmlInit)
FUNC(nvmlDeviceGetHandleByIndex)
FUNC(nvmlDeviceGetHandleByPciBusId)
#elif defined(NVML_V9)
FUNC(nvmlInitWithFlags);
#endif
FUNC_v2(nvmlInit)
FUNC_v2(nvmlDeviceGetHandleByIndex)
FUNC(nvmlDeviceGetHandleBySerial)
FUNC(nvmlDeviceGetHandleByUUID)
FUNC_v2(nvmlDeviceGetHandleByPciBusId)

#define LOAD(f) if (!(real_##f = dlsym(nvml, #f))) return NVML_ERROR_UNKNOWN
#if defined(NVML_V3)
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
#elif defined(NVML_V9)
#define INIT(name) nvmlReturn_t name() \
{ \
void *nvml = dlopen("libnvidia-ml.so." NVML_VERSION, RTLD_NOW); \
\
	LOAD(nvmlInit_v2); \
	LOAD(nvmlInitWithFlags); \
	LOAD(nvmlDeviceGetHandleByIndex_v2); \
	LOAD(nvmlDeviceGetHandleBySerial); \
	LOAD(nvmlDeviceGetHandleByUUID); \
	LOAD(nvmlDeviceGetHandleByPciBusId_v2); \
\
	return real_##name(); \
}
#endif

#if defined(NVML_V3)
INIT(nvmlInit)
#endif
INIT(nvmlInit_v2)

#if defined(NVML_V9)
nvmlReturn_t nvmlInitWithFlags(unsigned int flags) {
	void *nvml = dlopen("libnvidia-ml.so." NVML_VERSION, RTLD_NOW);

	LOAD(nvmlInit_v2);
	LOAD(nvmlInitWithFlags);
	LOAD(nvmlDeviceGetHandleByIndex_v2);
	LOAD(nvmlDeviceGetHandleBySerial);
	LOAD(nvmlDeviceGetHandleByUUID);
	LOAD(nvmlDeviceGetHandleByPciBusId_v2);

	return real_nvmlInitWithFlags(flags);
}
#endif

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
#elif defined(NVML_PATCH_390) || defined(NVML_PATCH_396)
# ifdef __i386__
#  error "No i386 support for this version yet!"
# else
	fix[352] = 1;
	fix[353] = 1;
# endif
#elif defined(NVML_PATCH_410) || defined(NVML_PATCH_415) || defined(NVML_PATCH_418) || defined(NVML_PATCH_430) || defined(NVML_PATCH_440)
# ifdef __i386__
#  error "No i386 support for this version yet!"
# else
	fix[362] = 1;
	fix[363] = 1;
# endif
#elif defined(NVML_PATCH_450)
# ifdef __i386__
#  error "No i386 support for this version yet!"
# else
#  if NVML_PATCH_MINOR >= 66
	fix[360] = 1;
	fix[361] = 1;
#  else
	fix[364] = 1;
	fix[365] = 1;
#  endif
# endif
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

#if defined(NVML_V3)
GET_HANDLE_BY(Index, unsigned int)
GET_HANDLE_BY(PciBusId, const char *)
#endif
GET_HANDLE_BY(Index_v2, unsigned int)
GET_HANDLE_BY(Serial, const char *)
GET_HANDLE_BY(UUID, const char *)
GET_HANDLE_BY(PciBusId_v2, const char *)
