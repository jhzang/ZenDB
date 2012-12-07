// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#include <zdb/datafield.h>
#include <cstring>
#include <string>
#include <sstream>
#include <zdb/datatypes.h>

namespace zdb
{
	DataField::DataField()
		: m_is_null(false), m_type(ZDB_UNKNOWN)
	{
		memset(&m_storage, 0, sizeof(m_storage));
	}

	DataField::~DataField()
	{
		Clear();
	}

	void DataField::Clear()
	{
		if (ZDB_STRING == m_type || ZDB_BLOB == m_type) {
			if (NULL != m_storage.buffer.ptr) {
				delete [] m_storage.buffer.ptr;
				m_storage.buffer.ptr = NULL;
				m_storage.buffer.size = 0;
			}
		}
		memset(&m_storage, 0, sizeof(m_storage));
	}

	std::string DataField::ToString() const
	{
		if (m_is_null) {
			return "";
		}
		std::stringstream ss;
		switch (m_type) {
			case ZDB_BOOL:
				ss << (m_storage.integer_value ? "TRUE" : "FALSE");
				break;
			case ZDB_BYTE:
			case ZDB_SHORT:
			case ZDB_INT:
			case ZDB_LONG:
			case ZDB_LONGLONG:
				ss << m_storage.integer_value;
				break;
			case ZDB_UBYTE:
			case ZDB_USHORT:
			case ZDB_UINT:
			case ZDB_ULONG:
			case ZDB_ULONGLONG:
				ss << static_cast<unsigned long long>(m_storage.integer_value);
				break;
			case ZDB_FLOAT:
			case ZDB_DOUBLE:
			case ZDB_LONGDOUBLE:
				ss << m_storage.floating_value;
				break;
			case ZDB_DATE:
				ss << "\"" << m_storage.datetime_value.date_portion.year << "-"
				   << m_storage.datetime_value.date_portion.month << "-"
				   << m_storage.datetime_value.date_portion.mday << "\"";
				break;
			case ZDB_TIME:
				ss << "\"" << m_storage.datetime_value.time_portion.hour << ":"
				   << m_storage.datetime_value.time_portion.minute << ":"
				   << m_storage.datetime_value.time_portion.second << "\"";
				break;
			case ZDB_DATETIME:
				ss << "\"" << m_storage.datetime_value.date_portion.year << "-"
				   << m_storage.datetime_value.date_portion.month << "-"
				   << m_storage.datetime_value.date_portion.mday << " "
				   << m_storage.datetime_value.time_portion.hour << ":"
				   << m_storage.datetime_value.time_portion.minute << ":"
				   << m_storage.datetime_value.time_portion.second << "\"";
				break;
			case ZDB_TIMESTAMP:
				ss << m_storage.timestamp_value;
				break;
			case ZDB_STRING:
				ss << "\"";
				ss.write(m_storage.buffer.ptr, m_storage.buffer.size);
				ss << "\"";
				break;
			case ZDB_BLOB:
			{
				ss << "\"";
				for (size_t i = 0; i < m_storage.buffer.size; ++i) {
					ss << std::hex << *(m_storage.buffer.ptr + i);
				}
				ss << "\"";
				break;
			}
			default:
				break;
		}
		return ss.str();
	}

} // namespace zdb

