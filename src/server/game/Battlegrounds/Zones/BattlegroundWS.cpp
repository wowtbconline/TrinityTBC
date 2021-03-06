#include "Log.h"
#include "DBCStores.h"
#include "Object.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundWS.h"
#include "Creature.h"
#include "GameObject.h"
#include "Chat.h"
#include "MapManager.h"
#include "Language.h"
#include "World.h"

// these variables aren't used outside of this file, so declare them only here
enum BG_WSG_Rewards
{
    BG_WSG_WIN = 0,
    BG_WSG_FLAG_CAP,
    BG_WSG_MAP_COMPLETE,
    BG_WSG_REWARD_NUM
};

uint32 BG_WSG_Honor[BG_HONOR_MODE_NUM][BG_WSG_REWARD_NUM] = {
    {20,40,40}, // normal honor
    {60,40,80}  // holiday
};

uint32 BG_WSG_Reputation[BG_HONOR_MODE_NUM][BG_WSG_REWARD_NUM] = {
    {0,35,0}, // normal honor
    {0,45,0}  // holiday
};

BattlegroundWS::BattlegroundWS()
{
    BgObjects.resize(BG_WS_OBJECT_MAX);
    BgCreatures.resize(BG_CREATURES_MAX_WS);
}

BattlegroundWS::~BattlegroundWS()
{

}

void BattlegroundWS::Update(time_t diff)
{
    Battleground::Update(diff);

    // after bg start we get there (once)
    if (GetStatus() == STATUS_WAIT_JOIN && GetPlayersSize())
    {
        ModifyStartDelayTime(diff);

        if(!(m_Events & BG_STARTING_EVENT_1))
        {
            m_Events |= BG_STARTING_EVENT_1;

            // setup here, only when at least one player has ported to the map
            if(!SetupBattleground())
            {
                EndNow();
                return;
            }

            for(uint32 i = BG_WS_OBJECT_DOOR_A_1; i <= BG_WS_OBJECT_DOOR_H_4; i++)
            {
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
                DoorClose(i);
            }
            for(uint32 i = BG_WS_OBJECT_A_FLAG; i <= BG_WS_OBJECT_BERSERKBUFF_2; i++)
                SpawnBGObject(i, RESPAWN_ONE_DAY);

            SetStartDelayTime(START_DELAY0);
        }
        // After 1 minute, warning is signalled
        else if(GetStartDelayTime() <= START_DELAY1 && !(m_Events & BG_STARTING_EVENT_3))
        {
            m_Events |= BG_STARTING_EVENT_3;
            SendMessageToAll(GetTrinityString(LANG_BG_WS_ONE_MINUTE));
        }
        // After 1,5 minute, warning is signalled
        else if(GetStartDelayTime() <= START_DELAY2 && !(m_Events & BG_STARTING_EVENT_4))
        {
            m_Events |= BG_STARTING_EVENT_4;
            SendMessageToAll(GetTrinityString(LANG_BG_WS_HALF_MINUTE));
        }
        // After 2 minutes, gates OPEN !
        else if(GetStartDelayTime() < 0 && !(m_Events & BG_STARTING_EVENT_5))
        {
            m_Events |= BG_STARTING_EVENT_5;
            for(uint32 i = BG_WS_OBJECT_DOOR_A_1; i <= BG_WS_OBJECT_DOOR_A_4; i++)
                DoorOpen(i);
            for(uint32 i = BG_WS_OBJECT_DOOR_H_1; i <= BG_WS_OBJECT_DOOR_H_2; i++)
                DoorOpen(i);

            for(uint32 i = BG_WS_OBJECT_A_FLAG; i <= BG_WS_OBJECT_BERSERKBUFF_2; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);

            SendMessageToAll(GetTrinityString(LANG_BG_WS_BEGIN));

            PlaySoundToAll(SOUND_BG_START);
            SetStatus(STATUS_IN_PROGRESS);

            for(const auto & itr : GetPlayers())
                if(Player* plr = ObjectAccessor::FindPlayer(itr.first))
                    plr->RemoveAurasDueToSpell(SPELL_PREPARATION);
        }
    }
    else if(GetStatus() == STATUS_IN_PROGRESS)
    {
        if(m_FlagState[TEAM_ALLIANCE] == BG_WS_FLAG_STATE_WAIT_RESPAWN)
        {
            m_FlagsTimer[TEAM_ALLIANCE] -= diff;

            if(m_FlagsTimer[TEAM_ALLIANCE] < 0)
            {
                m_FlagsTimer[TEAM_ALLIANCE] = 0;
                RespawnFlag(ALLIANCE, true);
            }
        }
        if(m_FlagState[TEAM_ALLIANCE] == BG_WS_FLAG_STATE_ON_GROUND)
        {
            m_FlagsDropTimer[TEAM_ALLIANCE] -= diff;

            if(m_FlagsDropTimer[TEAM_ALLIANCE] < 0)
            {
                m_FlagsDropTimer[TEAM_ALLIANCE] = 0;
                RespawnFlagAfterDrop(ALLIANCE);
                m_BothFlagsKept = false;
            }
        }
        if(m_FlagState[TEAM_HORDE] == BG_WS_FLAG_STATE_WAIT_RESPAWN)
        {
            m_FlagsTimer[TEAM_HORDE] -= diff;

            if(m_FlagsTimer[TEAM_HORDE] < 0)
            {
                m_FlagsTimer[TEAM_HORDE] = 0;
                RespawnFlag(HORDE, true);
            }
        }
        if(m_FlagState[TEAM_HORDE] == BG_WS_FLAG_STATE_ON_GROUND)
        {
            m_FlagsDropTimer[TEAM_HORDE] -= diff;

            if(m_FlagsDropTimer[TEAM_HORDE] < 0)
            {
                m_FlagsDropTimer[TEAM_HORDE] = 0;
                RespawnFlagAfterDrop(HORDE);
                m_BothFlagsKept = false;
            }
        }
        if(m_BothFlagsKept)
        {
          m_FlagSpellForceTimer += diff;
          if(m_FlagDebuffState == 0 && m_FlagSpellForceTimer >= 300000)  //5 minutes (down from 10)
          {
            if(Player * plr = ObjectAccessor::FindPlayer(m_FlagKeepers[0]))
              plr->CastSpell(plr,WS_SPELL_FOCUSED_ASSAULT, true);
            if(Player * plr = ObjectAccessor::FindPlayer(m_FlagKeepers[1]))
              plr->CastSpell(plr,WS_SPELL_FOCUSED_ASSAULT, true);
            m_FlagDebuffState = 1;
          }
          else if(m_FlagDebuffState == 1 && m_FlagSpellForceTimer >= 600000) //10 minutes (down from 15)
          {
            if(Player * plr = ObjectAccessor::FindPlayer(m_FlagKeepers[0]))
            {
              plr->RemoveAurasDueToSpell(WS_SPELL_FOCUSED_ASSAULT);
              plr->CastSpell(plr,WS_SPELL_BRUTAL_ASSAULT, true);
            }
            if(Player * plr = ObjectAccessor::FindPlayer(m_FlagKeepers[1]))
            {
              plr->RemoveAurasDueToSpell(WS_SPELL_FOCUSED_ASSAULT);
              plr->CastSpell(plr,WS_SPELL_BRUTAL_ASSAULT, true);
            }
            m_FlagDebuffState = 2;
          }
        }
        else
        {
          m_FlagSpellForceTimer = 0; //reset timer.
          m_FlagDebuffState = 0;
        }
    }
}

