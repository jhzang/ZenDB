#ifndef ZDB__MYSQL__MYSQL_FACTORY_H
#define ZDB__MYSQL__MYSQL_FACTORY_H

#include <zdb/dbfactory.h>

namespace zdb
{
	class MySql_Factory : public DBFactory
	{
	public:
		MySql_Factory();
		virtual ~MySql_Factory();

		virtual Connection* CreateConnection();

		static MySql_Factory* Instance();
	};
} // namespace zdb
#endif

