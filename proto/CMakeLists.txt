file(GLOB SRCS *.cc)
add_library(protocol  ${SRCS})
file (GLOB HEADERS *.h)
INSTALL(FILES ${HEADERS} DESTINATION include/)
