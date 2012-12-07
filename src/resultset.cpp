#include <zdb/resultset.h>

namespace zdb
{
	ResultSet::ResultSet()
	{
	}

	ResultSet::~ResultSet()
	{
		for (RowVector::iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
			if (NULL != *it) {
				delete *it;
				*it = NULL;
			}
		}
	}
} // namespace zdb

