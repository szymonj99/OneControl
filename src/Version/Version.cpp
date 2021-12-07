#include "Version.h"

oc::Version::Version(const uint32_t major, const uint32_t minor, const uint32_t revision)
{
	m_Major = major;
	m_Minor = minor;
	m_Revision = revision;
}

std::string oc::Version::GetVersionString() const
{
	return std::to_string(m_Major) + "." + std::to_string(m_Minor) + "." + std::to_string(m_Revision);
}

uint32_t oc::Version::GetMajor() const
{
	return m_Major;
}
uint32_t oc::Version::GetMinor() const
{
	return m_Minor;
}
uint32_t oc::Version::GetRevision() const
{
	return m_Revision;
}