void BattlegroundWS::AddPlayer(Player *plr)
{
    Battleground::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    auto  sc = new BattlegroundWGScore;

    PlayerScores[plr->GetGUID()] = sc;
}

void BattlegroundWS::RespawnFlag(uint32 Team, bool captured)
{
    if(Team == ALLIANCE)
        m_FlagState[TEAM_ALLIANCE] = BG_WS_FLAG_STATE_ON_BASE;
    else
        m_FlagState[TEAM_HORDE] = BG_WS_FLAG_STATE_ON_BASE;

    if(captured)
    {
        //when map_update will be allowed for battlegrounds this code will be useless
        SpawnBGObject(BG_WS_OBJECT_H_FLAG, RESPAWN_IMMEDIATELY);
        SpawnBGObject(BG_WS_OBJECT_A_FLAG, RESPAWN_IMMEDIATELY);
        SendMessageToAll(GetTrinityString(LANG_BG_WS_F_PLACED));
        PlaySoundToAll(BG_WS_SOUND_FLAGS_RESPAWNED);        // flag respawned sound...
    }
    m_BothFlagsKept = false;
}

void BattlegroundWS::RespawnFlagAfterDrop(uint32 team)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    RespawnFlag(team,false);
    if(team == ALLIANCE)
    {
        SpawnBGObject(BG_WS_OBJECT_A_FLAG, RESPAWN_IMMEDIATELY);
        SendMessageToAll(GetTrinityString(LANG_BG_WS_ALLIANCE_FLAG_RESPAWNED));
    }
    else
    {
        SpawnBGObject(BG_WS_OBJECT_H_FLAG, RESPAWN_IMMEDIATELY);
        SendMessageToAll(GetTrinityString(LANG_BG_WS_HORDE_FLAG_RESPAWNED));
    }

    PlaySoundToAll(BG_WS_SOUND_FLAGS_RESPAWNED);

    GameObject *obj = GetBgMap()->GetGameObject(GetDroppedFlagGUID(team));
    if(obj)
        obj->Delete();
    else
        TC_LOG_ERROR("bg.battleground","unknown droped flag bg, guid: %u",GetDroppedFlagGUID(team).GetCounter());

    SetDroppedFlagGUID(ObjectGuid::Empty, team);
    m_BothFlagsKept = false;
}

