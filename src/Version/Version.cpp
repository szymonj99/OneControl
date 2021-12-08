#include "Version.h"

oc::Version::Version(const VersionInt major, const VersionInt minor, const VersionInt revision)
{
	m_Major = major;
	m_Minor = minor;
	m_Revision = revision;
}

std::string oc::Version::GetVersionString() const
{
	return std::to_string(m_Major) + "." + std::to_string(m_Minor) + "." + std::to_string(m_Revision);
}

oc::VersionInt oc::Version::GetMajor() const
{
	return m_Major;
}

oc::VersionInt oc::Version::GetMinor() const
{
	return m_Minor;
}

oc::VersionInt oc::Version::GetRevision() const
{
	return m_Revision;
}
