// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#include <zdb/connection.h>

namespace zdb
{
	Connection::Connection()
		: m_server(), 
		  m_user(), 
		  m_password(), 
		  m_port(0),
		  m_is_autocommit(true),
		  m_is_autoreconnect(false),
		  m_is_opened(false)
	{
	}

	Connection::~Connection()
	{
	}

	void Connection::SetConnectionString(
		const char *server, const char *user, const char *password, const char *db, unsigned int port)
	{
		if (NULL == server) {
			m_server.clear();
		} else {
			m_server = server;
		}
		if (NULL == user) {
			m_user.clear();
		} else {
			m_user = user;
		}
		if (NULL == password) {
			m_password.clear();
		} else {
			m_password = password;
		}
		if (NULL == db) {
			m_db.clear();
		} else {
			m_db = db;
		}
		m_port = port;
	}
} // namespace zdb