void BattlegroundWS::EventPlayerCapturedFlag(Player *Source, uint32 BgObjectType)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint32 winner = 0;

    //TODO FIX reputation and honor gains for low level players!

    Source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    if(Source->GetTeam() == ALLIANCE)
    {
        if (!this->IsHordeFlagPickedup())
            return;
        SetHordeFlagPicker(ObjectGuid::Empty);                              // must be before aura remove to prevent 2 events (drop+capture) at the same time
                                                            // horde flag in base (but not respawned yet)
        m_FlagState[TEAM_HORDE] = BG_WS_FLAG_STATE_WAIT_RESPAWN;
                                                            // Drop Horde Flag from Player
        Source->RemoveAurasDueToSpell(BG_WS_SPELL_WARSONG_FLAG);
        if(m_FlagDebuffState == 1)
          Source->RemoveAurasDueToSpell(WS_SPELL_FOCUSED_ASSAULT);
        if(m_FlagDebuffState == 2)
          Source->RemoveAurasDueToSpell(WS_SPELL_BRUTAL_ASSAULT);
        SendMessageToAll(LANG_BG_WS_CAPTURED_HF, CHAT_MSG_BG_SYSTEM_ALLIANCE, Source);

        if(GetTeamScore(ALLIANCE) < BG_WS_MAX_TEAM_SCORE)
            AddPoint(ALLIANCE, 1);
        PlaySoundToAll(BG_WS_SOUND_FLAG_CAPTURED_ALLIANCE);
        RewardReputationToTeam(890, BG_WSG_Reputation[m_HonorMode][BG_WSG_FLAG_CAP], ALLIANCE);          // +35 reputation
        RewardHonorToTeam(BG_WSG_Honor[m_HonorMode][BG_WSG_FLAG_CAP], ALLIANCE);                    // +40 bonushonor
    }
    else
    {
        if (!this->IsAllianceFlagPickedup())
            return;
        SetAllianceFlagPicker(ObjectGuid::Empty);                           // must be before aura remove to prevent 2 events (drop+capture) at the same time
                                                            // alliance flag in base (but not respawned yet)
        m_FlagState[TEAM_ALLIANCE] = BG_WS_FLAG_STATE_WAIT_RESPAWN;
                                                            // Drop Alliance Flag from Player
        Source->RemoveAurasDueToSpell(BG_WS_SPELL_SILVERWING_FLAG);
        if(m_FlagDebuffState == 1)
          Source->RemoveAurasDueToSpell(WS_SPELL_FOCUSED_ASSAULT);
        if(m_FlagDebuffState == 2)
          Source->RemoveAurasDueToSpell(WS_SPELL_BRUTAL_ASSAULT);
        SendMessageToAll(LANG_BG_WS_CAPTURED_AF, CHAT_MSG_BG_SYSTEM_HORDE, Source);

        if(GetTeamScore(HORDE) < BG_WS_MAX_TEAM_SCORE)
            AddPoint(HORDE, 1);
        PlaySoundToAll(BG_WS_SOUND_FLAG_CAPTURED_HORDE);
        RewardReputationToTeam(889, BG_WSG_Reputation[m_HonorMode][BG_WSG_FLAG_CAP], HORDE);             // +35 reputation
        RewardHonorToTeam(BG_WSG_Honor[m_HonorMode][BG_WSG_FLAG_CAP], HORDE);                       // +40 bonushonor
    }
    
    for(auto & m_Player : m_Players)
    {
        Player *plr = ObjectAccessor::FindPlayer(m_Player.first);

        if(!plr)
            continue;

        plr->RemoveAurasDueToSpell(WS_SPELL_FOCUSED_ASSAULT);
        plr->RemoveAurasDueToSpell(WS_SPELL_BRUTAL_ASSAULT);
    }

    SpawnBGObject(BG_WS_OBJECT_H_FLAG, BG_WS_FLAG_RESPAWN_TIME);
    SpawnBGObject(BG_WS_OBJECT_A_FLAG, BG_WS_FLAG_RESPAWN_TIME);

    UpdateFlagState(Source->GetTeam(), 1);                  // flag state none
    UpdateTeamScore(Source->GetTeam());
    // only flag capture should be updated
    UpdatePlayerScore(Source, SCORE_FLAG_CAPTURES, 1);      // +1 flag captures...

    if(GetTeamScore(ALLIANCE) == BG_WS_MAX_TEAM_SCORE)
        winner = ALLIANCE;

    if(GetTeamScore(HORDE) == BG_WS_MAX_TEAM_SCORE)
        winner = HORDE;

    if(winner)
    {
        UpdateWorldState(BG_WS_FLAG_UNK_ALLIANCE, 0);
        UpdateWorldState(BG_WS_FLAG_UNK_HORDE, 0);
        UpdateWorldState(BG_WS_FLAG_STATE_ALLIANCE, 1);
        UpdateWorldState(BG_WS_FLAG_STATE_HORDE, 1);

        RewardHonorToTeam(BG_WSG_Honor[m_HonorMode][BG_WSG_WIN], winner);
        RewardHonorToTeam(BG_WSG_Honor[m_HonorMode][BG_WSG_MAP_COMPLETE], winner);
        RewardHonorToTeam(BG_WSG_Honor[m_HonorMode][BG_WSG_MAP_COMPLETE], (winner == HORDE) ? ALLIANCE : HORDE);
        EndBattleground(winner);
    }
    else
    {
        m_FlagsTimer[GetTeamIndexByTeamId(Source->GetTeam()) == TEAM_ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE] = BG_WS_FLAG_RESPAWN_TIME;
    }
}

