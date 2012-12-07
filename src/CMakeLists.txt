option(ZDB_MYSQL "Support MySql" ON)
option(ZDB_SQLITE3 "Support Sqlite3" OFF)
option(ZDB_MSSQL "Support Microsoft SQL Server" OFF)
option(ZDB_ORACLE "Support Oracle" OFF)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

set(SRCS dbfactory.cpp connection.cpp statement.cpp resultset.cpp datafield.cpp datarow.cpp)

if (ZDB_MYSQL)
	message(STATUS "Support MySql")
	set(SRCS ${SRCS} mysql/mysql_factory.cpp mysql/mysql_system.cpp mysql/mysql_statement.cpp)
endif(ZDB_MYSQL)

add_library(zdb_shared SHARED ${SRCS})
set_target_properties(zdb_shared PROPERTIES OUTPUT_NAME zdb VERSION 1.0 SOVERSION 1)

add_library(zdb_static STATIC ${SRCS})
set_target_properties(zdb_static PROPERTIES OUTPUT_NAME zdb)

install(TARGETS zdb_shared zdb_static
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/zdb DESTINATION include)

