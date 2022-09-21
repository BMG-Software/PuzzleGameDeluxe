
#ifndef NETWORKING_H
#define NETWORKING_H

#include <vector>

class Networking
{
public:

    enum class Type : bool { SERVER, CLIENT };

    Networking(Type type);

    void PushBoardUpdate(const std::vector<unsigned char> &serialisedBoard);
    bool PullBoardUpdate(std::vector<unsigned char> &serialisedBoard);

private:

    // TODO: Thread safe data queue here


};

#endif // NETWORKING_H