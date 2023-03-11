#include <OneControl/Version.h>

namespace oc
{
    std::istream& operator>>(std::istream& stream, oc::Version& version)
    {
        stream >> version.Major >> version.Minor >> version.Revision;
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const oc::Version& kVersion)
    {
        stream << kVersion.Major << " " << kVersion.Minor << " " << kVersion.Revision;
        return stream;
    }

    std::stringstream& operator>>(std::stringstream& stream, oc::Version& version)
    {
        stream >> version.Major >> version.Minor >> version.Revision;
        return stream;
    }

    std::istringstream& operator>>(std::istringstream& stream, oc::Version& version)
    {
        stream >> version.Major >> version.Minor >> version.Revision;
        return stream;
    }

    std::stringstream& operator<<(std::stringstream& stream, const oc::Version& kVersion)
    {
        stream << kVersion.Major << " " << kVersion.Minor << " " << kVersion.Revision;
        return stream;
    }

    std::ostringstream& operator<<(std::ostringstream& stream, const oc::Version& kVersion)
    {
        stream << kVersion.Major << " " << kVersion.Minor << " " << kVersion.Revision;
        return stream;
    }
}
