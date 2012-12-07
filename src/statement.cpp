#include <zdb/statement.h>

namespace zdb
{
	Statement::Statement(Connection *conn)
		: m_conn(conn)
	{
	}

	Statement::Statement(Connection *conn, const char *stmt_str, size_t len) 
		: m_conn(conn), m_stmt_str(stmt_str, len)
	{
	}

	Statement::~Statement()
	{
	}
} // namespace zdb

