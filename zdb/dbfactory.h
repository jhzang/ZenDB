#ifndef ZDB__DBFACTORY_H
#define ZDB__DBFACTORY_H

namespace zdb
{
	class Connection;

	enum DBType
	{
		DB_MYSQL5,
		DB_SQLITE3,
		DB_MSSQL,
		DB_ORACLE,
	};

	class DBFactory
	{
	public:
		DBFactory(DBType type);
		virtual ~DBFactory();

		virtual Connection* CreateConnection() = 0;

	public:
		static DBFactory* DefaultFactory(DBType type);

	private:
		DBType m_dbtype;
	};

} // namespace zdb
#endif

