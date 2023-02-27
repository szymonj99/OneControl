#include <OneControl/Packet.h>

// TODO: I wonder if we could stick the encryption/decryption in here instead.

void oc::Packet::onReceive(const void* data, std::size_t size)
{
	append(data, size);
}

const void* oc::Packet::onSend(std::size_t& size)
{
    size = getDataSize();
    return getData();
}
