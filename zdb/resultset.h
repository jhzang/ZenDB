// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZDB__RESULTSET_H
#define ZDB__RESULTSET_H

#include <vector>
#include <zdb/datarow.h>

namespace zdb
{
	class ResultSet
	{
	public:
		ResultSet();
		virtual ~ResultSet();

		inline void AddRow(DataRow *r);
		inline size_t GetRowCount() const;
		inline const DataRow* GetRow(size_t index) const;

	private:
		typedef std::vector<DataRow*> RowVector;
		RowVector m_rows;
	};

	void ResultSet::AddRow(DataRow *r)
	{
		m_rows.push_back(r);
	}

	size_t ResultSet::GetRowCount() const
	{
		return m_rows.size();
	}

	const DataRow* ResultSet::GetRow(size_t index) const
	{
		return (index >= m_rows.size() ? NULL : m_rows[index]);
	}

} // namespace zdb
#endif

