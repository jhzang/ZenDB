// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#include <zdb/datarow.h>

namespace zdb
{
	DataRow::DataRow()
	{
	}

	DataRow::~DataRow()
	{
		Clear();
	}

	void DataRow::Clear()
	{
		for (FieldVector::iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
			if (NULL != *it) {
				delete *it;
				*it = NULL;
			}
		}
		m_fields.clear();
	}

	const DataField* DataRow::GetField(const char *name) const
	{
		DataField *field = NULL;
		for (FieldVector::const_iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
			if (NULL != *it && (*it)->GetName().compare(name) == 0) {
				field = *it;
				break;
			}
		}
		return field;
	}

	std::string DataRow::ToString() const
	{
		std::stringstream ss;
		ss << "(";
		for (FieldVector::const_iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
			if (NULL != *it) {
				ss << (*it)->ToString() << ",";
			}
		}
		ss << ")";
		return ss.str();
	}

} // namespace zdb

