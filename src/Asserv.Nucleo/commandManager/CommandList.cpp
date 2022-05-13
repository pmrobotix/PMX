#include "CommandList.h"
#include "Commands/Command.h"
#include <cstdlib>

CommandList::CommandList(uint8_t nbElements, uint8_t elementSize)
{
    nextFreePos = 0;
    headPos = 0;
    full = false;
    nbElement = nbElements;
    commandList = (Command **) malloc(sizeof(Command *) * nbElement);
    for(int i=0; i<nbElement; i++)
    {
        commandList[i] = (Command *) malloc(elementSize);
    }
}

CommandList::~CommandList()
{

}

Command * CommandList::getFree()
{
    if (full)
        return nullptr;
    else
        return commandList[nextFreePos];
}

bool CommandList::push()
{
    if (full) 
    {
        return false;
    }
    else
    {
        nextFreePos = (nextFreePos + 1) % nbElement;

        if (nextFreePos == headPos)
            full = true;
        return true;
    }
}


uint8_t CommandList::size()
{
    if (full) 
        return nbElement;
    else if(headPos <= nextFreePos )
        return nextFreePos - headPos;
    else
        return nextFreePos + nbElement - headPos;
}

void CommandList::flush()
{
    nextFreePos = 0;
    headPos = 0;
    full = false;
}

Command* CommandList::getFirst()
{
    if (!full && nextFreePos == headPos) 
        return nullptr; // List is empty

    if (nextFreePos == headPos) 
        full = false;

    return commandList[headPos];
}

void CommandList::pop()
{
    if (!full && nextFreePos == headPos) 
        return; // List is empty

    if (nextFreePos == headPos) 
        full = false;

    headPos = (headPos + 1) % nbElement;
}

Command const* CommandList::getSecond()
{
    if( size() < 2 )
        return nullptr;

    return commandList[(headPos + 1) % nbElement];
}
