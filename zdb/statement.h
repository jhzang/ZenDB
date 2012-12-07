// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZDB__STATEMENT_H
#define ZDB__STATEMENT_H

#include <cassert>
#include <vector>
#include <string>

namespace zdb
{
	class Connection;
	class ResultSet;
	class DataField;

	// Plain statement
	class Statement
	{
	public:
		Statement(Connection *conn = NULL);
		Statement(Connection *conn, const char *stmt_str, size_t len);
		virtual ~Statement();

		inline Connection* GetConnection();
		inline const std::string& GetStatementString() const;

		inline void SetConnection(Connection *conn);
		inline void SetStatementString(const char *stmt_str, size_t len);

		// Return affected row count
		virtual int ExecuteNonQuery(int *errcode) = 0;
		// Return a result set
		virtual ResultSet* ExecuteQuery(int *errcode) = 0;
		// Output multiple result sets if any. e.g. executing multiple queries in one statement
		virtual int ExecuteQuery(std::vector<ResultSet*> &results) = 0;
		// Return the first column of the first row in result set
		virtual DataField* ExecuteScalar() = 0;

	protected:
		Connection *m_conn;
		std::string m_stmt_str;
	}; // class Statement


	Connection* Statement::GetConnection()
	{
		return m_conn;
	}

	const std::string& Statement::GetStatementString() const
	{
		return m_stmt_str;
	}

	void Statement::SetConnection(Connection *conn)
	{
		m_conn = conn;
	}

	void Statement::SetStatementString(const char *stmt_str, size_t len)
	{
		assert(NULL != stmt_str && len > 0);
		m_stmt_str = stmt_str;
	}

} // namespace zdb
#endif

