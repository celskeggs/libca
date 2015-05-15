#define MSPACES 1
#define ONLY_MSPACES 1
#define FOOTERS 1
#define ABORT panic_static("failed malloc")
//#define DEBUG 1
//#define ABORT_ON_ASSERT_FAILURE 1
#define MALLOC_FAILURE_ACTION ; // do nothing
#define NO_MALLOC_STATS 1
#define LACKS_UNISTD_H 1
#define LACKS_FCNTL_H 1
#define LACKS_SYS_PARAM_H 1
#define LACKS_SYS_MMAN_H 1
#define LACKS_STRINGS_H 1
//#define LACKS_STRING_H 1 - we do have this, but slightly different...
#define LACKS_SYS_TYPES_H 1
#define LACKS_ERRNO_H 1
#define LACKS_STDLIB_H 1
#define LACKS_SCHED_H 1
#define LACKS_TIME_H 1
#define NO_MALLINFO 1

#define LACKS_STDDEF_H 1
