#include "common.hpp"
#include "../utility/signatures/signatures.hpp"
#include "script_global.hpp"

script_global::script_global(std::size_t index) :
	m_index(index)
{
}

script_global script_global::at(std::ptrdiff_t index)
{
	return script_global(m_index + index);
}

script_global script_global::at(std::ptrdiff_t index, std::size_t size)
{
	return script_global(m_index + 1 + (index * size));
}

void* script_global::get()
{
	return Signatures::Instance()->definitions.m_script_globals[m_index >> 0x12 & 0x3F] + (m_index & 0x3FFFF);
}
