#ifndef Computer_h_seen
#define Computer_h_seen

class Computer
{

  public:
    virtual ~Computer() {}

    virtual bool hasTargetSquare() const = 0;
    virtual void setHasTargetSquare(bool) = 0;
    virtual void aiCast(int spellType) = 0;
    virtual void doAiSpell() = 0;
    virtual void doAiMovement() = 0;

};

#endif
