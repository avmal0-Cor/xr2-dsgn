////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.12.2007
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "property_integer_enum_value.h"

using System::String;

property_integer_enum_value::property_integer_enum_value	(
		integer_getter_type^ getter,
		integer_setter_type^ setter,
		pair *values,
		u32 const &value_count
	) :
	inherited				(getter, setter),
	m_collection			(gcnew collection_type())
{
	for (u32 i=0; i<value_count; ++i) {
		ValuePair^			pair = gcnew ValuePair();
		pair->first			= values[i].first;
		pair->second		= gcnew System::String(values[i].second);
		m_collection->Add	(pair);
	}
}

System::Object ^property_integer_enum_value::get_value		()
{
	int						value = safe_cast<int>(inherited::get_value());
	for each (ValuePair^ i in m_collection) {
		if (i->first != value)
			continue;

		return				(value);
	}

	return					(safe_cast<ValuePair^>(m_collection[0])->first);
}

void property_integer_enum_value::set_value					(Object ^object)
{
	String^					string_value = dynamic_cast<String^>(object);

	for each (ValuePair^ i in m_collection) {
		if (!i->second->Equals(string_value))
			continue;

		inherited::set_value(i->first);
		return;
	}

	inherited::set_value	(safe_cast<ValuePair^>(m_collection[0])->first);
}