set(SRCS
    coroutine.c)
add_library(coroutine  ${SRCS})
set(HEADERS
    coroutine.h)
INSTALL(FILES ${HEADERS} DESTINATION include/coroutine)
