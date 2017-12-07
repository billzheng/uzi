//wrap some gcc builtins so code compiles on non gnuc compilers
#if defined(__GNUC__)
#define RESTRICT __restrict
#define COLD __attribute((cold))
#define HOT __attribute((hot))
#define LIKELY(expr)    __builtin_expect(!!(expr), 1)
#define UNLIKELY(expr)  __builtin_expect(!!(expr), 0)
#define PACKED __attribute__((packed))
#define ALWAYS_INLINE inline __attribute__((__always_inline__))
#define NEVER_INLINE __attribute__((__noinline__))
#define PREFETCH_READ_NT(addr) __builtin_prefetch(addr, 0, 0)
#define PREFETCH_READ(addr) __builtin_prefetch(addr, 0, 3)
#define PREFETCH_WRITE(addr) __builtin_prefetch(addr, 1, 3)
#else
#define RESTRICT
#define COLD
#define HOT
#define LIKELY
#define UNLIKELY
#define PACKED
#define ALWAYS_INLINE inline
#define NEVER_INLINE
#define PREFETCH_READ
#define PREFETCH_WRITE
#define PREFETCH_READ_NT
#endif


#define UNUSED(n)   { (void)n; }

