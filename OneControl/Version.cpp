#include "Version.h"

oc::ocVersion::ocVersion(const uint32_t major, const uint32_t minor, const uint32_t revision)
{
	m_Major = major;
	m_Minor = minor;
	m_Revision = revision;
}

std::wstring oc::ocVersion::GetVersionStringView() const
{
	return L"" + std::to_wstring(m_Major) + L"." + std::to_wstring(m_Minor) + L"." + std::to_wstring(m_Revision);
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