void BattlegroundWS::EventPlayerDroppedFlag(Player *Source)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
    {
        // if not running, do not cast things at the dropper player (prevent spawning the "dropped" flag), neither send unnecessary messages
        // just take off the aura
        if(Source->GetTeam() == ALLIANCE)
        {
            if(!this->IsHordeFlagPickedup())
                return;
            if(GetFlagPickerGUID(TEAM_HORDE) == Source->GetGUID())
            {
                SetHordeFlagPicker(ObjectGuid::Empty);
                Source->RemoveAurasDueToSpell(BG_WS_SPELL_WARSONG_FLAG);
            }
        }
        else
        {
            if(!this->IsAllianceFlagPickedup())
                return;
            if(GetFlagPickerGUID(TEAM_ALLIANCE) == Source->GetGUID())
            {
                SetAllianceFlagPicker(ObjectGuid::Empty);
                Source->RemoveAurasDueToSpell(BG_WS_SPELL_SILVERWING_FLAG);
            }
        }
        return;
    }

    bool set = false;

    if(Source->GetTeam() == ALLIANCE)
    {
        if(!this->IsHordeFlagPickedup())
            return;

        if(GetFlagPickerGUID(TEAM_HORDE) == Source->GetGUID())
        {
            SetHordeFlagPicker(ObjectGuid::Empty);
            Source->RemoveAurasDueToSpell(BG_WS_SPELL_WARSONG_FLAG);
            if(m_FlagDebuffState == 1)
              Source->RemoveAurasDueToSpell(WS_SPELL_FOCUSED_ASSAULT);
            if(m_FlagDebuffState == 2)
              Source->RemoveAurasDueToSpell(WS_SPELL_BRUTAL_ASSAULT);
            m_FlagState[TEAM_HORDE] = BG_WS_FLAG_STATE_ON_GROUND;
            SendMessageToAll(LANG_BG_WS_DROPPED_HF, CHAT_MSG_BG_SYSTEM_HORDE, Source);

            Source->CastSpell(Source, BG_WS_SPELL_WARSONG_FLAG_DROPPED, true);
            set = true;
        }
    }
    else
    {
        if(!this->IsAllianceFlagPickedup())
            return;

        if(GetFlagPickerGUID(TEAM_ALLIANCE) == Source->GetGUID())
        {
            SetAllianceFlagPicker(ObjectGuid::Empty);
            Source->RemoveAurasDueToSpell(BG_WS_SPELL_SILVERWING_FLAG);
            if(m_FlagDebuffState == 1)
              Source->RemoveAurasDueToSpell(WS_SPELL_FOCUSED_ASSAULT);
            if(m_FlagDebuffState == 2)
              Source->RemoveAurasDueToSpell(WS_SPELL_BRUTAL_ASSAULT);
            m_FlagState[TEAM_ALLIANCE] = BG_WS_FLAG_STATE_ON_GROUND;
            SendMessageToAll(LANG_BG_WS_DROPPED_AF, CHAT_MSG_BG_SYSTEM_ALLIANCE, Source);

            Source->CastSpell(Source, BG_WS_SPELL_SILVERWING_FLAG_DROPPED, true);
            set = true;
        }
    }

    if (set)
    {
        Source->CastSpell(Source, SPELL_RECENTLY_DROPPED_FLAG, true);
        UpdateFlagState(Source->GetTeam(), 1);

        if(Source->GetTeam() == ALLIANCE)
            UpdateWorldState(BG_WS_FLAG_UNK_HORDE, uint32(-1));
        else
            UpdateWorldState(BG_WS_FLAG_UNK_ALLIANCE, uint32(-1));

        m_FlagsDropTimer[GetTeamIndexByTeamId(Source->GetTeam()) == TEAM_ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE] = BG_WS_FLAG_DROP_TIME;
    }
}

