file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/net/)
file(GLOB HEADERS_POLICY "policy/*.h")
install(FILES ${HEADERS_POLICY} DESTINATION include/)
