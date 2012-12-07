#include <zdb/dbfactory.h>
#include <cstdlib>
#include <zdb/mysql/mysql_factory.h>

namespace zdb
{
	DBFactory::DBFactory(DBType type)
		: m_dbtype(type)
	{
	}

	DBFactory::~DBFactory()
	{
	}

	DBFactory* DBFactory::DefaultFactory(DBType type)
	{
		DBFactory *factory = NULL;
		switch (type) {
			case DB_MYSQL5:
				factory = MySql_Factory::Instance();
				break;
			case DB_SQLITE3:
				break;
			case DB_MSSQL:
				break;
			case DB_ORACLE:
				break;
			default:
				break;
		}
		return factory;
	}

} // namespace zdb

