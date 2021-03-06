
#ifndef TRINITYCORE_GAMEOBJECT_H
#define TRINITYCORE_GAMEOBJECT_H

#include "Common.h"
#include "SharedDefines.h"
#include "Object.h"
#include "LootMgr.h"
#include "Database/DatabaseEnv.h"
#include "GameObjectData.h"
#include "GameObjectAI.h"
#include <G3D/Quat.h>
#include "Loot.h"
#include "WorldPacket.h"

class StaticTransport;
class MotionTransport;
class Group;

struct TransportAnimation;

union GameObjectValue
{
    //11 GAMEOBJECT_TYPE_TRANSPORT
    struct
    {
        //ever incrementing time
        uint32 PathProgress;
        TransportAnimation const* AnimationInfo;
        uint32 CurrentSeg;
    } Transport;

    //25 GAMEOBJECT_TYPE_FISHINGHOLE
    struct
    {
        uint32 MaxOpens;
    } FishingHole;
};

// For containers:  [GO_NOT_READY]->GO_READY (close)->GO_ACTIVATED (open) ->GO_JUST_DEACTIVATED->GO_READY        -> ...
// For bobber:      GO_NOT_READY  ->GO_READY (close)->GO_ACTIVATED (open) ->GO_JUST_DEACTIVATED-><deleted>
// For door(closed):[GO_NOT_READY]->GO_READY (close)->GO_ACTIVATED (open) ->GO_JUST_DEACTIVATED->GO_READY(close) -> ...
// For door(open):  [GO_NOT_READY]->GO_READY (open) ->GO_ACTIVATED (close)->GO_JUST_DEACTIVATED->GO_READY(open)  -> ...
enum LootState: uint32
{
    GO_NOT_READY = 0,
    GO_READY,                                               // can be ready but despawned, and then not possible activate until spawn
    GO_ACTIVATED,
    GO_JUST_DEACTIVATED
};

// from https://github.com/TrinityCore/TrinityCore/issues/21078
enum GameObjectActions : uint32
{                                   // Name from client executable      // Comments
    None,                           // -NONE-
    AnimateCustom0,                 // Animate Custom0
    AnimateCustom1,                 // Animate Custom1
    AnimateCustom2,                 // Animate Custom2
    AnimateCustom3,                 // Animate Custom3
    Disturb,                        // Disturb                          // Triggers trap
    Unlock,                         // Unlock                           // Resets GO_FLAG_LOCKED
    Lock,                           // Lock                             // Sets GO_FLAG_LOCKED
    Open,                           // Open                             // Sets GO_STATE_ACTIVE
    OpenAndUnlock,                  // Open + Unlock                    // Sets GO_STATE_ACTIVE and resets GO_FLAG_LOCKED
    Close,                          // Close                            // Sets GO_STATE_READY
    ToggleOpen,                     // Toggle Open
    Destroy,                        // Destroy                          // Sets GO_STATE_DESTROYED
    Rebuild,                        // Rebuild                          // Resets from GO_STATE_DESTROYED
    Creation,                       // Creation
    Despawn,                        // Despawn
    MakeInert,                      // Make Inert                       // Disables interactions
    MakeActive,                     // Make Active                      // Enables interactions
    CloseAndLock,                   // Close + Lock                     // Sets GO_STATE_READY and sets GO_FLAG_LOCKED
    UseArtKit0,                     // Use ArtKit0                      // 46904: 121
    UseArtKit1,                     // Use ArtKit1                      // 36639: 81, 46903: 122
    UseArtKit2,                     // Use ArtKit2
    UseArtKit3,                     // Use ArtKit3
    SetTapList,                     // Set Tap List
    GoTo1stFloor,                   // Go to 1st floor
    GoTo2ndFloor,                   // Go to 2nd floor
    GoTo3rdFloor,                   // Go to 3rd floor
    GoTo4thFloor,                   // Go to 4th floor
    GoTo5thFloor,                   // Go to 5th floor
    GoTo6thFloor,                   // Go to 6th floor
    GoTo7thFloor,                   // Go to 7th floor
    GoTo8thFloor,                   // Go to 8th floor
    GoTo9thFloor,                   // Go to 9th floor
    GoTo10thFloor,                  // Go to 10th floor
    UseArtKit4,                     // Use ArtKit4
    PlayAnimKit,                    // Play Anim Kit "%s"               // MiscValueB -> Anim Kit ID
    OpenAndPlayAnimKit,             // Open + Play Anim Kit "%s"        // MiscValueB -> Anim Kit ID
    CloseAndPlayAnimKit,            // Close + Play Anim Kit "%s"       // MiscValueB -> Anim Kit ID
    PlayOneshotAnimKit,             // Play One-shot Anim Kit "%s"      // MiscValueB -> Anim Kit ID
    StopAnimKit,                    // Stop Anim Kit
    OpenAndStopAnimKit,             // Open + Stop Anim Kit
    CloseAndStopAnimKit,            // Close + Stop Anim Kit
    PlaySpellVisual,                // Play Spell Visual "%s"           // MiscValueB -> Spell Visual ID
    StopSpellVisual,                // Stop Spell Visual
    SetTappedToChallengePlayers,    // Set Tapped to Challenge Players
};

