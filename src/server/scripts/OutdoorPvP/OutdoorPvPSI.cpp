
#include "DBCStores.h"
#include "ScriptMgr.h"
#include "OutdoorPvPSI.h"
#include "WorldPacket.h"
#include "Player.h"
#include "GameObject.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Language.h"
#include "World.h"
#include "ReputationMgr.h"

OutdoorPvPSI::OutdoorPvPSI()
{
    m_TypeId = OUTDOOR_PVP_SI;
    m_Gathered_A = 0;
    m_Gathered_H = 0;
    m_LastController = 0;
}

void OutdoorPvPSI::FillInitialWorldStates(WorldPacket &data)
{
    data << SI_GATHERED_A << m_Gathered_A;
    data << SI_GATHERED_H << m_Gathered_H;
    data << SI_SILITHYST_MAX << SI_MAX_RESOURCES;
}

void OutdoorPvPSI::SendRemoveWorldStates(Player *plr)
{
    plr->SendUpdateWorldState(SI_GATHERED_A,WORLD_STATE_REMOVE);
    plr->SendUpdateWorldState(SI_GATHERED_H,WORLD_STATE_REMOVE);
    plr->SendUpdateWorldState(SI_SILITHYST_MAX,WORLD_STATE_REMOVE);
}

void OutdoorPvPSI::UpdateWorldState()
{
    SendUpdateWorldState(SI_GATHERED_A,m_Gathered_A);
    SendUpdateWorldState(SI_GATHERED_H,m_Gathered_H);
    SendUpdateWorldState(SI_SILITHYST_MAX,SI_MAX_RESOURCES);
}

bool OutdoorPvPSI::SetupOutdoorPvP()
{
    SetMapFromZone(OutdoorPvPSIBuffZones[0]);

    for(uint32 OutdoorPvPSIBuffZone : OutdoorPvPSIBuffZones)
        RegisterZone(OutdoorPvPSIBuffZone);
    return true;
}

bool OutdoorPvPSI::Update(uint32 diff)
{
    return false;
}

void OutdoorPvPSI::HandlePlayerEnterZone(Player * plr, uint32 zone)
{
    if(plr->GetTeam() == m_LastController)
        plr->CastSpell(plr,SI_CENARION_FAVOR, true);
    OutdoorPvP::HandlePlayerEnterZone(plr,zone);
}

void OutdoorPvPSI::HandlePlayerLeaveZone(Player * plr, uint32 zone)
{
    // remove buffs
    plr->RemoveAurasDueToSpell(SI_CENARION_FAVOR);
    OutdoorPvP::HandlePlayerLeaveZone(plr, zone);
}

void OutdoorPvPSI::BuffTeam(uint32 team)
{
    if(team == ALLIANCE)
    {
        for(ObjectGuid itr : m_players[0])
        {
            if(Player * plr = ObjectAccessor::FindPlayer(itr))
                if(plr->IsInWorld()) plr->CastSpell(plr,SI_CENARION_FAVOR, true);
        }
        for(ObjectGuid itr : m_players[1])
        {
            if(Player * plr = ObjectAccessor::FindPlayer(itr))
                if(plr->IsInWorld()) plr->RemoveAurasDueToSpell(SI_CENARION_FAVOR);
        }
    }
    else if(team == HORDE)
    {
        for(ObjectGuid itr : m_players[1])
        {
            if(Player * plr = ObjectAccessor::FindPlayer(itr))
                if(plr->IsInWorld()) plr->CastSpell(plr,SI_CENARION_FAVOR, true);
        }
        for(ObjectGuid itr : m_players[0])
        {
            if(Player * plr = ObjectAccessor::FindPlayer(itr))
                if(plr->IsInWorld()) plr->RemoveAurasDueToSpell(SI_CENARION_FAVOR);
        }
    }
    else
    {
        for(ObjectGuid itr : m_players[0])
        {
            if(Player * plr = ObjectAccessor::FindPlayer(itr))
                if(plr->IsInWorld()) plr->RemoveAurasDueToSpell(SI_CENARION_FAVOR);
        }
        for(ObjectGuid itr : m_players[1])
        {
            if(Player * plr = ObjectAccessor::FindPlayer(itr))
                if(plr->IsInWorld()) plr->RemoveAurasDueToSpell(SI_CENARION_FAVOR);
        }
    }
}

