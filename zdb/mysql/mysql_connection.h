// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZENDBLIB__MYSQL__MYSQL_CONNECTION_H
#define ZENDBLIB__MYSQL__MYSQL_CONNECTION_H

#include <mysql/mysql.h>
#include <zdb/connection.h>
#include <zdb/mysql/mysql_statement.h>

namespace zdb
{
	class Statement;
	class PreparedStatement;

	class MySql_Connection : public Connection
	{
		friend class MySql_Statement;

	public:
		MySql_Connection() { mysql_init(&m_mysql); }
		virtual ~MySql_Connection() { Close(); }

		virtual bool BeginTransaction()
		{
			if (m_is_autocommit) {
				return false;
			}
			// Nothing to do
			return true;
		}

		virtual bool Commit()
		{
			if (m_is_autocommit) {
				return false;
			}
			return mysql_commit(&m_mysql) == 0;
		}

		virtual bool Rollback()
		{
			if (m_is_autocommit) {
				return false;
			}
			return mysql_rollback(&m_mysql) == 0;
		}

		// Statement
		virtual Statement* CreateStatement()
		{
			if (!m_is_opened) {
				return NULL;
			}
			return new MySql_Statement(this);
		}
		virtual PreparedStatement* CreatePreparedStatement()
		{
			//if (!m_is_opened) {
			//	return NULL;
			//}
			//return new MySql_PreparedStatement(this);
			// TODO
			return NULL;
		}

	protected:
		virtual bool DoOpen()
		{
			if (NULL == mysql_real_connect(
				&m_mysql, m_server.c_str(), m_user.c_str(), m_password.c_str(), m_db.c_str(), m_port, NULL, 0)) {
				return false;
			}
			// NOTE: mysql_real_connect() incorrectly reset the MYSQL_OPT_RECONNECT option to its default value 
			// before MySQL 5.1.6. Therefore, prior to that version, if you want reconnect to be enabled for 
			// each connection, you must call mysql_options() with the MYSQL_OPT_RECONNECT option after each 
			// call to mysql_real_connect(). This is not necessary as of 5.1.6: Call mysql_options() only 
			// before mysql_real_connect() as usual.
			if (mysql_get_client_version() < 50106) {
				SetAutoReconnect(m_is_autoreconnect);
			}
			return true;
		}

		virtual void DoClose()
		{
			mysql_close(&m_mysql);
		}

		virtual bool CheckActive()
		{
			// NOTE: If the connection has gone down and auto-reconnect is enabled, 
			//       an attempt to reconnect will be made by mysql_ping()
			return mysql_ping(&m_mysql) == 0;
		}

		virtual bool DoSetAutoReconnect(bool mode)
		{
			// Call mysql_options() after mysql_init() and before mysql_connect() or mysql_real_connect()
			my_bool tmp_mode = (mode ? 1 : 0);
			return (mysql_options(&m_mysql, MYSQL_OPT_RECONNECT, &tmp_mode) == 0);
		}

		virtual bool DoSetAutoCommit(bool mode)
		{
			my_bool tmp_mode = (mode ? 1 : 0);
			return (mysql_autocommit(&m_mysql, tmp_mode) == 0);
		}

	private:
		MYSQL m_mysql;
	}; // class MySql_Connection

} // namespace zdb
#endif

