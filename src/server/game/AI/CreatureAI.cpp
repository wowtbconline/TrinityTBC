#include "CreatureAI.h"
#include "Log.h"
#include "Creature.h"
#include "Player.h"
#include "Pet.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellHistory.h"
#include "World.h"
#include "CreatureTextMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Language.h"
#include "AreaBoundary.h"

CreatureAI::CreatureAI(Creature *c) : UnitAI((Unit*)c), me(c), _boundary(nullptr), _negateBoundary(false), _isEngaged(false), _moveInLOSLocked(false)
{

}

//Disable CreatureAI when charmed
void CreatureAI::OnCharmed(bool isNew)
{
    if (isNew && !me->IsCharmed() && me->LastCharmerGUID)
    {
        if (!me->HasReactState(REACT_PASSIVE))
            if (Unit* lastCharmer = ObjectAccessor::GetUnit(*me, me->LastCharmerGUID))
                me->EngageWithTarget(lastCharmer);

        me->LastCharmerGUID.Clear();
    }
    UnitAI::OnCharmed(isNew);
}

void CreatureAI::Talk(uint8 id, WorldObject const* whisperTarget /*= nullptr*/)
{
    sCreatureTextMgr->SendChat(me, id, whisperTarget);
}

// scripts does not take care about MoveInLineOfSight loops
// MoveInLineOfSight can be called inside another MoveInLineOfSight and cause stack overflow
void CreatureAI::MoveInLineOfSight_Safe(Unit* who)
{
    if (_moveInLOSLocked == true)
        return;
    _moveInLOSLocked = true;
    MoveInLineOfSight(who);
    _moveInLOSLocked = false;
}

void CreatureAI::MoveInLineOfSight(Unit* who)
{
    if (me->IsEngaged())
        return;

    if (me->HasReactState(REACT_AGGRESSIVE) && me->CanStartAttack(who, false))
        me->EngageWithTarget(who);
}

void CreatureAI::OnOwnerCombatInteraction(Unit* target)
{
    if (!target || !me->IsAlive())
        return;

    if (!me->HasReactState(REACT_PASSIVE) && me->CanStartAttack(target, true))
        me->EngageWithTarget(target);
}

bool CreatureAI::UpdateVictim()
{
    if (!IsEngaged())
        return false;

    if (!me->IsAlive())
    {
        EngagementOver();
        return false;
    }

    if (!me->HasReactState(REACT_PASSIVE))
    {
        if (Unit* victim = me->SelectVictim())
            if (!me->HasSpellFocus() && (victim != me->GetVictim() || me->GetMotionMaster()->GetCurrentMovementGeneratorType() != CHASE_MOTION_TYPE)) //sun: also start attack if we're not chasing... we might have triggered another movement in scripts
                AttackStart(victim);

        return me->GetVictim() != nullptr;
    }
    else if (!me->IsInCombat())
    {
        EnterEvadeMode(EVADE_REASON_NO_HOSTILES);
        return false;
    }
    else if (me->GetVictim())
        me->AttackStop();

    return true;
}

static bool ShouldFollowOnSpawn(SummonPropertiesEntry const* properties)
{
    // Summons without SummonProperties are generally scripted summons that don't belong to any owner
    if (!properties)
        return false;

    switch (properties->Category)
    {
        case SUMMON_CATEGORY_PET:
            return true;
        case SUMMON_CATEGORY_WILD:
        case SUMMON_CATEGORY_ALLY:
            switch (properties->Type)
            {
                case SUMMON_TYPE_PET:
                case SUMMON_TYPE_GUARDIAN:
                case SUMMON_TYPE_GUARDIAN2:
                case SUMMON_TYPE_MINION:
                case SUMMON_TYPE_MINIPET:
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }
}

void CreatureAI::JustAppeared()
{
    if (!IsEngaged())
    {
        if (TempSummon* summon = me->ToTempSummon())
        {
            // Only apply this to specific types of summons
            if (ShouldFollowOnSpawn(summon->m_Properties) && summon->CanFollowOwner())
            {
                if (Unit* owner = summon->GetCharmerOrOwner())
                {
                    summon->GetMotionMaster()->Clear();
                    summon->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, summon->GetFollowAngle());
                }
            }
        }
    }
}

void CreatureAI::JustEnteredCombat(Unit* who)
{
    if (!IsEngaged() && !me->CanHaveThreatList())
        EngagementStart(who);
}

