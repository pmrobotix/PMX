#ifndef ASSERV_E_CMD_LIST
#define ASSERV_E_CMD_LIST

#include <sys/types.h>



// Internal Structures
enum typeCMD {
    CMD_NULL, // Erreur....
    CMD_GO, // Avancer ou reculer d'une certaine distance
    CMD_TURN, // Tourner d'un certain angle
    CMD_WAIT, // Attendre un certain temps ('fin, je suppose, c'est pas implémenté...)
    CMD_STOP, // Bon, là, je sais pas...
    CMD_GOTO, // Aller à un point précis
    CMD_GOTO_BACK, // Aller à un point précis en marche arrière
    CMD_GOTOANGLE, // Se tourner vers un point précis
    CMD_GOTOANGLEREVERSE, // Se tourner vers un point précis en marche arrière
    CMD_GOTOENCHAIN // Aller vers un point précis, mais si la commande suivante est un GOTO ou
    // un GOTOENCHAIN, on s'autorise à ne pas s'arrêter au point de consigne
};

typedef struct CMD_struct {
    typeCMD type;
    int64_t value;
    int64_t secValue;
} CMD;

class CMDList
{
public:
    CMDList(int capacity);
    bool enqueue(typeCMD cmd , int64_t val, int64_t val2 = 0);
    CMD dequeue();
    int size();
    ~CMDList();

private:
    CMD *list;
    int capa;
    int nextFreePos;
    int headPos;
    bool full;
};

#endif
