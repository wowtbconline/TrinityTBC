
#include "GenericMovementGenerator.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "MoveSpline.h"
#include "Unit.h"

GenericMovementGenerator::GenericMovementGenerator(Movement::MoveSplineInit&& splineInit, MovementGeneratorType type, uint32 id) :
    _splineInit(std::move(splineInit)), _type(type), _pointId(id), _duration(0) 
{
    Mode = MOTION_MODE_DEFAULT;
    Priority = MOTION_PRIORITY_NORMAL;
    Flags = MOVEMENTGENERATOR_FLAG_INITIALIZATION_PENDING;
    BaseUnitState = UNIT_STATE_ROAMING;
}

void GenericMovementGenerator::Initialize(Unit* /*owner*/)
{
    if (HasFlag(MOVEMENTGENERATOR_FLAG_DEACTIVATED) && !HasFlag(MOVEMENTGENERATOR_FLAG_INITIALIZATION_PENDING)) // Resume spline is not supported
    {
        RemoveFlag(MOVEMENTGENERATOR_FLAG_DEACTIVATED);
        AddFlag(MOVEMENTGENERATOR_FLAG_FINALIZED);
        return;
    }

    RemoveFlag(MOVEMENTGENERATOR_FLAG_INITIALIZATION_PENDING | MOVEMENTGENERATOR_FLAG_DEACTIVATED);
    AddFlag(MOVEMENTGENERATOR_FLAG_INITIALIZED);

    _duration.Reset(_splineInit.Launch());
}

void GenericMovementGenerator::Reset(Unit* owner)
{
    Initialize(owner);
}

bool GenericMovementGenerator::Update(Unit* owner, uint32 diff)
{
    if (!owner || HasFlag(MOVEMENTGENERATOR_FLAG_FINALIZED))
        return false;

    _duration.Update(diff);
    if (_duration.Passed() || owner->movespline->Finalized())
    {
        AddFlag(MOVEMENTGENERATOR_FLAG_INFORM_ENABLED);
        return false;
    }

    return true;
}

void GenericMovementGenerator::Deactivate(Unit*)
{
    AddFlag(MOVEMENTGENERATOR_FLAG_DEACTIVATED);
}

void GenericMovementGenerator::Finalize(Unit* owner, bool/* active*/, bool movementInform)
{
    AddFlag(MOVEMENTGENERATOR_FLAG_FINALIZED);

    if (movementInform && HasFlag(MOVEMENTGENERATOR_FLAG_INFORM_ENABLED))
        MovementInform(owner);
}

void GenericMovementGenerator::MovementInform(Unit* owner)
{
    if (Creature* creature = owner->ToCreature())
        if (creature->AI())
            creature->AI()->MovementInform(_type, _pointId);
}