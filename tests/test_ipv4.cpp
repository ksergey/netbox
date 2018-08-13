// ------------------------------------------------------------
// Copyright (c) 2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#include <gtest/gtest.h>
#include <netbox/ipv4.hpp>

using namespace netbox;

TEST(IPv4, Address)
{
    using Address = IPv4::Address;

    Address address;
    ASSERT_EQ( address.toUint(), 0u );
    ASSERT_EQ( address, Address::any() );
    ASSERT_EQ( address.toBytes(), (Address::Bytes{0, 0, 0, 0}) );

    address = Address::loopback();
    ASSERT_EQ( address.toBytes(), (Address::Bytes{127, 0, 0, 1}) );

    Address xaddress = std::move(address);
    ASSERT_EQ( xaddress.toBytes(), (Address::Bytes{127, 0, 0, 1}) );
}

TEST(IPv4, Endpoint)
{
    using Endpoint = IPv4::Endpoint;
    using Address = IPv4::Address;

    Endpoint endpoint;
    ASSERT_EQ( endpoint.address().toUint(), 0u );
    ASSERT_EQ( endpoint.port(), 0u );

    endpoint.setAddress(Address::loopback());
    endpoint.setPort(6323);
    ASSERT_EQ( endpoint.address().toBytes(), (Address::Bytes{127, 0, 0, 1}) );
    ASSERT_EQ( endpoint.port(), 6323u );

    Endpoint xendpoint = std::move(endpoint);
    ASSERT_EQ( xendpoint.address().toBytes(), (Address::Bytes{127, 0, 0, 1}) );
    ASSERT_EQ( xendpoint.port(), 6323u );
}