void CreatureAI::EnterEvadeMode(EvadeReason why)
{
    if (!_EnterEvadeMode(why))
        return;

    if (!me->IsAlive())
        return;

    //TC_LOG_DEBUG("entities.unit", "Creature %u enters evade mode.", me->GetEntry());

#ifdef LICH_KING
    if (!me->GetVehicle()) // otherwise me will be in evade mode forever
#endif
    {
        if (Unit* owner = me->GetCharmerOrOwner())
        {
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
        }
        else
        {
            //UNIT_STATE_EVADE handled by HomeMovementGenerator
            me->GetMotionMaster()->MoveTargetedHome();
        }
    }

    Reset();

#ifdef LICH_KING
    if (me->IsVehicle()) // use the same sequence of addtoworld, aireset may remove all summons!
        me->GetVehicleKit()->Reset(true);
#endif
}

void CreatureAI::EngagementStart(Unit* who)
{
    if (_isEngaged)
    {
        TC_LOG_ERROR("scripts.ai", "CreatureAI::EngagementStart called even though creature is already engaged. Creature debug info:\n%s", me->GetDebugInfo().c_str());
        return;
    }
    _isEngaged = true;

    me->AtEngage(who);
}

void CreatureAI::EngagementOver()
{
    if (!_isEngaged)
    {
        TC_LOG_ERROR("scripts.ai", "CreatureAI::EngagementOver called even though creature is not currently engaged. Creature debug info:\n%s", me->GetDebugInfo().c_str());
        return;
    }
    _isEngaged = false;

    me->AtDisengage();
}

bool CreatureAI::_EnterEvadeMode(EvadeReason /*why*/)
{
    if (!IsEngaged())
        return false;

    if (!me->IsAlive())
    {
        EngagementOver();
        return false;
    }

    me->RemoveAurasOnEvade();

    me->CombatStop(true);
    me->LoadCreaturesAddon();
    me->SetLootRecipient(nullptr);
    me->ResetPlayerDamageReq();
    me->SetLastDamagedTime(0);
    me->SetCannotReachTarget(false);
    me->DoNotReacquireSpellFocusTarget();
    me->SetTarget(ObjectGuid::Empty);
    me->GetSpellHistory()->ResetAllCooldowns();
    EngagementOver();

    return true;
}

bool CreatureAI::IsInMeleeRange() const
{
    Unit* victim = me->GetVictim();
    if (!victim)
        return false;

    //check victim first to speed up most cases
    if (me->IsWithinMeleeRange(victim))
        return true;

    Map* pMap = me->GetMap();
    Map::PlayerList const &PlayerList = pMap->GetPlayers();
    if (!PlayerList.isEmpty())
    {
        for (const auto & i : PlayerList)
        {
            if (me->IsWithinMeleeRange(i.GetSource()))
                return true;
        }
    }
    return false;
}

void CreatureAI::DoZoneInCombat(Creature* creature /*= nullptr*/)
{
    if (!creature)
        creature = me;

    Map* map = creature->GetMap();
    if (!map->IsDungeon())                                  //use IsDungeon instead of Instanceable, in case battlegrounds will be instantiated
    {
        TC_LOG_ERROR("misc", "DoZoneInCombat call for map that isn't an instance (creature entry = %d)", creature->GetTypeId() == TYPEID_UNIT ? creature->ToCreature()->GetEntry() : 0);
        return;
    }

    Map::PlayerList const& playerList = map->GetPlayers();
    if (playerList.isEmpty())
        return;

    for (auto const& ref : playerList)
        if (Player* player = ref.GetSource())
        {
            if (!player->IsAlive() || !CombatManager::CanBeginCombat(creature, player))
                continue;
            creature->EngageWithTarget(player);
            for (Unit* pet : player->m_Controlled)
                creature->EngageWithTarget(pet);
#ifdef LICH_KING
            if (Unit* vehicle = player->GetVehicleBase())
                creature->EngageWithTarget(vehicle);
#endif
        }
}

