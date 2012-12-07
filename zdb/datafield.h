// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZDB__DATAFIELD_H
#define ZDB__DATAFIELD_H

#include <cstring>
#include <string>
#include <sstream>
#include <zdb/datatypes.h>

namespace zdb
{
	class DataField
	{
	public:
		struct Date
		{
			short year;
			unsigned char month; // [1,12]
			unsigned char mday;  // [1,31]
		};
		struct Time
		{
			unsigned char hour; // [0,23]
			unsigned char minute; // [0,59]
			unsigned char second; // [0, 60], 60 for leap second
		};
		struct DateTime
		{
			Date date_portion;
			Time time_portion;
		};

	private:
		union Storage
		{
			long long integer_value;
			long double floating_value;
			struct DateTime datetime_value;
			time_t timestamp_value;
			struct {
				char *ptr;
				size_t size;
			} buffer;
		};

	public:
		DataField();
		virtual ~DataField();

		void Clear();
		std::string ToString() const;

		inline const std::string& GetName() const;
		inline DataType GetType() const;
		inline bool IsNull() const;
		inline bool GetBool() const;
		inline char GetByte() const;
		inline short GetShort() const;
		inline int GetInt() const;
		inline long GetLong() const;
		inline long long GetLongLong() const;
		inline unsigned char GetUByte() const;
		inline unsigned short GetUShort() const;
		inline unsigned int GetUInt() const;
		inline unsigned long GetULong() const;
		inline unsigned long long GetULongLong() const;
		inline float GetFloat() const;
		inline double GetDouble() const;
		inline long double GetLongDouble() const;
		inline const Date& GetDate() const;
		inline const Time& GetTime() const;
		inline const DateTime& GetDateTime() const;
		inline time_t GetTimestamp() const;
		inline std::string GetString() const;
		inline void GetBlob(const void *&buf, size_t &size) const;

		inline void SetName(const char *name, size_t len);
		inline void SetNull(bool is_null);
		inline void SetBool(bool value);
		inline void SetByte(char value);
		inline void SetShort(short value);
		inline void SetInt(int value);
		inline void SetLong(long value);
		inline void SetLongLong(long long value);
		inline void SetUByte(unsigned char value);
		inline void SetUShort(unsigned short value);
		inline void SetUInt(unsigned int value);
		inline void SetULong(unsigned long value);
		inline void SetULongLong(unsigned long long value);
		inline void SetFloat(float value);
		inline void SetDouble(double value);
		inline void SetLongDouble(long double value);
		inline void SetDate(const Date &value);
		inline void SetTime(const Time &value);
		inline void SetDateTime(const DateTime &value);
		inline void SetTimestamp(time_t value);
		inline void SetString(char *str, size_t len);
		inline void SetBlob(const void *buf, size_t size);

	protected:
		DataField(const DataField &other) {}
		DataField& operator = (const DataField &other) { return *this; }

	private:
		std::string m_name;
		bool m_is_null;
		DataType m_type;
		Storage m_storage;
	}; // class DataField

	const std::string& DataField::GetName() const
	{
		return m_name;
	}

	DataType DataField::GetType() const
	{
		return m_type;
	}

	bool DataField::IsNull() const
	{
		return m_is_null;
	}

	bool DataField::GetBool() const
	{
		return static_cast<bool>(m_storage.integer_value);
	}

	char DataField::GetByte() const
	{
		return static_cast<char>(m_storage.integer_value);
	}

	short DataField::GetShort() const
	{
		return static_cast<short>(m_storage.integer_value);
	}

	int DataField::GetInt() const
	{
		return static_cast<int>(m_storage.integer_value);
	}

	long DataField::GetLong() const
	{
		return static_cast<long>(m_storage.integer_value);
	}

	long long DataField::GetLongLong() const
	{
		return m_storage.integer_value;
	}

	unsigned char DataField::GetUByte() const
	{
		return static_cast<unsigned char>(m_storage.integer_value);
	}

	unsigned short DataField::GetUShort() const
	{
		return static_cast<unsigned short>(m_storage.integer_value);
	}

	unsigned int DataField::GetUInt() const
	{
		return static_cast<unsigned int>(m_storage.integer_value);
	}

	unsigned long DataField::GetULong() const
	{
		return static_cast<unsigned long>(m_storage.integer_value);
	}

	unsigned long long DataField::GetULongLong() const
	{
		return static_cast<unsigned long long>(m_storage.integer_value);
	}

