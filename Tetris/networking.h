
#ifndef NETWORKING_H
#define NETWORKING_H

#include <SDL.h>
#include <SDL_net.h>

#include <vector>

class Networking final
{
public:

    class ControlCommand
    {
    public:
        ControlCommand(unsigned char x, unsigned char y);
        ControlCommand(const std::vector<unsigned char>& bytes);

        void GetXY(unsigned char& outX, unsigned char& outY) const;
        std::vector<unsigned char> GetSerialised() const;

    private:
        void DecodeSerialised(const std::vector<unsigned char>& bytes);

        unsigned char m_x, m_y;
    };

    enum class Type : bool { SERVER, CLIENT };

    Networking(Type type);
    ~Networking();

    void PushBoardUpdate(const ControlCommand &command);
    bool PullBoardUpdate(ControlCommand &outCommand);

private:
    Type m_type;

    TCPsocket m_serverSocket, m_clientSocket;
    IPaddress m_serverIp, m_clientIp;

};

#endif // NETWORKING_H