class Unit;
class GameObjectModel;

// 5 sec for bobber catch
#define FISHING_BOBBER_READY_TIME 5
//time before chest are automatically despawned after first loot
#define CHEST_DESPAWN_TIME 300

class TC_GAME_API GameObject : public WorldObject, public GridObject<GameObject>, public MapObject
{
    public:
        explicit GameObject();
        ~GameObject() override;

        void BuildValuesUpdate(uint8 updatetype, ByteBuffer* data, Player* target) const override;

        void AddToWorld() override;
        void RemoveFromWorld() override;
		void CleanupsBeforeDelete(bool finalCleanup = true) override;

        virtual bool Create(ObjectGuid::LowType guidlow, uint32 name_id, Map *map, uint32 phaseMask, Position const& pos, QuaternionData const& rotation, uint32 animprogress, GOState go_state, uint32 ArtKit = 0, bool dynamic = false, uint32 spawnid = 0);
        void Update(uint32 diff) override;
        static GameObject* GetGameObject(WorldObject& object, ObjectGuid guid);
        GameObjectTemplate const* GetGOInfo() const;
        GameObjectData const* GetGameObjectData() const { return m_goData; }
        GameObjectValue const* GetGOValue() const { return &m_goValue; }

        bool IsTransport() const;

        void SetOwnerGUID(ObjectGuid owner)
        {
            m_spawnedByDefault = false;                     // all object with owner is despawned after delay
            SetGuidValue(OBJECT_FIELD_CREATED_BY, owner);
        }
        ObjectGuid GetOwnerGUID() const override { return GetGuidValue(OBJECT_FIELD_CREATED_BY); }

        uint32 GetSpawnId() const { return m_spawnId; }

        void SetTransportPathRotation(QuaternionData const& rot);

        // overwrite WorldObject function for proper name localization
        std::string const& GetNameForLocaleIdx(LocaleConstant locale_idx) const override;

        void SaveToDB();
        void SaveToDB(uint32 mapid, uint8 spawnMask);
        bool LoadFromDB(uint32 spawnId, Map* map, bool addToMap, bool = true); // arg4 is unused, only present to match the signature on Creature
        void DeleteFromDB();

        LootState getLootState() const { return m_lootState; }
        // Note: unit is only used when s = GO_ACTIVATED
        void SetLootState(LootState state, Unit* unit = nullptr);

        uint16 GetLootMode() const { return m_LootMode; }
        bool HasLootMode(uint16 lootMode) const { return (m_LootMode & lootMode) != 0; }
        void SetLootMode(uint16 lootMode) { m_LootMode = lootMode; }
        void AddLootMode(uint16 lootMode) { m_LootMode |= lootMode; }
        void RemoveLootMode(uint16 lootMode) { m_LootMode &= ~lootMode; }
        void ResetLootMode() { m_LootMode = LOOT_MODE_DEFAULT; }
        void SetLootGenerationTime();
        uint32 GetLootGenerationTime() const { return m_lootGenerationTime; }

