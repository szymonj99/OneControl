#pragma once

#define UNICODE 1

#include <cstdint>
#include <string>

#include "../Helpers/Constants.h"

namespace oc
{
	class Version
	{
	private:
		VersionInt m_Major = 0;
		VersionInt m_Minor = 0;
		VersionInt m_Revision = 1;

	public:
		Version() {};
		Version(const VersionInt major, const VersionInt minor, const VersionInt revision);
		std::string GetVersionString() const;
		VersionInt GetMajor() const;
		VersionInt GetMinor() const;
		VersionInt GetRevision() const;
	};

	// The version of the application.
	const static Version kVersion(0, 0, 1);
}
