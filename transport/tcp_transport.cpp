#include "tcp_transport.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

namespace Transport {
    #ifdef _WIN32
        using Socket = SOCKET;
        constexpr Socket INVALID_SOCKET_VALUE = INVALID_SOCKET;
    #else
        using Socket = int;
        constexpr Socket INVALID_SOCKET_VALUE = -1;
    #endif

    struct TCPTransport::Impl {
        Socket socket = INVALID_SOCKET_VALUE;
    };

    static void initializedForWindows(std::shared_ptr<spdlog::logger> logger) {
        #ifdef _WIN32
            static int iResult = -1;
            static WSADATA wsaData;

            if (iResult == 0) return;

            iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
            if (iResult != 0) {
                logger->error("WSAStartup failed: {}", iResult);
                throw std::runtime_error("WSAStartup failed: " + std::to_string(iResult));
            }
        #endif
    }

    TCPTransport::TCPTransport(std::shared_ptr<spdlog::logger> logger)
    : logger(logger) {
        logger->trace("TCPTransport start initialization");
        impl =  std::make_unique<Impl>();
        #ifdef _WIN32
            initializedForWindows(logger);
            impl->socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        #else
            impl->socket = ::socket(AF_INET, SOCK_STREAM, 0);
        #endif

        if (impl->socket == INVALID_SOCKET_VALUE) {
            #ifdef _WIN32
                int err = WSAGetLastError();
                logger->error("socket() failed with error: {}", err);
            #endif
                logger->error("TCP socket was not created");
                throw std::runtime_error("TCP socket was not created");
        }
        logger->info("TCP constructor successfully ended");
    }

    void TCPTransport::connect(const std::string& host, uint16_t port) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (::inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
            #ifdef _WIN32
                int err = WSAGetLastError();
                logger->critical("Invalid IP address: {}. WSA error: {}", host, err);
            #else
                logger->critical("Invalid IP address: {}. Error: {}", host, strerror(errno));
            #endif
            throw std::runtime_error("Invalid IP address: " + host);
        }

        #ifdef _WIN32
            if (::connect(impl->socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
                int error = WSAGetLastError();
                logger->critical("Connection failed. WSA error: {}", error);
                throw std::runtime_error("Connection failed. WSA error " + std::to_string(error));
            }
        #else
            if (::connect(impl->socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
                logger->critical("Connection failed: {}", strerror(errno));
                throw std::runtime_error("Connection failed: " + std::string(strerror(errno)));
            }
        #endif

        logger->info("Connected to {}:{}", host, port);
    }

    void TCPTransport::listen(uint16_t port) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        int returnedValueFromBind = ::bind(impl->socket, reinterpret_cast<sockaddr*>(&addr), 
                                           sizeof(addr));
        #ifdef _WIN32
            if (returnedValueFromBind == SOCKET_ERROR) {
                int error = WSAGetLastError();
                logger->critical("Bind failed. WSA error: {}",  error);
                throw std::runtime_error("Bind failed. WSA error: " + std::to_string(error));
            }
        #else
            if (returnedValueFromBind < 0) {
                logger->critical("Bind failed: {}", strerror(errno));
                throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
            }
        #endif

        int returnedValueFromListen = ::listen(impl->socket, SOMAXCONN);
        #ifdef _WIN32
            if (returnedValueFromListen == SOCKET_ERROR) {
                int error = WSAGetLastError();
                logger->critical("Listen failed. WSA error: {}", error);
                throw std::runtime_error("Listen failed. WSA error " + std::to_string(error));
            }
        #else
            if (returnedValueFromListen < 0) {
                logger->critical("Listen failed: {}", strerror(errno));
                throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
            }
        #endif

        logger->info("Listening port: {}", port);
    }

    TCPTransport::TCPTransport(std::shared_ptr<spdlog::logger> logger, std::unique_ptr<Impl>&& impl)
        : logger(logger) {
        logger->trace("TCPTransport start initialization (after accept)");
        this->impl = std::move(impl);
        logger->info("TCP constructor successfully ended (after accept)");
    }

