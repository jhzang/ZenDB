#include <gtest/gtest.h>
#include <cmath>
#include <zdb/dbfactory.h>
#include <zdb/connection.h>
#include <zdb/statement.h>
#include <zdb/resultset.h>
using namespace zdb;

TEST(MySqlTest, SetupTestDb) {
	std::stringstream ss;
	ss << "mysql -uroot -p123456 -e\"";
	//ss << "SHOW DATABASES;";
	// Create database
	ss << "DROP DATABASE IF EXISTS zdb_test;";
	ss << "CREATE DATABASE zdb_test;";
	ss << "USE zdb_test;";
	// Create table
	ss << "DROP TABLE IF EXISTS TEST;";
	ss << "CREATE TABLE TEST (f_bigint BIGINT NOT NULL PRIMARY KEY,";
	ss << "                   f_binary BINARY(16) NULL,";
	ss << "                   f_bit BIT(20) NULL,";     // max: 64
	ss << "                   f_blob BLOB NULL,";       // max: 65535
	ss << "                   f_boolean BOOLEAN NULL,"; // synonym for TINYINT(1)
	ss << "                   f_char CHAR(255) NULL,";  // max: 255
	ss << "                   f_date DATE NULL,";       // '1000-01-01' to '9999-12-31'
	ss << "                   f_datetime DATETIME NULL,"; // '1000-01-01 00:00:00' to '9999-12-31 23:59:59'
	ss << "                   f_decimal DECIMAL(65,30) NULL,";
	ss << "                   f_enum ENUM('v1','v2','v3') NULL,";
	ss << "                   f_float FLOAT(3,2) NULL,"; // use DOUBLE instead
	ss << "                   f_int INT NULL,"; // 32bit integer
	ss << "                   f_set SET('v1','v2','v3','v4','v5','v6') NULL,"; // max: 64
	ss << "                   f_text TEXT,";
	ss << "                   f_time TIME NULL,"; // '-838:59:59' to '838:59:59'
	ss << "                   f_timestamp TIMESTAMP NULL,"; // '1970-01-01 00:00:01' UTC to '2038-01-19 03:14:07' UTC
	ss << "                   f_varbinary VARBINARY(255) NULL,";
	ss << "                   f_varchar VARCHAR(255) NULL,";
	ss << "                   f_year YEAR(4) NULL";
	ss << "                  );";
	// Populate test data
	//ss << "INSERT INTO TEST(f_bigint) VALUES(1);";
	//ss << "INSERT INTO TEST VALUES(null, 'ABCDEFG', 7, 'ABCDEFG', 1, 'ABCDEFG', CURRENT_DATE, NOW(), 123.456, 'v1', 1.73, 123, 'v2' | 'v5', 'ABCDEFG', CURRENT_TIME, CURRENT_TIMESTAMP, 'ABCDEFG', 'ABCDEFG', 2012);";
	ss << "\"";
	system(ss.str().c_str());
}

TEST(MySqlTest, Connection) {
	Connection *conn = DBFactory::DefaultFactory(DB_MYSQL5)->CreateConnection();
	EXPECT_TRUE(NULL != conn);
	conn->SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
	conn->SetAutoCommit(true);
	conn->SetAutoReconnect(false);
	EXPECT_TRUE(conn->Open());
	EXPECT_TRUE(conn->IsOpened());
	conn->Close();
	EXPECT_FALSE(conn->IsOpened());
	delete conn;
	conn = NULL;
}

TEST(MySqlTest, AutoReconnect) {
	Connection *conn = DBFactory::DefaultFactory(DB_MYSQL5)->CreateConnection();
	EXPECT_TRUE(NULL != conn);
	conn->SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
	conn->SetAutoCommit(true);
	conn->SetAutoReconnect(true);
	EXPECT_TRUE(conn->Open());
	EXPECT_TRUE(conn->IsOpened());
	conn->Close();
	delete conn;
	conn = NULL;
}

TEST(MySqlTest, Statement_Insert) {
	Connection *conn = DBFactory::DefaultFactory(DB_MYSQL5)->CreateConnection();
	EXPECT_TRUE(NULL != conn);
	conn->SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
	conn->SetAutoCommit(true);
	conn->SetAutoReconnect(false);
	EXPECT_TRUE(conn->Open());
	if (!conn->IsOpened()) {
		delete conn;
		conn = NULL;
		return;
	}

	Statement *stmt = conn->CreateStatement();
	EXPECT_TRUE(NULL != stmt);
	std::string stmt_str;
	// Insert a row with null fields
	stmt_str = "INSERT INTO TEST(f_bigint) VALUES(1);";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	EXPECT_TRUE(stmt->ExecuteNonQuery(NULL) == 1);
	// Insert a row with normal fields
	stmt_str = "INSERT INTO TEST VALUES(2, 'ABCDEFG', b'10011110000', 'ABCDEFG', 1, 'ABCDEFG', CURRENT_DATE, NOW(), 123.456, 'v1', 1.73, 123, 'v2,v5', 'ABCDEFG', CURRENT_TIME, CURRENT_TIMESTAMP, 'ABCDEFG', 'ABCDEFG', 2012);";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	EXPECT_TRUE(stmt->ExecuteNonQuery(NULL) == 1);
	// Insert duplicate row
	stmt_str = "INSERT INTO TEST(f_bigint) VALUES(1);";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	EXPECT_TRUE(stmt->ExecuteNonQuery(NULL) == 0);
	// Release statement object
	delete stmt;
	stmt = NULL;
	delete conn;
	conn = NULL;
}

