// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZDB__DATATYPES_H
#define ZDB__DATATYPES_H

namespace zdb
{
	enum DataType
	{
		ZDB_UNKNOWN,
		ZDB_BOOL,
		ZDB_BYTE,
		ZDB_SHORT,
		ZDB_INT,
		ZDB_LONG,
		ZDB_LONGLONG,
		ZDB_UBYTE,
		ZDB_USHORT,
		ZDB_UINT,
		ZDB_ULONG,
		ZDB_ULONGLONG,
		ZDB_FLOAT,
		ZDB_DOUBLE,
		ZDB_LONGDOUBLE,
		ZDB_DATE,
		ZDB_TIME,
		ZDB_DATETIME,
		ZDB_TIMESTAMP,
		ZDB_STRING,
		ZDB_BLOB,
	};
} // namespace zdb
#endif