	float DataField::GetFloat() const
	{
		return static_cast<float>(m_storage.floating_value);
	}

	double DataField::GetDouble() const
	{
		return static_cast<double>(m_storage.floating_value);
	}

	long double DataField::GetLongDouble() const
	{
		return m_storage.floating_value;
	}

	const DataField::Date& DataField::GetDate() const
	{
		return m_storage.datetime_value.date_portion;
	}

	const DataField::Time& DataField::GetTime() const
	{
		return m_storage.datetime_value.time_portion;
	}

	const DataField::DateTime& DataField::GetDateTime() const
	{
		return m_storage.datetime_value;
	}

	time_t DataField::GetTimestamp() const
	{
		return m_storage.timestamp_value;
	}

	std::string DataField::GetString() const
	{
		return std::string(m_storage.buffer.ptr, m_storage.buffer.size);
	}

	void DataField::GetBlob(const void *&buf, size_t &size) const
	{
		buf = m_storage.buffer.ptr;
		size = m_storage.buffer.size;
	}

	void DataField::SetName(const char *name, size_t len)
	{
		m_name.assign(name, len);
	}

	void DataField::SetNull(bool is_null)
	{ m_is_null = is_null; Clear(); }

	void DataField::SetBool(bool value)
	{
		Clear();
		m_type = ZDB_BOOL;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetByte(char value)
	{
		Clear();
		m_type = ZDB_BYTE;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetShort(short value)
	{
		Clear();
		m_type = ZDB_SHORT;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetInt(int value)
	{
		Clear();
		m_type = ZDB_INT;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetLong(long value)
	{
		Clear();
		m_type = ZDB_LONG;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetLongLong(long long value)
	{
		Clear();
		m_type = ZDB_LONGLONG;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetUByte(unsigned char value)
	{
		Clear();
		m_type = ZDB_UBYTE;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetUShort(unsigned short value)
	{
		Clear();
		m_type = ZDB_USHORT;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetUInt(unsigned int value)
	{
		Clear();
		m_type = ZDB_UINT;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetULong(unsigned long value)
	{
		Clear();
		m_type = ZDB_ULONG;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetULongLong(unsigned long long value)
	{
		Clear();
		m_type = ZDB_ULONGLONG;
		m_is_null = false;
		m_storage.integer_value = value;
	}

	void DataField::SetFloat(float value)
	{
		Clear();
		m_type = ZDB_FLOAT;
		m_is_null = false;
		m_storage.floating_value = value;
	}

	void DataField::SetDouble(double value)
	{
		Clear();
		m_type = ZDB_DOUBLE;
		m_is_null = false;
		m_storage.floating_value = value;
	}

	void DataField::SetLongDouble(long double value)
	{
		Clear();
		m_type = ZDB_LONGDOUBLE;
		m_is_null = false;
		m_storage.floating_value = value;
	}

	void DataField::SetDate(const Date &value)
	{
		Clear();
		m_type = ZDB_DATE;
		m_is_null = false;
		m_storage.datetime_value.date_portion = value;
	}

	void DataField::SetTime(const Time &value)
	{
		Clear();
		m_type = ZDB_TIME;
		m_is_null = false;
		m_storage.datetime_value.time_portion = value;
	}

	void DataField::SetDateTime(const DateTime &value)
	{
		Clear();
		m_type = ZDB_DATETIME;
		m_is_null = false;
		m_storage.datetime_value = value;
	}

	void DataField::SetTimestamp(time_t value)
	{
		Clear();
		m_type = ZDB_TIMESTAMP;
		m_is_null = false;
		m_storage.timestamp_value = value;
	}

	void DataField::SetString(char *str, size_t len)
	{
		Clear(); 
		m_type = ZDB_STRING;
		m_is_null = false;
		if (NULL != str && len > 0) {
			m_storage.buffer.size = len;
			m_storage.buffer.ptr = new char[len];
			memcpy(m_storage.buffer.ptr, str, len);
		}
	}

	void DataField::SetBlob(const void *buf, size_t size)
	{
		Clear(); 
		m_type = ZDB_BLOB;
		m_is_null = false;
		if (NULL != buf && size > 0) {
			m_storage.buffer.size = size;
			m_storage.buffer.ptr = new char[size];
			memcpy(m_storage.buffer.ptr, buf, size);
		}
	}


} // namespace zdb
#endif

