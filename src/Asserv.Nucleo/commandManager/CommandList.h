#ifndef CMD_LIST
#define CMD_LIST

#include <cstdint>

class Command;

class CommandList
{
public:
    explicit CommandList(uint8_t nbElement, uint8_t elementSize);
    ~CommandList();


    Command* getFree();
    bool push();

    Command* getFirst();
    void pop();
    Command const * getSecond();
    
    uint8_t size();
    void flush();

private:
    uint8_t nbElement;
    Command **commandList;
    uint8_t nextFreePos;
    uint8_t headPos;
    bool full;
};

#endif
