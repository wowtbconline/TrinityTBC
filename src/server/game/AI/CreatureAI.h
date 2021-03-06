
#ifndef TRINITY_CREATUREAI_H
#define TRINITY_CREATUREAI_H

#include "Define.h"
#include "UnitAI.h"
#include "Optional.h"
#include "QuestDef.h"

class AreaBoundary;
class Unit;
class Creature;
class Player;
class GameObject;
class GameObjectAI;
class SpellInfo;
class PlayerAI;

typedef std::vector<AreaBoundary const*> CreatureBoundary;

#define TIME_INTERVAL_LOOK   5000
#define VISIBILITY_RANGE    10000

//Spell targets used by SelectSpell
enum SelectSpellTarget
{
    SELECT_TARGET_DONTCARE = 0,                             //All target types allowed

    SELECT_TARGET_SELF,                                     //Only Self casting

    SELECT_TARGET_SINGLE_ENEMY,                             //Only Single Enemy
    SELECT_TARGET_AOE_ENEMY,                                //Only AoE Enemy
    SELECT_TARGET_ANY_ENEMY,                                //AoE or Single Enemy

    SELECT_TARGET_SINGLE_FRIEND,                            //Only Single Friend
    SELECT_TARGET_AOE_FRIEND,                               //Only AoE Friend
    SELECT_TARGET_ANY_FRIEND,                               //AoE or Single Friend
};

//Spell Effects used by SelectSpell
enum SelectEffect
{
    SELECT_EFFECT_DONTCARE = 0,                             //All spell effects allowed
    SELECT_EFFECT_DAMAGE,                                   //Spell does damage
    SELECT_EFFECT_HEALING,                                  //Spell does healing
    SELECT_EFFECT_AURA,                                     //Spell applies an aura
};

enum SCEquip
{
    EQUIP_NO_CHANGE = -1,
    EQUIP_UNEQUIP   = 0
};

class TC_GAME_API CreatureAI : public UnitAI
{
    public:
        bool UpdateVictim(); //made public for tests
    protected:
        Creature *me;

        Creature* DoSummon(uint32 entry, Position const& pos, uint32 despawnTime = 30000, TempSummonType summonType = TEMPSUMMON_CORPSE_TIMED_DESPAWN);
        Creature* DoSummon(uint32 entry, WorldObject* obj, float radius = 5.0f, uint32 despawnTime = 30000, TempSummonType summonType = TEMPSUMMON_CORPSE_TIMED_DESPAWN);
        Creature* DoSummonFlyer(uint32 entry, WorldObject* obj, float flightZ, float radius = 5.0f, uint32 despawnTime = 30000, TempSummonType summonType = TEMPSUMMON_CORPSE_TIMED_DESPAWN);

    public:
        enum EvadeReason
        {
            EVADE_REASON_NO_HOSTILES,       // the creature's threat list is empty
            EVADE_REASON_BOUNDARY,          // the creature has moved outside its evade boundary
            EVADE_REASON_NO_PATH,           // the creature was unable to reach its target for over 5 seconds
            EVADE_REASON_SEQUENCE_BREAK,    // this is a boss and the pre-requisite encounters for engaging it are not defeated yet
            EVADE_REASON_OTHER,
        };

		explicit CreatureAI(Creature *c);

        ~CreatureAI() override = default;

        bool IsEngaged() const { return _isEngaged; }

        void Talk(uint8 id, WorldObject const* whisperTarget = nullptr);
        //Places the entire map into combat with creature
        void DoZoneInCombat(Creature* creature = nullptr);
        bool IsInMeleeRange() const;

		// Called if IsVisible(Unit* who) is true at each who move, reaction at visibility zone enter
		void MoveInLineOfSight_Safe(Unit* who);

        // Called for reaction at stopping attack at no attackers or targets
        virtual void EnterEvadeMode(EvadeReason why = EVADE_REASON_OTHER);

        // Called for reaction whenever we start being in combat (overridden from base UnitAI)
        void JustEnteredCombat(Unit* /*who*/) override;

        // Called for reaction whenever a new non-offline unit is added to the threat list
        virtual void JustStartedThreateningMe(Unit* who) { if (!IsEngaged()) EngagementStart(who); }

        // Called for reaction when initially engaged - this will always happen _after_ JustEnteredCombat
        virtual void JustEngagedWith(Unit* /*who*/) { }

        // Called when the creature is killed
        virtual void JustDied(Unit *) {}

        // Called when the creature kills a unit
        virtual void KilledUnit(Unit *) {}

        // Called when the creature summon successfully other creature
        virtual void JustSummoned(Creature* ) {}
        virtual void IsSummonedBy(WorldObject* /*summoner*/) { }

        virtual void SummonedCreatureDespawn(Creature* /*unit*/) {}
        virtual void SummonedCreatureDies(Creature* /*summon*/, Unit* /*killer*/) {}

        // Called when hit by a spell
        virtual void SpellHit(Unit* /*caster*/, SpellInfo const* /*spellInfo*/) { }
        virtual void SpellHitByGameObject(GameObject* /*caster*/, SpellInfo const* /*spellInfo*/) { }

        // Called when spell hits a target
        virtual void SpellHitTarget(Unit* /*target*/, SpellInfo const* /*spellInfo*/) { }
        virtual void SpellHitTargetGameObject(GameObject* /*target*/, SpellInfo const* /*spellInfo*/) { }

        virtual bool IsEscorted() const { return false; }

        // Called when vitim entered water and creature can not enter water
        virtual bool canReachByRangeAttack(Unit*) { return false; }

        // Called when creature appears in the world (spawn, respawn, grid load etc...)
        virtual void JustAppeared();

