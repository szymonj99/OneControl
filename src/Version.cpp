#include <OneControl/Version.h>

/**
 * Create a new Version.
 * @param major Major version
 * @param minor Minor version
 * @param revision Revision
 */
oc::Version::Version(const oc::VersionInt major, const oc::VersionInt minor, const oc::VersionInt revision)
{
	m_Major = major;
	m_Minor = minor;
	m_Revision = revision;
}

/**
 * Convert the version to a string
 * @return A period-separeted string of the version
 */
std::string oc::Version::GetVersionString() const
{
	return std::to_string(m_Major) + "." + std::to_string(m_Minor) + "." + std::to_string(m_Revision);
}

/**
 * Get the major version
 * @return Value for the major version
 */
oc::VersionInt oc::Version::GetMajor() const
{
	return m_Major;
}

/**
 * Get the minor version
 * @return Value for the minor version
 */
oc::VersionInt oc::Version::GetMinor() const
{
	return m_Minor;
}

/**
 * Get the revision
 * @return Value for the revision
 */
oc::VersionInt oc::Version::GetRevision() const
{
	return m_Revision;
}

// TODO: Write an std::cout overload rather than calling GetVersionString
//operator<<()