void BattlegroundWS::EventPlayerClickedOnFlag(Player *Source, GameObject* target_obj)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    //alliance flag picked up from base
    if(Source->GetTeam() == HORDE && this->GetFlagState(ALLIANCE) == BG_WS_FLAG_STATE_ON_BASE
        && this->BgObjects[BG_WS_OBJECT_A_FLAG] == ObjectGuid(target_obj->GetGUID()))
    {
        SendMessageToAll(LANG_BG_WS_PICKEDUP_AF, CHAT_MSG_BG_SYSTEM_HORDE, Source);
        PlaySoundToAll(BG_WS_SOUND_ALLIANCE_FLAG_PICKED_UP);
        SpawnBGObject(BG_WS_OBJECT_A_FLAG, RESPAWN_ONE_DAY);
        SetAllianceFlagPicker(Source->GetGUID());
        m_FlagState[TEAM_ALLIANCE] = BG_WS_FLAG_STATE_ON_PLAYER;
        //update world state to show correct flag carrier
        UpdateFlagState(HORDE, BG_WS_FLAG_STATE_ON_PLAYER);
        UpdateWorldState(BG_WS_FLAG_UNK_ALLIANCE, 1);
        Source->CastSpell(Source, BG_WS_SPELL_SILVERWING_FLAG, TRIGGERED_FULL_MASK);
        if(m_FlagState[1] == BG_WS_FLAG_STATE_ON_PLAYER)
          m_BothFlagsKept = true;
    }

    //horde flag picked up from base
    if (Source->GetTeam() == ALLIANCE && this->GetFlagState(HORDE) == BG_WS_FLAG_STATE_ON_BASE
        && this->BgObjects[BG_WS_OBJECT_H_FLAG] == ObjectGuid(target_obj->GetGUID()))
    {
        SendMessageToAll(LANG_BG_WS_PICKEDUP_HF, CHAT_MSG_BG_SYSTEM_ALLIANCE, Source);
        PlaySoundToAll(BG_WS_SOUND_HORDE_FLAG_PICKED_UP);
        SpawnBGObject(BG_WS_OBJECT_H_FLAG, RESPAWN_ONE_DAY);
        SetHordeFlagPicker(Source->GetGUID());
        m_FlagState[TEAM_HORDE] = BG_WS_FLAG_STATE_ON_PLAYER;
        //update world state to show correct flag carrier
        UpdateFlagState(ALLIANCE, BG_WS_FLAG_STATE_ON_PLAYER);
        UpdateWorldState(BG_WS_FLAG_UNK_HORDE, 1);
        Source->CastSpell(Source, BG_WS_SPELL_WARSONG_FLAG, true);
        if(m_FlagState[0] == BG_WS_FLAG_STATE_ON_PLAYER)
          m_BothFlagsKept = true;
    }

    //Alliance flag on ground(not in base) (returned or picked up again from ground!)
    if(this->GetFlagState(ALLIANCE) == BG_WS_FLAG_STATE_ON_GROUND && Source->IsWithinDistInMap(target_obj, 10) && target_obj->GetGOInfo()->entry == BG_OBJECT_A_FLAG_GROUND_WS_ENTRY)
    {
        if(Source->GetTeam() == ALLIANCE)
        {
            SendMessageToAll(LANG_BG_WS_RETURNED_AF, CHAT_MSG_BG_SYSTEM_ALLIANCE, Source);
            UpdateFlagState(HORDE, BG_WS_FLAG_STATE_WAIT_RESPAWN);
            RespawnFlag(ALLIANCE, false);
            SpawnBGObject(BG_WS_OBJECT_A_FLAG, RESPAWN_IMMEDIATELY);
            PlaySoundToAll(BG_WS_SOUND_FLAG_RETURNED);
            UpdatePlayerScore(Source, SCORE_FLAG_RETURNS, 1);
            m_BothFlagsKept = false;
        }
        else
        {
            SendMessageToAll(LANG_BG_WS_PICKEDUP_AF, CHAT_MSG_BG_SYSTEM_HORDE, Source);
            PlaySoundToAll(BG_WS_SOUND_ALLIANCE_FLAG_PICKED_UP);
            SpawnBGObject(BG_WS_OBJECT_A_FLAG, RESPAWN_ONE_DAY);
            SetAllianceFlagPicker(Source->GetGUID());
            Source->CastSpell(Source, BG_WS_SPELL_SILVERWING_FLAG, true);
            m_FlagState[TEAM_ALLIANCE] = BG_WS_FLAG_STATE_ON_PLAYER;
            UpdateFlagState(HORDE, BG_WS_FLAG_STATE_ON_PLAYER);
            if(m_FlagDebuffState == 1)
              Source->CastSpell(Source,WS_SPELL_FOCUSED_ASSAULT, true);
            if(m_FlagDebuffState == 2)
              Source->CastSpell(Source,WS_SPELL_BRUTAL_ASSAULT, true);
            UpdateWorldState(BG_WS_FLAG_UNK_ALLIANCE, 1);
        }
        //called in HandleGameObjectUseOpcode:
        //target_obj->Delete();
    }

    //Horde flag on ground(not in base) (returned or picked up again)
    if(this->GetFlagState(HORDE) == BG_WS_FLAG_STATE_ON_GROUND && Source->IsWithinDistInMap(target_obj, 10) && target_obj->GetGOInfo()->entry == BG_OBJECT_H_FLAG_GROUND_WS_ENTRY)
    {
        if(Source->GetTeam() == HORDE)
        {
            SendMessageToAll(LANG_BG_WS_RETURNED_HF, CHAT_MSG_BG_SYSTEM_HORDE, Source);
            UpdateFlagState(ALLIANCE, BG_WS_FLAG_STATE_WAIT_RESPAWN);
            RespawnFlag(HORDE, false);
            SpawnBGObject(BG_WS_OBJECT_H_FLAG, RESPAWN_IMMEDIATELY);
            PlaySoundToAll(BG_WS_SOUND_FLAG_RETURNED);
            UpdatePlayerScore(Source, SCORE_FLAG_RETURNS, 1);
            m_BothFlagsKept = false;
        }
        else
        {
            SendMessageToAll(LANG_BG_WS_PICKEDUP_HF, CHAT_MSG_BG_SYSTEM_ALLIANCE, Source);
            PlaySoundToAll(BG_WS_SOUND_HORDE_FLAG_PICKED_UP);
            SpawnBGObject(BG_WS_OBJECT_H_FLAG, RESPAWN_ONE_DAY);
            SetHordeFlagPicker(Source->GetGUID());
            Source->CastSpell(Source, BG_WS_SPELL_WARSONG_FLAG, true);
            m_FlagState[TEAM_HORDE] = BG_WS_FLAG_STATE_ON_PLAYER;
            UpdateFlagState(ALLIANCE, BG_WS_FLAG_STATE_ON_PLAYER);
            if(m_FlagDebuffState == 1)
              Source->CastSpell(Source,WS_SPELL_FOCUSED_ASSAULT, true);
            if(m_FlagDebuffState == 2)
              Source->CastSpell(Source,WS_SPELL_BRUTAL_ASSAULT, true);
            UpdateWorldState(BG_WS_FLAG_UNK_HORDE, 1);
        }
        //called in HandleGameObjectUseOpcode:
        //target_obj->Delete();
    }

    Source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundWS::RemovePlayer(Player *plr, ObjectGuid guid)
{
    // sometimes flag aura not removed :(
    if(IsAllianceFlagPickedup() && m_FlagKeepers[TEAM_ALLIANCE] == guid)
    {
        if(!plr)
        {
            TC_LOG_ERROR("bg.battleground","BattlegroundWS: Removing offline player who has the FLAG!!");
            this->SetAllianceFlagPicker(ObjectGuid::Empty);
            this->RespawnFlag(ALLIANCE, false);
        }
        else
            this->EventPlayerDroppedFlag(plr);
    }
    if(IsHordeFlagPickedup() && m_FlagKeepers[TEAM_HORDE] == guid)
    {
        if(!plr)
        {
            TC_LOG_ERROR("bg.battleground","BattlegroundWS: Removing offline player who has the FLAG!!");
            this->SetHordeFlagPicker(ObjectGuid::Empty);
            this->RespawnFlag(HORDE, false);
        }
        else
            this->EventPlayerDroppedFlag(plr);
    }
}

