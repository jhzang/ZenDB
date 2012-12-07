// Copyright (c) 2012 Junheng Zang. All Rights Reserved.
//
#ifndef ZDB__PARAMETER_H
#define ZDB__PARAMETER_H

namespace zdb
{
	class Parameter
	{
	public:
		enum Direction { IN, OUT, INOUT };

	public:
		Parameter(const char *name, Direction dir) : m_name(name), m_direction(dir) {}
		virtual ~Parameter() {}

		const char* GetName() const { return m_name.c_str(); }
		Direction GetDirection() const { return m_direction; }
		DataField& GetData() { return m_data; }
		const DataField& GetData() const { return m_data; }

	private:
		std::string m_name; // parameter name
		Direction m_direction;
		DataField m_data;
	}; // class Parameter
} // namespace zdb
#endif

