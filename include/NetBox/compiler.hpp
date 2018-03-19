/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_compiler_140318105608
#define KSERGEY_compiler_140318105608

#ifndef OVERKILL_LIKELY
#   define OVERKILL_LIKELY(x) __builtin_expect(static_cast< bool >(x), true)
#endif

#ifndef OVERKILL_UNLIKELY
#   define OVERKILL_UNLIKELY(x) __builtin_expect(static_cast< bool >(x), false)
#endif

#endif /* KSERGEY_compiler_140318105608 */
