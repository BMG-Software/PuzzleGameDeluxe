#include "networking.h"

#include <future>

template <typename F, typename... Ts>
inline std::future<std::result_of_t<F(Ts...)>>
    ReallyAsync(F&& f, Ts &&... params)
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}


Networking::ControlCommand::ControlCommand(unsigned char x, unsigned char y)
{
    m_x = x;
    m_y = y;
}

Networking::ControlCommand::ControlCommand(const std::vector<unsigned char>& bytes)
{
    m_x = 0;
    m_y = 0;
    DecodeSerialised(bytes);
}

void Networking::ControlCommand::GetXY(unsigned char& outX, unsigned char& outY) const
{
    outX = m_x;
    outY = m_y;
}

std::vector<unsigned char> Networking::ControlCommand::GetSerialised() const
{
    return { m_x, ',', m_y };
}

void Networking::ControlCommand::DecodeSerialised(const std::vector<unsigned char>& bytes)
{
    m_x = bytes[0];
    m_y = bytes[2];
}

const int PORT = 1234;

Networking::Networking(Type type)
{
    m_type = type;
    m_serverSocket = NULL;
    m_clientSocket = NULL;
    m_serverIp = NULL;
    m_clientIp = NULL;

    if (type == Type::SERVER)
    {
        bool resolvedHost = (SDLNet_ResolveHost(m_serverIp, NULL, PORT) != -1);
        if (!resolvedHost)
            throw std::runtime_error("SERVER: Could not resolve host");

        m_serverSocket = SDLNet_TCP_Open(m_serverIp);
        if (!m_serverSocket)
            throw std::runtime_error("SERVER: Could not open TCP socket");
    }
    else if (type == Type::CLIENT)
    {
        bool resolvedHost = (SDLNet_ResolveHost(m_clientIp, "127.0.0.1", PORT) != -1);
        if (!resolvedHost)
            throw std::runtime_error("CLIENT: Could not resolve host");

        m_clientSocket = SDLNet_TCP_Open(m_clientIp);
        if (!m_clientSocket)
            throw std::runtime_error("CLIENT: Could not open TCP socket");
    }
}

Networking::~Networking()
{
    if (m_serverSocket)
    {
        SDLNet_TCP_Close(m_serverSocket);
    }

    if (m_clientSocket)
    {
        SDLNet_TCP_Close(m_clientSocket);
    }
}

void Networking::PushBoardUpdate(const ControlCommand& command)
{
    if (m_clientSocket)
    {
        auto serialised = command.GetSerialised();
        int length = SDLNet_TCP_Send(m_clientSocket, serialised.data(), serialised.size());
        if (length < serialised.size())
            return; // Failed to send correctly
    }
}

bool Networking::PullBoardUpdate(ControlCommand& outCommand)
{
    if (!m_clientSocket || !m_clientIp) // No client connected. Attempt to let one connect
    {
        m_clientSocket = SDLNet_TCP_Accept(m_serverSocket);
        if (!m_clientSocket)
            return false;

        m_clientIp = SDLNet_TCP_GetPeerAddress(m_clientSocket);
        if (!m_clientIp)
            return false;

        // TODO: from example, could print connection as debug info here
    }
        
    std::vector<unsigned char> message(4);
    int length = SDLNet_TCP_Recv(m_clientSocket, message.data(), message.size());
    if (!length)
        return false; // Nothing to receive

    outCommand = ControlCommand(message);
    return true;
}

//void Networking::PushBoardUpdateAsync(const ControlCommand &command)
//{
//    // TODO: Push board update with std::async. Fire and forget
//    // TODO: Std async function will put the response on the queue for next call to "pull"
//
//    auto serverSend = [this]()
//    {
//        std::lock_guard<decltype(m_sendingLock)> lock(m_sendingLock);
//
//    };
//}
//
//bool Networking::PullBoardUpdateAsync(ControlCommand &outCommand)
//{
//    auto serverRecv = [ this ]() 
//    {
//        std::lock_guard<decltype(m_receivingLock)> lock(m_receivingLock);
//        if (!m_clientSocket || !m_clientIp) // No client connected. Attempt to let one connect
//        {
//            m_clientSocket = SDLNet_TCP_Accept(m_serverSocket);
//            if (!m_clientSocket)
//                return;
//
//            m_clientIp = SDLNet_TCP_GetPeerAddress(m_clientSocket);
//            if (!m_clientIp)
//                return;
//
//            // TODO: from example, could print connection as debug info here
//        }
//        else
//        {
//            std::vector<unsigned char> message(4);
//            int length = SDLNet_TCP_Recv(m_clientSocket, message.data(), message.size());
//            if (!length)
//                return; // Nothing to receive
//
//            m_commands.Push(ControlCommand(message));
//        }
//    };
//
//    // TODO: Client receive code
//    auto clientRecv = []() {};
//
//    // Asynchronously start a server receive attempt (if required)
//    if (!m_receivingLock.owns_lock())
//    {
//        if (m_type == Type::SERVER)
//        {
//            ReallyAsync(serverRecv);
//        }
//        else if (m_type == Type::CLIENT)
//        {
//            ReallyAsync(clientRecv);
//        }
//    }
//
//    return m_commands.TryPop(outCommand);
//}
