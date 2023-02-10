#pragma once

#include <cstdint>
#include <string>

#include <OneControl/Constants.h>

namespace oc
{
	/**
	 * This class is used for authentication purposes. It's a quick check to see if the client and server are compatible.
	 */
	class Version
	{
	private:
		oc::VersionInt m_Major = 0;
		oc::VersionInt m_Minor = 0;
		oc::VersionInt m_Revision = 2;

	public:
		Version() = default;
		Version(const oc::VersionInt major, const oc::VersionInt minor, const oc::VersionInt revision);
		[[nodiscard]] std::string GetVersionString() const;
		[[nodiscard]] oc::VersionInt GetMajor() const;
		[[nodiscard]] oc::VersionInt GetMinor() const;
		[[nodiscard]] oc::VersionInt GetRevision() const;
	};

	// The version of the application.
	const static Version kVersion(0, 0, 2);
}
