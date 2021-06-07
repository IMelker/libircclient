# ---------------------------------------------------------------------------------------
# IDE support for headers
# ---------------------------------------------------------------------------------------
set(LIBIRCCLIENT_HEADERS_DIR "${CMAKE_CURRENT_LIST_DIR}/../include/")

file(GLOB LIBIRCCLIENT_TOP_HEADERS "${LIBIRCCLIENT_HEADERS_DIR}/libircclient/*.h")
include_directories(${LIBIRCCLIENT_HEADERS_DIR}/libircclient/)
file(GLOB LIBIRCCLIENT_DETAILS_HEADERS "${LIBIRCCLIENT_HEADERS_DIR}/../src/*.h")
include_directories(${LIBIRCCLIENT_HEADERS_DIR}/../src/)
set(LIBIRCCLIENT_ALL_HEADERS ${LIBIRCCLIENT_TOP_HEADERS} ${LIBIRCCLIENT_DETAILS_HEADERS})

source_group("Header Files\\libircclient" FILES ${LIBIRCCLIENT_TOP_HEADERS})
source_group("Header Files\\libircclient\\details" FILES ${LIBIRCCLIENT_DETAILS_HEADERS})