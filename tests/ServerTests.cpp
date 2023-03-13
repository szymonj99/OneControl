#include <catch2/catch_test_macros.hpp>

#include <OneControl/Client.h>
#include <OneControl/Server.h>

// TODO: Tests here

// How do I want to do this?

TEST_CASE("Client and Server authenticate correctly", "[ServerTests]")
{
    oc::Server server{};
    oc::Client client{};

    std::binary_semaphore semServerDone{0};
    std::binary_semaphore semClientDone{0};
    oc::ReturnCode serverStatus, clientStatus;
    std::jthread thServer([&]
    {
        if (server.WaitForClient() == oc::ReturnCode::Success)
        {
            serverStatus = server.AuthenticateClient();
            server.Disconnect();
            semServerDone.release();
        }
    });

    std::jthread thClient([&]
    {
        client.ConnectToServer(sf::IpAddress::LocalHost);
        clientStatus = client.AuthenticateToServer();
        client.Disconnect();
        semClientDone.release();
    });

    semServerDone.acquire();
    REQUIRE(serverStatus == oc::ReturnCode::Success);
    REQUIRE(clientStatus == oc::ReturnCode::Success);
    thServer.join();
    thClient.join();
}