        uint32 GetLockId() const
        {
            if (manual_unlock)
                return 0;

            switch(GetGoType())
            {
                case GAMEOBJECT_TYPE_DOOR:       return GetGOInfo()->door.lockId;
                case GAMEOBJECT_TYPE_BUTTON:     return GetGOInfo()->button.lockId;
                case GAMEOBJECT_TYPE_QUESTGIVER: return GetGOInfo()->questgiver.lockId;
                case GAMEOBJECT_TYPE_CHEST:      return GetGOInfo()->chest.lockId;
                case GAMEOBJECT_TYPE_TRAP:       return GetGOInfo()->trap.lockId;
                case GAMEOBJECT_TYPE_GOOBER:     return GetGOInfo()->goober.lockId;
                case GAMEOBJECT_TYPE_AREADAMAGE: return GetGOInfo()->areadamage.lockId;
                case GAMEOBJECT_TYPE_CAMERA:     return GetGOInfo()->camera.lockId;
                case GAMEOBJECT_TYPE_FLAGSTAND:  return GetGOInfo()->flagstand.lockId;
                case GAMEOBJECT_TYPE_FISHINGHOLE:return GetGOInfo()->fishinghole.lockId;
                case GAMEOBJECT_TYPE_FLAGDROP:   return GetGOInfo()->flagdrop.lockId;
                default: return 0;
            }
        }

        time_t GetRespawnTime() const { return m_respawnTime; }
        time_t GetRespawnTimeEx() const;
        uint32 GetDespawnDelay() const { return m_despawnDelay; }