    std::unique_ptr<ITransport> TCPTransport::accept() {
        sockaddr_in addr{};
        socklen_t addrSize = sizeof(addr); 

        Socket clientSocket = ::accept(impl->socket, reinterpret_cast<sockaddr*>(&addr), 
                                               &addrSize);
        auto clientImpl = std::make_unique<Impl>();
        clientImpl->socket = clientSocket;
        #ifdef _WIN32
            if (clientSocket == INVALID_SOCKET_VALUE) {
                int error = WSAGetLastError();
                logger->critical("Accept failed. WSA error: {}", error);
                throw std::runtime_error("Accept failed. WSA error " + std::to_string(error));
            }
        #else
            if (clientSocket < 0) {
                logger->critical("Accept failed: {}", strerror(errno));
                throw std::runtime_error("Accept failed: " + std::string(strerror(errno)));
            }
        #endif
        
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ipStr, sizeof(ipStr));
        uint16_t port = ntohs(addr.sin_port);

        logger->info("Accepted TCP client {}:{}", ipStr, port);
        std::cout << "Accepted TCP client " << ipStr << ":" << port << std::endl;
        
        return std::unique_ptr<TCPTransport>(new TCPTransport(logger, std::move(clientImpl)));
    }

    void TCPTransport::sendAll(const void* buffer, std::size_t size) {
        std::size_t sentTotal = 0;
        const char* charBuffer = static_cast<const char*>(buffer);

        while (sentTotal < size) {
            #ifdef _WIN32
                int sent = ::send(
                    impl->socket,
                    charBuffer + sentTotal,
                    static_cast<int>(size - sentTotal),
                    0
                );
                if (sent == SOCKET_ERROR) {
                    int err = WSAGetLastError();
                    logger->critical("Send failed. WSA error: {}", err);
                    throw std::runtime_error("Send failed");
                }
            #else
                ssize_t sent = ::send(
                    impl->socket,
                    charBuffer + sentTotal,
                    size - sentTotal,
                    0
                );
                if (sent < 0) {
                    logger->critical("Send failed: {}", strerror(errno));
                    throw std::runtime_error("Send failed: " + std::string(strerror(errno)));
                }
            #endif

            if (sent == 0) {
                throw std::runtime_error("Connection closed by peer during send");
            }

            sentTotal += static_cast<std::size_t>(sent);
        }
    }

    void TCPTransport::send(const std::vector<uint8_t>& data) {
        uint32_t size = static_cast<uint32_t>(data.size());
        uint32_t toOneFormatSize = htonl(size);

        sendAll(&toOneFormatSize, sizeof(toOneFormatSize));

        if (size > 0) {
            sendAll(data.data(), size);
        }

        logger->info("Sent {} bytes", size);
    }

    void TCPTransport::recvAll(void* buffer, std::size_t size) {
        std::size_t received = 0;
        char* charBuffer = static_cast<char*>(buffer);

        while (received < size) {
            #ifdef _WIN32
                int getted = ::recv(
                    impl->socket,
                    charBuffer + received,
                    static_cast<int>(size - received),
                    0
                );
                if (getted == SOCKET_ERROR) {
                    int err = WSAGetLastError();
                    logger->critical("Recv failed. WSA error: {}", err);
                    throw std::runtime_error("Recv failed");
                }
            #else
                ssize_t getted = ::recv(
                    impl->socket,
                    charBuffer + received,
                    size - received,
                    0
                );
                if (getted < 0) {
                    logger->critical("Recv failed: {}", strerror(errno));
                    throw std::runtime_error(
                        "Recv failed: " + std::string(strerror(errno)));
                }
            #endif

            if (getted == 0) {
                throw std::runtime_error("Connection closed by peer");
            }

            received += static_cast<std::size_t>(getted);
        }
    }

    uint32_t TCPTransport::recvUint32() {
        uint32_t netSize;
        recvAll(&netSize, sizeof(netSize));
        return ntohl(netSize);
    }

    void TCPTransport::receive(std::vector<uint8_t>& data) {
        uint32_t size = recvUint32();
        if (size == 0) {
            data.clear();
            return;
        }

        data.resize(size);
        recvAll(data.data(), size);

        logger->info("Received {} bytes", size);
    }

    void TCPTransport::close() {
        if (impl->socket != INVALID_SOCKET_VALUE) {
            #ifdef _WIN32
                ::closesocket(impl->socket);
            #else
                ::close(impl->socket);
            #endif
            impl->socket = INVALID_SOCKET_VALUE;
            logger->info("TCP socket closed"); 
        }
    }

    TCPTransport::~TCPTransport() {
        logger->trace("TCPTransport deleted");
        close();
    }

} // Transport