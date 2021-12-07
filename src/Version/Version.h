#pragma once

#define UNICODE 1

#include <cstdint>
#include <string>

namespace oc
{
	class Version
	{
	private:
		uint32_t m_Major = 0;
		uint32_t m_Minor = 0;
		uint32_t m_Revision = 1;

	public:
		Version() {};
		Version(const uint32_t major, const uint32_t minor, const uint32_t revision);
		std::string GetVersionString() const;
		uint32_t GetMajor() const;
		uint32_t GetMinor() const;
		uint32_t GetRevision() const;
	};

	// The version of the application.
	const static oc::Version kVersion(0, 0, 1);
}