TEST(MySqlTest, Statement_Select_NullFields) {
	Connection *conn = DBFactory::DefaultFactory(DB_MYSQL5)->CreateConnection();
	EXPECT_TRUE(NULL != conn);
	conn->SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
	conn->SetAutoCommit(true);
	conn->SetAutoReconnect(false);
	EXPECT_TRUE(conn->Open());
	if (!conn->IsOpened()) {
		delete conn;
		conn = NULL;
		return;
	}

	Statement *stmt = conn->CreateStatement();
	EXPECT_TRUE(NULL != stmt);
	std::string stmt_str;

	// Select a row with null fields
	stmt_str = "SELECT * from TEST WHERE f_bigint = 1;";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	ResultSet *result_set = stmt->ExecuteQuery(NULL);
	EXPECT_TRUE(NULL != result_set);
	EXPECT_TRUE(result_set->GetRowCount() == 1);
	const DataRow *row = result_set->GetRow(0);
	EXPECT_TRUE(NULL != row);
	const DataField *field = row->GetField("f_bigint");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetLongLong() == 1);
	field = row->GetField("f_binary");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_bit");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_blob");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_boolean");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_char");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_date");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_datetime");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_decimal");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_enum");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_float");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_int");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_set");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_text");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_time");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_timestamp");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_varbinary");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_varchar");
	EXPECT_TRUE(NULL != field && field->IsNull());
	field = row->GetField("f_year");
	EXPECT_TRUE(NULL != field && field->IsNull());

	delete result_set;
	result_set = NULL;
	delete stmt;
	stmt = NULL;
}

TEST(MySqlTest, Statement_Select_NormalFields) {
	Connection *conn = DBFactory::DefaultFactory(DB_MYSQL5)->CreateConnection();
	EXPECT_TRUE(NULL != conn);
	conn->SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
	conn->SetAutoCommit(true);
	conn->SetAutoReconnect(false);
	EXPECT_TRUE(conn->Open());
	if (!conn->IsOpened()) {
		delete conn;
		conn = NULL;
		return;
	}

	Statement *stmt = conn->CreateStatement();
	EXPECT_TRUE(NULL != stmt);
	std::string stmt_str;

	// Select a row with normal fields
	stmt_str = "SELECT * from TEST WHERE f_bigint = 2;";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	ResultSet *result_set = stmt->ExecuteQuery(NULL);
	EXPECT_TRUE(NULL != result_set);
	EXPECT_TRUE(result_set->GetRowCount() == 1);
	const DataRow *row = result_set->GetRow(0);
	EXPECT_TRUE(NULL != row);
	const DataField *field = row->GetField("f_bigint");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetLongLong() == 2);
	field = row->GetField("f_binary");
	const void *buf = NULL;
	size_t size = 0;
	EXPECT_TRUE(NULL != field && !field->IsNull());
	field->GetBlob(buf, size);
	EXPECT_TRUE(16 == size && memcmp(buf, "ABCDEFG", 7) == 0);
	field = row->GetField("f_bit");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetULongLong() == 0x04F0);
	field = row->GetField("f_blob");
	EXPECT_TRUE(NULL != field && !field->IsNull());
	field->GetBlob(buf, size);
	EXPECT_TRUE(7 == size && memcmp(buf, "ABCDEFG", 7) == 0);
	field = row->GetField("f_boolean");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetBool());
	field = row->GetField("f_char");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetString().compare("ABCDEFG") == 0);
	field = row->GetField("f_date");
	EXPECT_TRUE(NULL != field && !field->IsNull());
	field = row->GetField("f_datetime");
	EXPECT_TRUE(NULL != field && !field->IsNull());
	field = row->GetField("f_decimal");
	EXPECT_TRUE(NULL != field && !field->IsNull() && fabsl(field->GetLongDouble() - 123.456) < 1e-6);
	field = row->GetField("f_enum");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetString().compare("v1") == 0);
	field = row->GetField("f_float");
	EXPECT_TRUE(NULL != field && !field->IsNull() && fabs(field->GetFloat() - 1.73f) < 1e-6);
	field = row->GetField("f_int");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetInt() == 123);
	field = row->GetField("f_set");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetString().compare("v2,v5") == 0);
	field = row->GetField("f_text");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetString().compare("ABCDEFG") == 0);
	field = row->GetField("f_time");
	EXPECT_TRUE(NULL != field && !field->IsNull());
	field = row->GetField("f_timestamp");
	EXPECT_TRUE(NULL != field && !field->IsNull());
	field = row->GetField("f_varbinary");
	field->GetBlob(buf, size);
	EXPECT_TRUE(NULL != field && !field->IsNull());
	EXPECT_TRUE(7 == size && memcmp(buf, "ABCDEFG", 7) == 0);
	field = row->GetField("f_varchar");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetString().compare("ABCDEFG") == 0);
	field = row->GetField("f_year");
	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetInt() == 2012);

	delete result_set;
	result_set = NULL;
	delete stmt;
	stmt = NULL;
}


