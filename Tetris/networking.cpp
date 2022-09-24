
#include "networking.h"

#include <exception>

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

    if (type == Type::SERVER)
    {
        bool resolvedHost = (SDLNet_ResolveHost(&m_serverIp, NULL, PORT) != -1);
        if (!resolvedHost)
            throw std::exception("SERVER: Could not resolve host");

        m_serverSocket = SDLNet_TCP_Open(&m_serverIp);
        if (!m_serverSocket)
            throw std::exception("SERVER: Could not open TCP socket");
    }
    else if (type == Type::CLIENT)
    {
        bool resolvedHost = (SDLNet_ResolveHost(&m_clientIp, "127.0.0.1", PORT) != -1);
        if (!resolvedHost)
            throw std::exception("CLIENT: Could not resolve host");

        m_clientSocket = SDLNet_TCP_Open(&m_clientIp);
        if (!m_clientSocket)
            throw std::exception("CLIENT: Could not open TCP socket");
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
    if (!m_clientSocket) // No client connected. Attempt to let one connect
    {
        m_clientSocket = SDLNet_TCP_Accept(m_serverSocket);
        if (!m_clientSocket)
            return false;
    }
        
    std::vector<unsigned char> message(4);
    int length = SDLNet_TCP_Recv(m_clientSocket, message.data(), message.size());
    if (!length)
        return false; // Nothing to receive

    outCommand = ControlCommand(message);
    return true;
}