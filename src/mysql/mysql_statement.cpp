// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#include <zdb/mysql/mysql_statement.h>
#include <zdb/mysql/mysql_connection.h>
#include <zdb/resultset.h>
#include <zdb/datarow.h>
#include <cstdlib>
#include <cstdio>

namespace zdb
{
	int MySql_Statement::Execute()
	{
		return mysql_real_query(&(((MySql_Connection*)m_conn)->m_mysql), m_stmt_str.c_str(), m_stmt_str.size());
	}

	ResultSet* MySql_Statement::FetchResult(MYSQL_RES *raw_result)
	{
		assert(NULL != raw_result);
		ResultSet *result_set = new ResultSet();
		assert(NULL != result_set);

		my_ulonglong row_count = mysql_num_rows(raw_result);
		unsigned int field_count = mysql_num_fields(raw_result);
		MYSQL_FIELD *field_defs = mysql_fetch_fields(raw_result);
		for (my_ulonglong row_index = 0; row_index < row_count; ++row_index) {
			MYSQL_ROW raw_row = mysql_fetch_row(raw_result);
			assert(NULL != raw_row);
			unsigned long *field_lengths = mysql_fetch_lengths(raw_result);
			assert(NULL != field_lengths);
			DataRow *data_row = new DataRow();
			assert(NULL != data_row);
			for (unsigned int field_index = 0; field_index < field_count; ++field_index) {
				const MYSQL_FIELD &cur_field_def = field_defs[field_index];
				unsigned long cur_field_len = field_lengths[field_index];
				DataField *data_field = new DataField();
				data_field->SetName(cur_field_def.name, cur_field_def.name_length);
				if (0 == cur_field_len) {
					data_field->SetNull(true);
				} else {
					switch (cur_field_def.type) {
						case MYSQL_TYPE_BIT:
						{
							unsigned long long value = 0;
							unsigned int bit_offset = 0;
							for (int i = cur_field_len - 1; i >= 0; --i) {
								unsigned long long tmp = static_cast<unsigned char>(raw_row[field_index][i]);
								value += (tmp << bit_offset);
								bit_offset += 8;
							}
							data_field->SetULongLong(value);
							break;
						}
						case MYSQL_TYPE_TINY:
							if (cur_field_def.flags & UNSIGNED_FLAG) {
								data_field->SetByte(strtol(raw_row[field_index], NULL, 10));
							} else {
								data_field->SetUByte(strtoul(raw_row[field_index], NULL, 10));
							}
							break;
						case MYSQL_TYPE_SHORT:
							if (cur_field_def.flags & UNSIGNED_FLAG) {
								data_field->SetShort(strtol(raw_row[field_index], NULL, 10));
							} else {
								data_field->SetUShort(strtoul(raw_row[field_index], NULL, 10));
							}
							break;
						case MYSQL_TYPE_INT24: // MEDIUMINT
							if (cur_field_def.flags & UNSIGNED_FLAG) {
								data_field->SetInt(strtol(raw_row[field_index], NULL, 10));
							} else {
								data_field->SetUInt(strtoul(raw_row[field_index], NULL, 10));
							}
							break;
						case MYSQL_TYPE_LONG:
							if (cur_field_def.flags & UNSIGNED_FLAG) {
								data_field->SetLong(strtol(raw_row[field_index], NULL, 10));
							} else {
								data_field->SetULong(strtoul(raw_row[field_index], NULL, 10));
							}
							break;
						case MYSQL_TYPE_LONGLONG:
							if (cur_field_def.flags & UNSIGNED_FLAG) {
								data_field->SetLongLong(strtoll(raw_row[field_index], NULL, 10));
							} else {
								data_field->SetULongLong(strtoull(raw_row[field_index], NULL, 10));
							}
							break;
						case MYSQL_TYPE_YEAR:
							data_field->SetInt(atoi(raw_row[field_index]));
							break;
						case MYSQL_TYPE_FLOAT:
							data_field->SetFloat(strtof(raw_row[field_index], NULL));
							break;
						case MYSQL_TYPE_DOUBLE:
							data_field->SetDouble(strtod(raw_row[field_index], NULL));
							break;
						case MYSQL_TYPE_DECIMAL:
						case MYSQL_TYPE_NEWDECIMAL:
							data_field->SetLongDouble(strtold(raw_row[field_index], NULL));
							break;
						case MYSQL_TYPE_DATE:
						case MYSQL_TYPE_NEWDATE:
						{
							int year, month, mday;
							sscanf(raw_row[field_index], "%d-%d-%d", &year, &month, &mday);
							DataField::Date date = { year, month, mday };
							data_field->SetDate(date);
							break;
						}
						case MYSQL_TYPE_TIME:
						{
							int hour, minute, second;
							sscanf(raw_row[field_index], "%d:%d:%d", &hour, &minute, &second);
							DataField::Time t = { hour, minute, second };
							data_field->SetTime(t);
							break;
						}
						case MYSQL_TYPE_DATETIME:
						{
							int year, month, mday, hour, minute, second;
							sscanf(raw_row[field_index], "%d-%d-%d %d:%d:%d", 
								&year, &month, &mday, &hour, &minute, &second);
							DataField::DateTime dt;
							dt.date_portion.year = year;
							dt.date_portion.month = month;
							dt.date_portion.mday = mday;
							dt.time_portion.hour = hour;
							dt.time_portion.minute = minute;
							dt.time_portion.second = second;
							data_field->SetDateTime(dt);
							break;
						}
						case MYSQL_TYPE_VARCHAR:
						case MYSQL_TYPE_ENUM:
						case MYSQL_TYPE_SET:
						case MYSQL_TYPE_VAR_STRING:
						case MYSQL_TYPE_STRING:
						case MYSQL_TYPE_GEOMETRY:
							data_field->SetString(raw_row[field_index], cur_field_len);
							break;
						case MYSQL_TYPE_TINY_BLOB:
						case MYSQL_TYPE_MEDIUM_BLOB:
						case MYSQL_TYPE_LONG_BLOB:
						case MYSQL_TYPE_BLOB:
							data_field->SetBlob(raw_row[field_index], cur_field_len);
							break;
						default:
							data_field->SetString(raw_row[field_index], cur_field_len);
							break;
					}
				}
				data_row->AddField(data_field);
			}
			result_set->AddRow(data_row);
		}
		return result_set;
	}

