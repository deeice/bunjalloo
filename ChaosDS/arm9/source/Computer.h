#ifndef Computer_h_seen
#define Computer_h_seen

class Computer
{

  public:
    virtual ~Computer() {}

    virtual void aiCastCreature() = 0;
    virtual void doAiSpell() = 0;
    virtual void doAiMovement() = 0;

};

#endif