bool OutdoorPvPSI::HandleAreaTrigger(Player *plr, uint32 trigger)
{
    switch(trigger)
    {
    case SI_AREATRIGGER_A:
        if(plr->GetTeam() == ALLIANCE && plr->HasAuraEffect(SI_SILITHYST_FLAG,0))
        {
            // remove aura
            plr->RemoveAurasDueToSpell(SI_SILITHYST_FLAG);
            ++ m_Gathered_A;
            if(m_Gathered_A >= SI_MAX_RESOURCES)
            {
                BuffTeam(ALLIANCE);
                sWorld->SendZoneText(OutdoorPvPSIBuffZones[0],sObjectMgr->GetTrinityStringForDBCLocale(LANG_OPVP_SI_CAPTURE_A));
                m_LastController = ALLIANCE;
                m_Gathered_A = 0;
                m_Gathered_H = 0;
            }
            UpdateWorldState();
            // reward player
            plr->CastSpell(plr,SI_TRACES_OF_SILITHYST, true);
            // add 19 honor
            // plr->RewardHonor(NULL,1,19);     // Commented out to prevent bug abusing
            // add 20 cenarion circle repu
            plr->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(609), 20);
            // complete quest
            plr->KilledMonsterCredit(SI_TURNIN_QUEST_CM_A, ObjectGuid::Empty);
        }
        return true;
    case SI_AREATRIGGER_H:
        if(plr->GetTeam() == HORDE && plr->HasAuraEffect(SI_SILITHYST_FLAG,0))
        {
            // remove aura
            plr->RemoveAurasDueToSpell(SI_SILITHYST_FLAG);
            ++ m_Gathered_H;
            if(m_Gathered_H >= SI_MAX_RESOURCES)
            {
                BuffTeam(HORDE);
                sWorld->SendZoneText(OutdoorPvPSIBuffZones[0],sObjectMgr->GetTrinityStringForDBCLocale(LANG_OPVP_SI_CAPTURE_H));
                m_LastController = HORDE;
                m_Gathered_A = 0;
                m_Gathered_H = 0;
            }
            UpdateWorldState();
            // reward player
            plr->CastSpell(plr,SI_TRACES_OF_SILITHYST, true);
            // add 19 honor
            // plr->RewardHonor(NULL,1,19);     // Commented out to prevent bug abusing
            // add 20 cenarion circle repu
            plr->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(609), 20);
            // complete quest
            plr->KilledMonsterCredit(SI_TURNIN_QUEST_CM_H, ObjectGuid::Empty);
        }
        return true;
    }
    return false;
}

bool OutdoorPvPSI::HandleDropFlag(Player *plr, uint32 spellId)
{
    if(spellId == SI_SILITHYST_FLAG)
    {
        // if it was dropped away from the player's turn-in point, then create a silithyst mound, if it was dropped near the areatrigger, then it was dispelled by the outdoorpvp, so do nothing
        switch(plr->GetTeam())
        {
        case ALLIANCE:
            {
                AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(SI_AREATRIGGER_A);
                if(atEntry)
                {
                    // 5.0f is safe-distance
                    if(plr->GetDistance(atEntry->x,atEntry->y,atEntry->z) > 5.0f + atEntry->radius)
                    {
                        // he dropped it further, summon mound
                        auto  go = new GameObject;
                        Map * map = sMapMgr->CreateMap(plr->GetMapId(), plr);
                        if(!map){

                          delete go;
                          return true;
                          
                        }
                        
                        if(!go->Create(map->GenerateLowGuid<HighGuid::GameObject>(), SI_SILITHYST_MOUND, map, PHASEMASK_NORMAL, plr->GetPosition(), QuaternionData(), 255, GO_STATE_READY))
                        {
                            delete go;
                        }
                        else
                        {
                            go->SetRespawnTime(0);
                            map->AddToMap(go);
                        }
                    }
                }
            }
            break;
        case HORDE:
            {
                AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(SI_AREATRIGGER_H);
                if(atEntry)
                {
                    // 5.0f is safe-distance
                    if(plr->GetDistance(atEntry->x,atEntry->y,atEntry->z) > 5.0f + atEntry->radius)
                    {
                        // he dropped it further, summon mound
                        auto  go = new GameObject;
                        Map * map = sMapMgr->CreateMap(plr->GetMapId(), plr);
                        if(!map)
                        {
                          delete go;
                          return true;
                        }
                        if(!go->Create(map->GenerateLowGuid<HighGuid::GameObject>(), SI_SILITHYST_MOUND, map, PHASEMASK_NORMAL, plr->GetPosition(), QuaternionData(), 255, GO_STATE_READY))
                        {
                            delete go;
                        }
                        else
                        {
                            go->SetRespawnTime(0);
                            map->AddToMap(go);
                        }
                    }
                }
            }
            break;
        }
        return true;
    }
    return false;
}

bool OutdoorPvPSI::HandleCustomSpell(Player *plr, uint32 spellId, GameObject *go)
{
    if(!go || spellId != SI_SILITHYST_FLAG_GO_SPELL)
        return false;
    plr->CastSpell(plr,SI_SILITHYST_FLAG, true);
    if(go->GetGOInfo()->entry == SI_SILITHYST_MOUND)
    {
        // despawn go
        go->SetRespawnTime(0);
        go->Delete();
    }
    return true;
}

class OutdoorPvP_silithus : public OutdoorPvPScript
{
    public:
        OutdoorPvP_silithus() : OutdoorPvPScript("outdoorpvp_si") { }

        OutdoorPvP* GetOutdoorPvP() const override
        {
            return new OutdoorPvPSI();
        }
};

void AddSC_outdoorpvp_si()
{
    new OutdoorPvP_silithus();
}
