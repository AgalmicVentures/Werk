
#pragma once

#if __has_attribute(always_inline)
	#define ALWAYS_INLINE __attribute__((always_inline))
#else
	#define ALWAYS_INLINE
#endif

#if __has_attribute(warn_unused_result)
	#define CHECKED __attribute__((warn_unused_result))
#else
	#define CHECKED
#endif

#if __has_attribute(cold)
	#define COLD __attribute__((cold))
#else
	#define COLD
#endif

#if __has_attribute(deprecated)
	#define DEPRECATED __attribute__((deprecated))
#else
	#define DEPRECATED
#endif

#if __has_attribute(hot)
	#define HOT __attribute__((hot))
#else
	#define HOT
#endif

#if __has_attribute(pure)
	#define PURE __attribute__((pure))
#else
	#define PURE
#endif

#if __has_attribute(unused)
	#define UNUSED __attribute__((unused))
#else
	#define UNUSED
#endif
