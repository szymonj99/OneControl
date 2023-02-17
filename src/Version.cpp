#include <OneControl/Version.h>

namespace oc
{
    // TODO: Add string stream operators in the future.
    bool oc::Version::operator==(const oc::Version& kVersion)
    {
        return (this->Major == kVersion.Major && this->Minor == kVersion.Minor && this->Revision == kVersion.Revision);
    }

    sf::Packet& operator<<(sf::Packet& packet, const oc::Version& kVersion)
    {
        packet << kVersion.Major << kVersion.Minor << kVersion.Revision;
        return packet;
    }

    sf::Packet& operator>>(sf::Packet& packet, oc::Version& version)
    {
        packet >> version.Major >> version.Minor >> version.Revision;
        return packet;
    }
}