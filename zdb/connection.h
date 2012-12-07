// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZDB__CONNECTION_H
#define ZDB__CONNECTION_H

#include <string>

namespace zdb
{
	class Statement;
	class PreparedStatement;

	class Connection
	{
	public:
		Connection();
		virtual ~Connection();

		void SetConnectionString(const char *server, const char *user, const char *password, const char *db, unsigned int port);
		inline bool Open(const char *server, const char *user, const char *password, const char *db, unsigned int port);
		inline bool Open();
		inline void Close();
		inline bool SetAutoReconnect(bool mode);
		inline bool SetAutoCommit(bool mode);

		inline bool IsOpened();
		inline bool IsAutoCommit() const;
		inline bool IsAutoReconnect() const;

		// Transaction related
		virtual bool BeginTransaction() = 0;
		virtual bool Commit() = 0;
		virtual bool Rollback() = 0;
		// Statement
		virtual Statement* CreateStatement() = 0;
		virtual PreparedStatement* CreatePreparedStatement() = 0;

	protected:
		virtual bool DoOpen() = 0;
		virtual void DoClose() = 0;
		virtual bool CheckActive() = 0;
		virtual bool DoSetAutoReconnect(bool mode) = 0;
		virtual bool DoSetAutoCommit(bool mode) = 0;

	protected:
		std::string m_server;
		std::string m_user;
		std::string m_password;
		std::string m_db;
		unsigned int m_port;
		bool m_is_autocommit;
		bool m_is_autoreconnect;

		bool m_is_opened;
	}; // class Connection


	bool Connection::Open(const char *server, const char *user, const char *password, const char *db, unsigned int port)
	{
		if (m_is_opened) {
			DoClose();
		}
		SetConnectionString(server, user, password, db, port);
		return Open();
	}

	bool Connection::IsAutoCommit() const
	{
		return m_is_autocommit;
	}

	bool Connection::IsAutoReconnect() const
	{
		return m_is_autoreconnect;
	}

	bool Connection::Open()
	{
		m_is_opened = DoOpen();
		if (m_is_opened) {
			SetAutoReconnect(m_is_autoreconnect);
			SetAutoCommit(m_is_autocommit);
		}
		return m_is_opened;
	}

	void Connection::Close()
	{
		if (m_is_opened) {
			DoClose();
			m_is_opened = false;
		}
	}

	bool Connection::IsOpened()
	{
		if (!m_is_opened) {
			return false;
		}
		return CheckActive();
	}

	bool Connection::SetAutoReconnect(bool mode)
	{
		if (m_is_opened) {
			return false;
		}
		if (!DoSetAutoReconnect(mode)) {
			return false;
		}
		m_is_autoreconnect = mode;
		return true;
	}

	bool Connection::SetAutoCommit(bool mode)
	{
		if (m_is_opened) {
			if (DoSetAutoCommit(mode)) {
				m_is_autocommit = mode;
				return true;
			} else {
				return false;
			}
		}
		m_is_autocommit = mode;
		return true;
	}

} // namespace zdb

#endif

