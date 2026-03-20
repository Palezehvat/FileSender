include_directories(protocol security)

add_executable(tests protocol/packet_serializer_test.cpp security/security_tests.cpp)

target_link_libraries(tests
    PRIVATE
    logger
    key_exchange
    libsodium_security
    protocol
    GTest::GTest
    GTest::Main
    sodium
)

include(GoogleTest)
gtest_discover_tests(tests)