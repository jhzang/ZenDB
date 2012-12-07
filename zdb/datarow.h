// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZDB__DATAROW_H
#define ZDB__DATAROW_H

#include <vector>
#include <zdb/datafield.h>

namespace zdb
{
	class DataField;

	class DataRow
	{
	public:
		DataRow();
		virtual ~DataRow();

		void Clear();
		inline void AddField(DataField *field);
		inline size_t GetFieldCount() const;
		inline const DataField* GetField(int index) const;
		const DataField* GetField(const char *name) const;
		std::string ToString() const;

	protected:
		DataRow(const DataRow &other) {}
		DataRow& operator = (const DataRow &other) { return *this; }

	private:
		typedef std::vector<DataField*> FieldVector;
		FieldVector m_fields;
	}; // class DataRow

	void DataRow::AddField(DataField *field)
	{
		m_fields.push_back(field);
	}

	size_t DataRow::GetFieldCount() const
	{
		return m_fields.size();
	}

	const DataField* DataRow::GetField(int index) const
	{
		return (index < 0 || index >= m_fields.size() ? NULL : m_fields[index]);
	}
} // namespace zdb
#endif

