add_library(sqlite3 STATIC sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(SQLite::SQLite3 ALIAS sqlite3)