static const uint32 BOUNDARY_VISUALIZE_CREATURE = 15425;
static const float BOUNDARY_VISUALIZE_CREATURE_SCALE = 0.25f;
static const int8 BOUNDARY_VISUALIZE_STEP_SIZE = 1;
static const int32 BOUNDARY_VISUALIZE_FAILSAFE_LIMIT = 750;
static const float BOUNDARY_VISUALIZE_SPAWN_HEIGHT = 5.0f;
int32 CreatureAI::VisualizeBoundary(uint32 duration, Unit* owner, bool fill) const
{
    typedef std::pair<int32, int32> coordinate;

    if (!owner)
        return -1;
    
    if (!_boundary || _boundary->empty())
        return LANG_CREATURE_MOVEMENT_NOT_BOUNDED;

    std::queue<coordinate> Q;
    std::unordered_set<coordinate> alreadyChecked;
    std::unordered_set<coordinate> outOfBounds;

    Position startPosition = owner->GetPosition();
    if (!IsInBoundary(&startPosition))
    { // fall back to creature position
        startPosition = me->GetPosition();
        if (!IsInBoundary(&startPosition))
        { // fall back to creature home position
            startPosition = me->GetHomePosition();
            if (!IsInBoundary(&startPosition))
                return LANG_CREATURE_NO_INTERIOR_POINT_FOUND;
        }
    }
    float spawnZ = startPosition.GetPositionZ() + BOUNDARY_VISUALIZE_SPAWN_HEIGHT;

    bool boundsWarning = false;
    Q.push({ 0,0 });
    while (!Q.empty())
    {
        coordinate front = Q.front();
        bool hasOutOfBoundsNeighbor = false;
        for (coordinate off : std::initializer_list<coordinate>{ { 1,0 },{ 0,1 },{ -1,0 },{ 0,-1 } })
        {
            coordinate next(front.first + off.first, front.second + off.second);
            if (next.first > BOUNDARY_VISUALIZE_FAILSAFE_LIMIT || next.first < -BOUNDARY_VISUALIZE_FAILSAFE_LIMIT || next.second > BOUNDARY_VISUALIZE_FAILSAFE_LIMIT || next.second < -BOUNDARY_VISUALIZE_FAILSAFE_LIMIT)
            {
                boundsWarning = true;
                continue;
            }
            if (alreadyChecked.find(next) == alreadyChecked.end()) // never check a coordinate twice
            {
                Position nextPos(startPosition.GetPositionX() + next.first*BOUNDARY_VISUALIZE_STEP_SIZE, startPosition.GetPositionY() + next.second*BOUNDARY_VISUALIZE_STEP_SIZE, startPosition.GetPositionZ());
                if (IsInBoundary(&nextPos))
                    Q.push(next);
                else
                {
                    outOfBounds.insert(next);
                    hasOutOfBoundsNeighbor = true;
                }
                alreadyChecked.insert(next);
            }
            else
                if (outOfBounds.find(next) != outOfBounds.end())
                    hasOutOfBoundsNeighbor = true;
        }
        if (fill || hasOutOfBoundsNeighbor)
            if (TempSummon* point = owner->SummonCreature(BOUNDARY_VISUALIZE_CREATURE, Position(startPosition.GetPositionX() + front.first*BOUNDARY_VISUALIZE_STEP_SIZE, startPosition.GetPositionY() + front.second*BOUNDARY_VISUALIZE_STEP_SIZE, spawnZ), TEMPSUMMON_TIMED_DESPAWN, duration * IN_MILLISECONDS))
            {
                point->SetDisableGravity(true);
                point->SetObjectScale(BOUNDARY_VISUALIZE_CREATURE_SCALE);
                point->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                point->SetImmuneToAll(true);
                if (!hasOutOfBoundsNeighbor)
                    point->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        Q.pop();
    }
    return boundsWarning ? LANG_CREATURE_MOVEMENT_MAYBE_UNBOUNDED : 0;
}

bool CreatureAI::IsInBoundary(Position const* who) const
{
    if (!_boundary)
        return true;

    if (!who)
        who = me;

    return (CreatureAI::IsInBounds(*_boundary, who) != _negateBoundary);
}

bool CreatureAI::IsInBounds(CreatureBoundary const& boundary, Position const* pos)
{
    for (AreaBoundary const* areaBoundary : boundary)
        if (!areaBoundary->IsWithinBoundary(pos))
            return false;

    return true;
}

bool CreatureAI::CheckInRoom()
{
    if (IsInBoundary())
        return true;
    else
    {
        EnterEvadeMode(EVADE_REASON_BOUNDARY);
        return false;
    }
}

void CreatureAI::SetBoundary(CreatureBoundary const* boundary, bool negateBoundaries /*= false*/)
{
    _boundary = boundary;
    _negateBoundary = negateBoundaries;
    me->DoImmediateBoundaryCheck();
}

Creature* CreatureAI::DoSummon(uint32 entry, Position const& pos, uint32 despawnTime, TempSummonType summonType)
{
	return me->SummonCreature(entry, pos, summonType, despawnTime);
}

Creature* CreatureAI::DoSummon(uint32 entry, WorldObject* obj, float radius, uint32 despawnTime, TempSummonType summonType)
{
	Position pos = obj->GetRandomNearPosition(radius);
	return me->SummonCreature(entry, pos, summonType, despawnTime);
}

Creature* CreatureAI::DoSummonFlyer(uint32 entry, WorldObject* obj, float flightZ, float radius, uint32 despawnTime, TempSummonType summonType)
{
	Position pos = obj->GetRandomNearPosition(radius);
	pos.m_positionZ += flightZ;
	return me->SummonCreature(entry, pos, summonType, despawnTime);
}
