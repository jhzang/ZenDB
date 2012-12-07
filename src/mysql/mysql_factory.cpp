#include <zdb/mysql/mysql_factory.h>
#include <zdb/mysql/mysql_connection.h>

namespace zdb
{
	MySql_Factory::MySql_Factory()
		: DBFactory(DB_MYSQL5)
	{
	}

	MySql_Factory::~MySql_Factory()
	{
	}

	Connection* MySql_Factory::CreateConnection()
	{
		return new MySql_Connection();
	}

	MySql_Factory* MySql_Factory::Instance()
	{
		static MySql_Factory s_instance;
		return &s_instance;
	}
}