void BattlegroundWS::UpdateFlagState(uint32 team, uint32 value)
{
    if(team == ALLIANCE)
        UpdateWorldState(BG_WS_FLAG_STATE_ALLIANCE, value);
    else
        UpdateWorldState(BG_WS_FLAG_STATE_HORDE, value);
}

void BattlegroundWS::UpdateTeamScore(uint32 team)
{
    if(team == ALLIANCE)
        UpdateWorldState(BG_WS_FLAG_CAPTURES_ALLIANCE, GetTeamScore(team));
    else
        UpdateWorldState(BG_WS_FLAG_CAPTURES_HORDE, GetTeamScore(team));
}

void BattlegroundWS::HandleAreaTrigger(Player *Source, uint32 Trigger)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    //uint32 SpellId = 0;
    //ObjectGuid buff_guid = 0;
    switch(Trigger)
    {
        case 3686:                                          // Alliance elixir of speed spawn. Trigger not working, because located inside other areatrigger, can be replaced by IsWithinDist(object, dist) in Battleground::Update().
            //buff_guid = BgObjects[BG_WS_OBJECT_SPEEDBUFF_1];
            break;
        case 3687:                                          // Horde elixir of speed spawn. Trigger not working, because located inside other areatrigger, can be replaced by IsWithinDist(object, dist) in Battleground::Update().
            //buff_guid = BgObjects[BG_WS_OBJECT_SPEEDBUFF_2];
            break;
        case 3706:                                          // Alliance elixir of regeneration spawn
            //buff_guid = BgObjects[BG_WS_OBJECT_REGENBUFF_1];
            break;
        case 3708:                                          // Horde elixir of regeneration spawn
            //buff_guid = BgObjects[BG_WS_OBJECT_REGENBUFF_2];
            break;
        case 3707:                                          // Alliance elixir of berserk spawn
            //buff_guid = BgObjects[BG_WS_OBJECT_BERSERKBUFF_1];
            break;
        case 3709:                                          // Horde elixir of berserk spawn
            //buff_guid = BgObjects[BG_WS_OBJECT_BERSERKBUFF_2];
            break;
        case 3646:                                          // Alliance Flag spawn
            if(m_FlagState[TEAM_HORDE] && !m_FlagState[TEAM_ALLIANCE])
                if(GetFlagPickerGUID(TEAM_HORDE) == Source->GetGUID())
                    EventPlayerCapturedFlag(Source, 0);
            break;
        case 3647:                                          // Horde Flag spawn
            if(m_FlagState[TEAM_ALLIANCE] && !m_FlagState[TEAM_HORDE])
                if(GetFlagPickerGUID(TEAM_ALLIANCE) == Source->GetGUID())
                    EventPlayerCapturedFlag(Source, 0);
            break;
        case 3649:                                          // unk1
        case 3688:                                          // unk2
        case 4628:                                          // unk3
        case 4629:                                          // unk4
            break;
        default:
            TC_LOG_ERROR("bg.battleground","WARNING: Unhandled AreaTrigger in Battleground: %u", Trigger);
            Source->GetSession()->SendAreaTriggerMessage("Warning: Unhandled AreaTrigger in Battleground: %u", Trigger);
            break;
    }

    //if(buff_guid)
    //    HandleTriggerBuff(buff_guid,Source);
}

