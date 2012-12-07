// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZENDBLIB__MYSQL__MYSQL_STATEMENT_H
#define ZENDBLIB__MYSQL__MYSQL_STATEMENT_H

#include <mysql/mysql.h>
#include <zdb/statement.h>

namespace zdb
{
	class MySql_Statement : public Statement
	{
	public:
		MySql_Statement(Connection *conn = NULL) : Statement(conn) {}
		MySql_Statement(Connection *conn, const char *stmt_str, size_t len) : Statement(conn, stmt_str, len) {}
		virtual ~MySql_Statement() {}

		// Return affected row count
		virtual int ExecuteNonQuery(int *errcode);
		// Return a result set
		virtual ResultSet* ExecuteQuery(int *errcode);
		// Output multiple result sets if any. e.g. executing multiple queries in one statement
		virtual int ExecuteQuery(std::vector<ResultSet*> &results);
		// Return the first column of the first row in result set
		virtual DataField* ExecuteScalar();

	private:
		int Execute();
		ResultSet* FetchResult(MYSQL_RES *raw_result);
	}; // class MySql_Statement

} // namespace zdb
#endif

