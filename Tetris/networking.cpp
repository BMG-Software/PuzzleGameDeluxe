#include "networking.h"

Networking::Networking(Type type)
{
    // TODO: Setup as client or server depending on the type
}

void Networking::PushBoardUpdate(const std::vector<unsigned char>& serialisedBoard)
{
    // TODO: Push board update with std::async. Fire and forget
    // TODO: Std async function will put the response on the queue for next call to "pull"
}

bool Networking::PullBoardUpdate(std::vector<unsigned char>& serialisedBoard)
{
    // TODO: Check if there is a board update to pull
    // TODO: If not just continue. It's not our time yet 
    return false;
}
