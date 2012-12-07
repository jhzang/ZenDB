// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
// Prepared execution is an efficient way to execute a statement more than once. 
// The statement is first parsed to prepare it for execution. Then it is executed 
// one or more times at a later time, using the statement handle returned by the 
// initialization function.
//
// Prepared execution is faster than direct execution for statements executed more 
// than once, primarily because the query is parsed only once. In the case of direct 
// execution, the query is parsed every time it is executed. Prepared execution also 
// can provide a reduction of network traffic because for each execution of the prepared 
// statement, it is necessary only to send the data for the parameters.
//
// Prepared statements might not provide a performance increase in some situations. 
// For best results, test your application both with prepared and nonprepared statements 
// and choose whichever yields best performance.
//
// Another advantage of prepared statements is that it uses a binary protocol that makes 
// data transfer between client and server more efficient.
//
#ifndef ZDB__PREPARED_STATEMENT_H
#define ZDB__PREPARED_STATEMENT_H

#include <string>
#include <zdb/statement.h>

namespace zdb
{
	// e.g. "SELECT * FROM USER WHERE id = ?"
	class PreparedStatement : public Statement
	{
	public:
		PreparedStatement(Connection *conn = NULL) : Statement(conn) {}
		PreparedStatement(Connection *conn, const char *stmt_str, size_t len) : Statement(conn, stmt_str, len) {}
		virtual ~PreparedStatement()
		{
			for (std::vector<Parameter*>::iterator it = m_parameters.begin(); it != m_parameters.end(); ++it) {
				if (NULL != *it) {
					delete *it;
					*it = NULL;
				}
			}
		}

		virtual bool BindParameter(int index, Parameter *param) = 0;

		// Return affected row count
		virtual int ExecuteNonQuery() = 0;
		// Return a data set
		virtual DataReader* ExecuteQuery() = 0;
		// Return the first column of the first row in result set
		virtual DataField* ExecuteScalar() = 0;

	private:
		std::vector<Parameter*> m_parameters;
	}; // class Statement
} // namespace zdb
#endif

