#pragma once

#include <cstdint>
#include <string>

#include "../Helpers/Constants.h"

namespace oc
{
	class Version
	{
	private:
		oc::VersionInt m_Major = 0;
		oc::VersionInt m_Minor = 0;
		oc::VersionInt m_Revision = 1;

	public:
		Version() {};
		Version(const oc::VersionInt major, const oc::VersionInt minor, const oc::VersionInt revision);
		std::string GetVersionString() const;
		oc::VersionInt GetMajor() const;
		oc::VersionInt GetMinor() const;
		oc::VersionInt GetRevision() const;
	};

	// The version of the application.
	const static Version kVersion(0, 0, 1);
}
