#pragma once

#include <cstdint>
#include <string>

#include <SFML/Network/Packet.hpp>

#include <OneControl/Constants.h>

namespace oc
{
	/**
	 * This class is used for authentication purposes. It's a quick check to see if the client and server are compatible.
	 */
	struct Version
	{
		oc::VersionInt Major = 0;
		oc::VersionInt Minor = 0;
		oc::VersionInt Revision = 0;

		Version() = default;
		Version(const oc::VersionInt& kMajor, const oc::VersionInt& kMinor, const oc::VersionInt& kRevision) { this->Major = kMajor; this->Minor = kMinor; this->Revision = kRevision; }

		bool operator==(const oc::Version& kVersion);
	};

	static std::string VersionToString(const oc::Version& kVersion)
	{
		return std::to_string(kVersion.Major) + "." + std::to_string(kVersion.Minor) + "." + std::to_string(kVersion.Revision);
	}

	const static oc::Version kVersion(0, 0, 3);

	sf::Packet& operator<<(sf::Packet& packet, const oc::Version& kVersion);
	sf::Packet& operator>>(sf::Packet& packet, oc::Version& version);
}