        //Force despawn after specified time 
        void DespawnOrUnsummon(Milliseconds const& delay = 0ms, Seconds forceRespawnTime = 0s);
        void SetRespawnTime(int32 respawn);
        void Respawn();
        bool isSpawned() const
        {
            return m_respawnDelayTime == 0 ||
                (m_respawnTime > 0 && !m_spawnedByDefault) ||
                (m_respawnTime == 0 && m_spawnedByDefault);
        }
        bool isSpawnedByDefault() const { return m_spawnedByDefault; }
        void SetSpawnedByDefault(bool b) { m_spawnedByDefault = b; }
        uint32 GetRespawnDelay() const { return m_respawnDelayTime; }
        void Refresh();
        void Delete();
        void SetSpellId(uint32 id) { m_spellId = id;}
        uint32 GetSpellId() const { return m_spellId;}
        void getFishLoot(Loot* loot, Player* loot_owner);
#ifdef LICH_KING
        void getFishLootJunk(Loot* loot, Player* loot_owner);
#endif
        GameobjectTypes GetGoType() const { return GameobjectTypes(GetUInt32Value(GAMEOBJECT_TYPE_ID)); }
        void SetGoType(GameobjectTypes type) { SetUInt32Value(GAMEOBJECT_TYPE_ID, type); }
        GOState GetGoState() const { return GOState(GetUInt32Value(GAMEOBJECT_STATE)); }
        void SetGoState(GOState state);
        uint32 GetGoArtKit() const { return GetUInt32Value(GAMEOBJECT_ARTKIT); }
        void SetGoArtKit(uint32 artkit);
        uint32 GetGoAnimProgress() const { return GetUInt32Value(GAMEOBJECT_ANIMPROGRESS); }
        void SetGoAnimProgress(uint32 animprogress) { SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, animprogress); }
        
		void SetPhaseMask(uint32 newPhaseMask, bool update) override;

        void EnableCollision(bool enable);

        void Use(Unit* user);

        void AddToSkillupList(ObjectGuid::LowType PlayerGuidLow) { m_SkillupList.push_back(PlayerGuidLow); }
        bool IsInSkillupList(ObjectGuid::LowType PlayerGuidLow) const
        {
            for (ObjectGuid::LowType i : m_SkillupList)
                if (i == PlayerGuidLow) return true;
            return false;
        }
        void ClearSkillupList() { m_SkillupList.clear(); }

        void AddUniqueUse(Player* player);
        void AddUse();

        uint32 GetUseCount() const { return m_usetimes; }
        uint32 GetUniqueUseCount() const { return m_unique_users.size(); }

        void SaveRespawnTime(uint32 forceDelay = 0, bool savetodb = true) override;

        Loot loot;
        Player* GetLootRecipient() const;
        Group* GetLootRecipientGroup() const;
        void SetLootRecipient(Unit* unit, Group* group = nullptr);
        bool IsLootAllowedFor(Player const* player) const;
        bool HasLootRecipient() const { return !m_lootRecipient.IsEmpty() || m_lootRecipientGroup; }

        uint32 m_groupLootTimer;                            // (msecs)timer used for group loot
        ObjectGuid::LowType lootingGroupLowGUID;

        GameObject* GetLinkedTrap();
        void SetLinkedTrap(GameObject* linkedTrap) { m_linkedTrap = linkedTrap->GetGUID(); }

        bool HasQuest(uint32 quest_id) const override;
        bool HasInvolvedQuest(uint32 quest_id) const override;
        bool ActivateToQuest(Player *pTarget) const;
        //Open or activate gameobject (GO_STATE_ACTIVE). No effect if gameobject is already opened
        void UseDoorOrButton(uint32 time_to_restore = 0, bool alternative = false, Unit* user = nullptr);
        //Close or reset gameobject (GO_STATE_READY). No effect if gameobject is already closed
        void ResetDoorOrButton();

		bool IsNeverVisible() const override;

		bool IsAlwaysVisibleFor(WorldObject const* seer) const override;
		bool IsInvisibleDueToDespawn() const override;

        uint8 GetLevelForTarget(WorldObject const* target) const override;

        uint32 GetLinkedGameObjectEntry() const
        {
            switch(GetGoType())
            {
                case GAMEOBJECT_TYPE_CHEST:       return GetGOInfo()->chest.linkedTrapId;
                case GAMEOBJECT_TYPE_SPELL_FOCUS: return GetGOInfo()->spellFocus.linkedTrapId;
                case GAMEOBJECT_TYPE_GOOBER:      return GetGOInfo()->goober.linkedTrapId;
                default: return 0;
            }
        }

        uint32 GetAutoCloseTime() const;

        void TriggeringLinkedGameObject( uint32 trapEntry, Unit* target);

        GameObject* LookupFishingHoleAround(float range);

        void SendCustomAnim(uint32 anim);
        bool IsInRange(float x, float y, float z, float radius) const;
        
        void SwitchDoorOrButton(bool activate, bool alternative = false);
        
        GameObjectModel * m_model;
        void GetRespawnPosition(float &x, float &y, float &z, float* ori = nullptr) const;
        
        float GetInteractionDistance() const;

        inline bool IsStaticTransport() const { return GetGoType() == GAMEOBJECT_TYPE_TRANSPORT; }
        inline bool IsMotionTransport() const { return GetGoType() == GAMEOBJECT_TYPE_MO_TRANSPORT; }

        Transport* ToTransport() { if (IsMotionTransport() || IsStaticTransport()) return reinterpret_cast<Transport*>(this); else return nullptr; }
        Transport const* ToTransport() const { if (IsMotionTransport() || IsStaticTransport()) return reinterpret_cast<Transport const*>(this); else return nullptr; }

        StaticTransport* ToStaticTransport() { if (IsStaticTransport()) return reinterpret_cast<StaticTransport*>(this); else return nullptr; }
        StaticTransport const* ToStaticTransport() const { if (IsStaticTransport()) return reinterpret_cast<StaticTransport const*>(this); else return nullptr; }

        MotionTransport* ToMotionTransport() { if (IsMotionTransport()) return reinterpret_cast<MotionTransport*>(this); else return nullptr; }
        MotionTransport const* ToMotionTransport() const { if (IsMotionTransport()) return reinterpret_cast<MotionTransport const*>(this); else return nullptr; }

        void UpdateModelPosition();

        void EventInform(uint32 eventId, WorldObject* invoker = nullptr);

        // There's many places not ready for dynamic spawns. This allows them to live on for now.
        void SetRespawnCompatibilityMode(bool mode = true) { m_respawnCompatibilityMode = mode; }
        bool GetRespawnCompatibilityMode() const { return m_respawnCompatibilityMode; }

        uint32 GetScriptId() const;
        GameObjectAI* AI() const { return m_AI; }

        std::string GetAIName() const;
        void SetDisplayId(uint32 displayid);
        uint32 GetDisplayId() const { return GetUInt32Value(GAMEOBJECT_DISPLAYID); }
        
        uint32 GetFaction() const override { return GetUInt32Value(GAMEOBJECT_FACTION); }
        void SetFaction(uint32 faction) override { SetUInt32Value(GAMEOBJECT_FACTION, faction); }

        void setManualUnlocked() { manual_unlock = true; RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED); }
        void SetInactive(bool inactive) { m_inactive = inactive; }
        bool IsInactive() { return m_inactive; }

        float GetStationaryX() const override { if (GetGOInfo()->type != GAMEOBJECT_TYPE_MO_TRANSPORT) return m_stationaryPosition.GetPositionX(); return GetPositionX(); }
        float GetStationaryY() const override { if (GetGOInfo()->type != GAMEOBJECT_TYPE_MO_TRANSPORT) return m_stationaryPosition.GetPositionY(); return GetPositionY(); }
        float GetStationaryZ() const override { if (GetGOInfo()->type != GAMEOBJECT_TYPE_MO_TRANSPORT) return m_stationaryPosition.GetPositionZ(); return GetPositionZ(); }
        float GetStationaryO() const override { if (GetGOInfo()->type != GAMEOBJECT_TYPE_MO_TRANSPORT) return m_stationaryPosition.GetOrientation(); return GetOrientation(); }

        bool AIM_Initialize();
        void AIM_Destroy();

        std::string GetDebugInfo() const override;
    protected:
        uint32      m_charges;                              // Spell charges for GAMEOBJECT_TYPE_SPELLCASTER (22)
        uint32      m_spellId;
        time_t      m_respawnTime;                          // (secs) time of next respawn (or despawn if GO have owner()),
        uint32      m_respawnDelayTime;                     // (secs) if 0 then current GO state no dependent from timer
        uint32      m_despawnDelay;                         // ms despawn delay
        Seconds     m_despawnRespawnTime;                   // override respawn time after delayed despawn
        LootState   m_lootState;
        ObjectGuid  m_lootStateUnitGUID;                    // GUID of the unit passed with SetLootState(LootState, Unit*)
        bool        m_spawnedByDefault;
        time_t      m_cooldownTime;                         // used as internal reaction delay time store (not state change reaction).
                                                            // For traps this: spell casting cooldown, for doors/buttons: reset time.
        GOState     m_prevGoState;                          // What state to set whenever resetting
        bool        m_inactive;
        std::list<ObjectGuid::LowType> m_SkillupList;

        std::set<ObjectGuid::LowType> m_unique_users;
        uint32 m_usetimes;

        ObjectGuid::LowType m_spawnId;                               ///< For new or temporary gameobjects is 0 for saved it is lowguid
        GameObjectTemplate const* m_goInfo;
        GameObjectData const* m_goData;
        GameObjectValue m_goValue;
        bool manual_unlock;
        
        GameObjectModel* CreateModel();
        static bool CanHaveModel(GameobjectTypes);
        void UpdateModel();                                 // updates model in case displayId were changed

        Position m_stationaryPosition;

        ObjectGuid m_lootRecipient;
        uint32 m_lootRecipientGroup;
        uint16 m_LootMode;                                  // bitmask, default LOOT_MODE_DEFAULT, determines what loot will be lootable
        uint32 m_lootGenerationTime;

        ObjectGuid m_linkedTrap;
    private:
		void RemoveFromOwner();

        GameObjectAI* m_AI;
        bool m_respawnCompatibilityMode;
};
#endif