        // Called at waypoint reached or point movement finished
        virtual void MovementInform(uint32 /*MovementType*/, uint32 /*Data*/) {}
        
        // Called when creature finishes a spell cast
        virtual void OnSpellFinish(Unit *caster, uint32 spellId, Unit *target, bool ok) {}
        
        // Called when creature reaches its home position
        virtual void JustReachedHome() {}

        virtual void ReceiveEmote(Player* /*player*/, uint32 /*text_emote*/) {}

		// Called when owner takes damage
        virtual void OwnerAttackedBy(Unit* attacker) { OnOwnerCombatInteraction(attacker); }

		// Called when owner attacks something
        virtual void OwnerAttacked(Unit* target) { OnOwnerCombatInteraction(target); }


        virtual void DespawnDueToGameEventEnd(int32 /*eventId*/) {}

        // called when the corpse of this creature gets removed
        virtual void CorpseRemoved(uint32& respawnDelay) {}

        void OnCharmed(bool isNew) override;
        
        // Called when creature's master (pet case) killed a unit
        virtual void MasterKilledUnit(Unit* unit) {}
        
        //called for friendly creatures death FOR UP TO 60m
        virtual void FriendlyKilled(Creature const* c, float range) {}

        //remove me as soon as you can
        virtual bool sOnDummyEffect(Unit* /*caster*/, uint32 /*spellId*/, uint32 /*effIndex*/) { return false; }

        virtual void OnRemove() {}

        /* Script interaction */
        virtual uint64 message(uint32 id, uint64 data) { return 0; }

        // Should return true if the NPC is target of an escort quest
        // If onlyIfActive is set, should return true only if the escort quest is currently active
        virtual bool IsEscortNPC(bool /*onlyIfActive*/) const { return false; }


        /// == Gossip system ================================

        // Called when the dialog status between a player and the creature is requested.
        virtual Optional<QuestGiverStatus> GetDialogStatus(Player* /*player*/) { return boost::none; }

        // Called when a player opens a gossip dialog with the creature.
        virtual bool GossipHello(Player* /*player*/) { return false; }

        // Called when a player selects a gossip item in the creature's gossip menu.
        virtual bool GossipSelect(Player* /*player*/, uint32 /*menuId*/, uint32 /*gossipListId*/) { return false; }

        // Called when a player selects a gossip with a code in the creature's gossip menu.
        virtual bool GossipSelectCode(Player* /*player*/, uint32 /*menuId*/, uint32 /*gossipListId*/, char const* /*code*/) { return false; }

        // Called when a player accepts a quest from the creature.
        virtual void QuestAccept(Player* /*player*/, Quest const* /*quest*/) { }

        // Called when a player completes a quest and is rewarded, opt is the selected item's index or 0
        virtual void QuestReward(Player* /*player*/, Quest const* /*quest*/, uint32 /*opt*/) { }

        // (sun) Called when a target was hit in melee.
        // hitMask: ProcFlagsHit
        virtual void OnMeleeProcHit(Unit* target, uint32 hitMask) { }

        /// == Waypoints system =============================

        virtual void WaypointPathStarted(uint32 /*pathId*/) { }
        virtual void WaypointStarted(uint32 /*nodeId*/, uint32 /*pathId*/) { }
        virtual void WaypointReached(uint32 /*nodeId*/, uint32 /*pathId*/) { }
        virtual void WaypointPathEnded(uint32 /*nodeId*/, uint32 /*pathId*/) { }

        /// == Fields =======================================

		virtual void PassengerBoarded(Unit* /*passenger*/, int8 /*seatId*/, bool /*apply*/) { }

        virtual void OnSpellClick(Unit* /*clicker*/, bool /*spellClickHandled*/) { }

		virtual bool CanSeeAlways(WorldObject const* /*obj*/) { return false; }

        // Called when a player is charmed by the creature
        // If a PlayerAI* is returned, that AI is placed on the player instead of the default charm AI
        // Object destruction is handled by Unit::RemoveCharmedBy
        virtual PlayerAI* GetAIForCharmedPlayer(Player* /*who*/) { return nullptr; }

        // intended for encounter design/debugging. do not use for other purposes. expensive.
        int32 VisualizeBoundary(uint32 duration, Unit* owner = nullptr, bool fill = false) const;
        virtual bool CheckInRoom();
        CreatureBoundary const* GetBoundary() const { return _boundary; }
        void SetBoundary(CreatureBoundary const* boundary, bool negativeBoundaries = false);

        static bool IsInBounds(CreatureBoundary const& boundary, Position const* who);
        bool IsInBoundary(Position const* who = nullptr) const;

    protected:
        void EngagementStart(Unit* who);
        void EngagementOver();
		// Called at each *who move, AND if creature is aggressive
		virtual void MoveInLineOfSight(Unit *);

		//Same as MoveInLineOfSight but with is called with every react state (so not only if the creature is aggressive)
		virtual void MoveInLineOfSight2(Unit *) {}

        bool _EnterEvadeMode(EvadeReason why = EVADE_REASON_OTHER);  

        CreatureBoundary const* _boundary;
        bool _negateBoundary;

    private:
        void OnOwnerCombatInteraction(Unit* target);

        bool _isEngaged;
        bool _moveInLOSLocked;
};

enum Permitions
{
    PERMIT_BASE_NO                 = -1,
    PERMIT_BASE_IDLE               = 1,
    PERMIT_BASE_REACTIVE           = 100,
    PERMIT_BASE_PROACTIVE          = 200,
    PERMIT_BASE_FACTION_SPECIFIC   = 400,
    PERMIT_BASE_SPECIAL            = 800
};

#endif