	int MySql_Statement::ExecuteNonQuery(int *errcode)
	{
		int dbcode = Execute();
		if (0 != dbcode) {
			if (NULL != errcode) {
				*errcode = dbcode;
			}
			return 0;
		}
		return mysql_affected_rows(&(((MySql_Connection*)m_conn)->m_mysql));
	}

	ResultSet* MySql_Statement::ExecuteQuery(int *errcode)
	{
		int retcode = Execute();
		if (retcode != 0) {
			if (NULL != errcode) {
				*errcode = retcode;
			}
			return NULL;
		}

		MYSQL *mysql = &(((MySql_Connection*)m_conn)->m_mysql);
		MYSQL_RES *raw_result = mysql_store_result(mysql);
		if (NULL == raw_result) {
			if (NULL != errcode) {
				*errcode = mysql_errno(mysql);
			}
			return NULL;
		}

		ResultSet *result_set = FetchResult(raw_result);
		mysql_free_result(raw_result);
		raw_result = NULL;
		if (NULL == result_set) {
			if (NULL != errcode) {
				*errcode = -1;
			}
		}
		return result_set;
	}

	int MySql_Statement::ExecuteQuery(std::vector<ResultSet*> &results)
	{
		int retcode = Execute();
		if (retcode != 0) {
			return retcode;
		}
		MYSQL *mysql = &(((MySql_Connection*)m_conn)->m_mysql);
		do {
			MYSQL_RES *raw_result = mysql_store_result(mysql);
			if (NULL == raw_result) {
				retcode = mysql_errno(mysql);
				break;
			}

			ResultSet *result_set = FetchResult(raw_result);
			mysql_free_result(raw_result);
			raw_result = NULL;
			if (NULL == result_set) {
				retcode = -1;
				break;
			}
			results.push_back(result_set);
		} while (mysql_next_result(mysql) == 0);
		return retcode;
	}

	DataField* MySql_Statement::ExecuteScalar()
	{
		// TODO
		return NULL;
	}
} // namespace zdb

