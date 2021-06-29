#pragma once

#include <cstdint>
#include <string>

namespace oc
{
	class ocVersion
	{
	private:
		uint32_t m_Major = 0;
		uint32_t m_Minor = 0;
		uint32_t m_Revision = 1;

	public:
		ocVersion() {};
		ocVersion(const uint32_t major, const uint32_t minor, const uint32_t revision);
		std::wstring GetVersionStringView() const;
		uint32_t GetMajor() const;
		uint32_t GetMinor() const;
		uint32_t GetRevision() const;
	};
}
