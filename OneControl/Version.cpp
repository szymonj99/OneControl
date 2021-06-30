#include "Version.h"

oc::ocVersion::ocVersion(const uint32_t major, const uint32_t minor, const uint32_t revision)
{
	m_Major = major;
	m_Minor = minor;
	m_Revision = revision;
}

std::string oc::ocVersion::GetVersionStringView() const
{
	return std::to_string(m_Major) + "." + std::to_string(m_Minor) + "." + std::to_string(m_Revision);
}

uint32_t oc::ocVersion::GetMajor() const
{
	return m_Major;
}
uint32_t oc::ocVersion::GetMinor() const
{
	return m_Minor;
}
uint32_t oc::ocVersion::GetRevision() const
{
	return m_Revision;
}