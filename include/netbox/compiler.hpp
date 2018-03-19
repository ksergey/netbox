/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_compiler_140318105608
#define KSERGEY_compiler_140318105608

#ifndef NETBOX_LIKELY
#   define NETBOX_LIKELY(x) __builtin_expect(static_cast< bool >(x), true)
#endif

#ifndef NETBOX_UNLIKELY
#   define NETBOX_UNLIKELY(x) __builtin_expect(static_cast< bool >(x), false)
#endif

#endif /* KSERGEY_compiler_140318105608 */