TEST(MySqlTest, Statement_Select_EmptyResult) {
	Connection *conn = DBFactory::DefaultFactory(DB_MYSQL5)->CreateConnection();
	EXPECT_TRUE(NULL != conn);
	conn->SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
	conn->SetAutoCommit(true);
	conn->SetAutoReconnect(false);
	EXPECT_TRUE(conn->Open());
	if (!conn->IsOpened()) {
		delete conn;
		conn = NULL;
		return;
	}

	Statement *stmt = conn->CreateStatement();
	EXPECT_TRUE(NULL != stmt);
	std::string stmt_str;
	
	// Select not-existent row
	stmt_str = "SELECT * from TEST WHERE f_bigint = 20;";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	ResultSet *result_set = stmt->ExecuteQuery(NULL);
	EXPECT_TRUE(NULL != result_set && result_set->GetRowCount() == 0);

	delete result_set;
	result_set = NULL;
	delete stmt;
	stmt = NULL;
	delete conn;
	conn = NULL;
}

//TEST(MySqlTest, Statement_Select_SetTypeAsNumber) {
//	MySql_Connection conn;
//	conn.SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
//	conn.SetAutoCommit(true);
//	conn.SetAutoReconnect(false);
//	EXPECT_TRUE(conn.Open());
//	if (!conn.IsOpened()) {
//		return;
//	}
//
//	Statement *stmt = conn.CreateStatement();
//	EXPECT_TRUE(NULL != stmt);
//	std::string stmt_str;
//	
//	// Select not-existent row
//	stmt_str = "SELECT f_set+0 as f_set_as_num from TEST WHERE f_bigint = 2;";
//	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
//	ResultSet *result_set = stmt->ExecuteQuery(NULL);
//	EXPECT_TRUE(NULL != result_set && result_set->GetRowCount() == 1);
//	const DataRow *row = result_set->GetRow(0);
//	EXPECT_TRUE(NULL != row);
//	const DataField *field = row->GetField(0);
//	EXPECT_TRUE(NULL != field && !field->IsNull() && field->GetULongLong() == 0x12);
//
//	delete result_set;
//	result_set = NULL;
//	delete stmt;
//	stmt = NULL;
//}

TEST(MySqlTest, Statement_Update) {
	Connection *conn = DBFactory::DefaultFactory(DB_MYSQL5)->CreateConnection();
	EXPECT_TRUE(NULL != conn);
	conn->SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
	conn->SetAutoCommit(true);
	conn->SetAutoReconnect(false);
	EXPECT_TRUE(conn->Open());
	if (!conn->IsOpened()) {
		delete conn;
		conn = NULL;
		return;
	}

	Statement *stmt = conn->CreateStatement();
	EXPECT_TRUE(NULL != stmt);
	std::string stmt_str;
	stmt_str = "UPDATE TEST SET f_int=100 WHERE f_bigint = 1;";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	EXPECT_EQ(stmt->ExecuteNonQuery(NULL), 1);
	stmt_str = "UPDATE TEST SET f_int=100 WHERE f_bigint = 10;";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	EXPECT_EQ(stmt->ExecuteNonQuery(NULL), 0);

	delete stmt;
	stmt = NULL;
	delete conn;
	conn = NULL;
}

TEST(MySqlTest, Statement_Delete) {
	Connection *conn = DBFactory::DefaultFactory(DB_MYSQL5)->CreateConnection();
	EXPECT_TRUE(NULL != conn);
	conn->SetConnectionString("localhost", "root", "123456", "zdb_test", 0);
	conn->SetAutoCommit(true);
	conn->SetAutoReconnect(false);
	EXPECT_TRUE(conn->Open());
	if (!conn->IsOpened()) {
		delete conn;
		conn = NULL;
		return;
	}

	Statement *stmt = conn->CreateStatement();
	EXPECT_TRUE(NULL != stmt);
	std::string stmt_str;
	stmt_str = "DELETE FROM TEST WHERE f_bigint = 1;";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	EXPECT_EQ(stmt->ExecuteNonQuery(NULL), 1);
	stmt_str = "DELETE FROM TEST WHERE f_bigint = 10;";
	stmt->SetStatementString(stmt_str.c_str(), stmt_str.size());
	EXPECT_EQ(stmt->ExecuteNonQuery(NULL), 0);

	delete stmt;
	stmt = NULL;
	delete conn;
	conn = NULL;
}

TEST(MySqlTest, Statement_StoredProcedure) {
	// TODO
}

TEST(MySqlTest, PreparedStatement_Insert) {
}

