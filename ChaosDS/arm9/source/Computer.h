#ifndef Computer_h_seen
#define Computer_h_seen

class Computer
{

  public:
    //! Virtual destructor for base interface.
    virtual ~Computer() {}

    //! @return true if the computer has found a target square.
    virtual bool hasTargetSquare() const = 0;
    //! Set the target square value.
    virtual void setHasTargetSquare(bool) = 0;
    //! Make the computer cast the given spell id.
    virtual void aiCast(int spellType) = 0;
    //! start the computer spell casting.
    virtual void doAiSpell() = 0;
    //! start the computer movement.
    virtual void doAiMovement() = 0;

};

#endif
