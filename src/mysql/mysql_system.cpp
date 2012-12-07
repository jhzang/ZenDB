// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#include <cstdlib>
#include <cstdio>
#include <mysql/mysql.h>

namespace db
{
	class MySqlSystem
	{
	public:
		MySqlSystem()
		{
			if (mysql_library_init(0, NULL, NULL) != 0) {
				fprintf(stderr, "MySql library initialization failed.\n");
			}
		}
		~MySqlSystem()
		{
			mysql_library_end();
		}
	}; // class MySqlSystem

	static MySqlSystem g_mysql_system;

} // namespace zdb