bool BattlegroundWS::SetupBattleground()
{
    // flags
    if(    !AddObject(BG_WS_OBJECT_A_FLAG, BG_OBJECT_A_FLAG_WS_ENTRY, 1540.423f, 1481.325f, 351.8284f, 3.089233f, 0, 0, 0.9996573f, 0.02617699f, BG_WS_FLAG_RESPAWN_TIME/1000)
        || !AddObject(BG_WS_OBJECT_H_FLAG, BG_OBJECT_H_FLAG_WS_ENTRY, 916.0226f, 1434.405f, 345.413f, 0.01745329f, 0, 0, 0.008726535f, 0.9999619f, BG_WS_FLAG_RESPAWN_TIME/1000)
        // buffs
        || !AddObject(BG_WS_OBJECT_SPEEDBUFF_1, BG_OBJECTID_SPEEDBUFF_ENTRY, 1449.93f, 1470.71f, 342.6346f, -1.64061f, 0, 0, 0.7313537f, -0.6819983f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_SPEEDBUFF_2, BG_OBJECTID_SPEEDBUFF_ENTRY, 1005.171f, 1447.946f, 335.9032f, 1.64061f, 0, 0, 0.7313537f, 0.6819984f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_REGENBUFF_1, BG_OBJECTID_REGENBUFF_ENTRY, 1317.506f, 1550.851f, 313.2344f, -0.2617996f, 0, 0, 0.1305263f, -0.9914448f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_REGENBUFF_2, BG_OBJECTID_REGENBUFF_ENTRY, 1110.451f, 1353.656f, 316.5181f, -0.6806787f, 0, 0, 0.333807f, -0.9426414f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_BERSERKBUFF_1, BG_OBJECTID_BERSERKERBUFF_ENTRY, 1320.09f, 1378.79f, 314.7532f, 1.186824f, 0, 0, 0.5591929f, 0.8290376f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_BERSERKBUFF_2, BG_OBJECTID_BERSERKERBUFF_ENTRY, 1139.688f, 1560.288f, 306.8432f, -2.443461f, 0, 0, 0.9396926f, -0.3420201f, BUFF_RESPAWN_TIME)
        // alliance gates
        || !AddObject(BG_WS_OBJECT_DOOR_A_1, BG_OBJECT_DOOR_A_1_WS_ENTRY, 1503.335f, 1493.466f, 352.1888f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_A_2, BG_OBJECT_DOOR_A_2_WS_ENTRY, 1492.478f, 1457.912f, 342.9689f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_A_3, BG_OBJECT_DOOR_A_3_WS_ENTRY, 1468.503f, 1494.357f, 351.8618f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_A_4, BG_OBJECT_DOOR_A_4_WS_ENTRY, 1471.555f, 1458.778f, 362.6332f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, RESPAWN_IMMEDIATELY)
       // || !AddObject(BG_WS_OBJECT_DOOR_A_5, BG_OBJECT_DOOR_A_5_WS_ENTRY, 1492.347f, 1458.34f, 342.3712f, -0.03490669f, 0, 0, 0.01745246f, -0.9998477f, RESPAWN_IMMEDIATELY)
       // || !AddObject(BG_WS_OBJECT_DOOR_A_6, BG_OBJECT_DOOR_A_6_WS_ENTRY, 1503.466f, 1493.367f, 351.7352f, -0.03490669f, 0, 0, 0.01745246f, -0.9998477f, RESPAWN_IMMEDIATELY)
        // horde gates
        || !AddObject(BG_WS_OBJECT_DOOR_H_1, BG_OBJECT_DOOR_H_1_WS_ENTRY, 949.1663f, 1423.772f, 345.6241f, -0.5756807f, -0.01673368f, -0.004956111f, -0.2839723f, 0.9586737f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_H_2, BG_OBJECT_DOOR_H_2_WS_ENTRY, 953.0507f, 1459.842f, 340.6526f, -1.99662f, -0.1971825f, 0.1575096f, -0.8239487f, 0.5073641f, RESPAWN_IMMEDIATELY)
       // || !AddObject(BG_WS_OBJECT_DOOR_H_3, BG_OBJECT_DOOR_H_3_WS_ENTRY, 949.9523f, 1422.751f, 344.9273f, 0.0f, 0, 0, 0, 1, RESPAWN_IMMEDIATELY)
       // || !AddObject(BG_WS_OBJECT_DOOR_H_4, BG_OBJECT_DOOR_H_4_WS_ENTRY, 950.7952f, 1459.583f, 342.1523f, 0.05235988f, 0, 0, 0.02617695f, 0.9996573f, RESPAWN_IMMEDIATELY)
        )
    {
        TC_LOG_ERROR("battleground","BatteGroundWS: Failed to spawn some object Battleground not created!");
        return false;
    }

    WorldSafeLocsEntry const *sg = sWorldSafeLocsStore.LookupEntry(WS_GRAVEYARD_MAIN_ALLIANCE);
    if(!sg || !AddSpiritGuide(WS_SPIRIT_MAIN_ALLIANCE, sg->x, sg->y, sg->z, 3.124139f, ALLIANCE))
    {
        TC_LOG_ERROR("battleground","BatteGroundWS: Failed to spawn Alliance spirit guide! Battleground not created!");
        return false;
    }

    sg = sWorldSafeLocsStore.LookupEntry(WS_GRAVEYARD_MAIN_HORDE);
    if(!sg || !AddSpiritGuide(WS_SPIRIT_MAIN_HORDE, sg->x, sg->y, sg->z, 3.193953f, HORDE))
    {
        TC_LOG_ERROR("battleground","BatteGroundWS: Failed to spawn Horde spirit guide! Battleground not created!");
        return false;
    }

    return true;
}

