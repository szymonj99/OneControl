#include <OneControl/Packet.h>

void oc::Packet::onReceive(const void* data, std::size_t size)
{
	append(data, size);
}

const void* oc::Packet::onSend(std::size_t& size)
{
    size = getDataSize();
    return getData();
}