void BattlegroundWS::Reset()
{
    Battleground::Reset();

    m_FlagKeepers[TEAM_ALLIANCE].Clear();
    m_FlagKeepers[TEAM_HORDE].Clear();
    m_DroppedFlagGUID[TEAM_ALLIANCE].Clear();
    m_DroppedFlagGUID[TEAM_HORDE].Clear();
    m_FlagState[TEAM_ALLIANCE]       = BG_WS_FLAG_STATE_ON_BASE;
    m_FlagState[TEAM_HORDE]          = BG_WS_FLAG_STATE_ON_BASE;
    m_TeamScores[TEAM_ALLIANCE]      = 0;
    m_TeamScores[TEAM_HORDE]         = 0;
    
    m_MaxLevel = 0;

    /* Spirit nodes is static at this BG and then not required deleting at BG reset.
    if(BgCreatures[WS_SPIRIT_MAIN_ALLIANCE])
        DelCreature(WS_SPIRIT_MAIN_ALLIANCE);

    if(BgCreatures[WS_SPIRIT_MAIN_HORDE])
        DelCreature(WS_SPIRIT_MAIN_HORDE);
    */
}

void BattlegroundWS::HandleKillPlayer(Player *player, Player *killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    EventPlayerDroppedFlag(player);

    Battleground::HandleKillPlayer(player, killer);
}

void BattlegroundWS::UpdatePlayerScore(Player *Source, uint32 type, uint32 value)
{

    auto itr = PlayerScores.find(Source->GetGUID());

    if(itr == PlayerScores.end())                         // player not found
        return;

    switch(type)
    {
        case SCORE_FLAG_CAPTURES:                           // flags captured
            ((BattlegroundWGScore*)itr->second)->FlagCaptures += value;
            break;
        case SCORE_FLAG_RETURNS:                            // flags returned
            ((BattlegroundWGScore*)itr->second)->FlagReturns += value;
            break;
        default:
            Battleground::UpdatePlayerScore(Source, type, value);
            break;
    }
}

void BattlegroundWS::FillInitialWorldStates(WorldPacket& data)
{
    data << uint32(BG_WS_FLAG_CAPTURES_ALLIANCE) << uint32(GetTeamScore(ALLIANCE));
    data << uint32(BG_WS_FLAG_CAPTURES_HORDE) << uint32(GetTeamScore(HORDE));

    if(m_FlagState[TEAM_ALLIANCE] == BG_WS_FLAG_STATE_ON_GROUND)
        data << uint32(BG_WS_FLAG_UNK_ALLIANCE) << uint32(-1);
    else if (m_FlagState[TEAM_ALLIANCE] == BG_WS_FLAG_STATE_ON_PLAYER)
        data << uint32(BG_WS_FLAG_UNK_ALLIANCE) << uint32(1);
    else
        data << uint32(BG_WS_FLAG_UNK_ALLIANCE) << uint32(0);

    if(m_FlagState[TEAM_HORDE] == BG_WS_FLAG_STATE_ON_GROUND)
        data << uint32(BG_WS_FLAG_UNK_HORDE) << uint32(-1);
    else if (m_FlagState[TEAM_HORDE] == BG_WS_FLAG_STATE_ON_PLAYER)
        data << uint32(BG_WS_FLAG_UNK_HORDE) << uint32(1);
    else
        data << uint32(BG_WS_FLAG_UNK_HORDE) << uint32(0);

    data << uint32(BG_WS_FLAG_CAPTURES_MAX) << uint32(BG_WS_MAX_TEAM_SCORE);

    if (m_FlagState[TEAM_HORDE] == BG_WS_FLAG_STATE_ON_PLAYER)
        data << uint32(BG_WS_FLAG_STATE_ALLIANCE) << uint32(2);
    else
        data << uint32(BG_WS_FLAG_STATE_ALLIANCE) << uint32(1);

    if (m_FlagState[TEAM_ALLIANCE] == BG_WS_FLAG_STATE_ON_PLAYER)
        data << uint32(BG_WS_FLAG_STATE_HORDE) << uint32(2);
    else
        data << uint32(BG_WS_FLAG_STATE_HORDE) << uint32(1);

}

// Hack to respawn players at graveyard when they die in starting area: that building is a WorldSafeLoc, and is considered as Closest Graveyard by default.
WorldSafeLocsEntry const *BattlegroundWS::GetClosestGraveYard(float x, float y, float z, uint32 team)
{
    WorldSafeLocsEntry const* entry = nullptr;
    if (GetStatus() == STATUS_IN_PROGRESS) {
        uint32 entryId = (team == HORDE) ? 772 : 771;
        entry = sWorldSafeLocsStore.LookupEntry(entryId);
    }
    else
        entry = sObjectMgr->GetClosestGraveYard(x, y, z, GetMapId(), team);
    
    if (!entry) {
        TC_LOG_ERROR("bg.battleground","BattlegroundWS: Not found the team graveyard. Graveyard system isn't working!");
        return nullptr;
    }
    
    return entry;
}
