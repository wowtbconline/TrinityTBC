/*
 * Copyright (C) 2005-2008 MaNGOS <http://www.mangosproject.org/>
 *
 * Copyright (C) 2008 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "Log.h"
#include "PlayerDump.h"
#include "Database/DatabaseEnv.h"
#include "UpdateFields.h"
#include "ObjectMgr.h"

// Character Dump tables
#define DUMP_TABLE_COUNT 21

struct DumpTable
{
    char const* name;
    DumpTableType type;
};

static DumpTable dumpTables[DUMP_TABLE_COUNT] =
{
    { "characters",                     DTT_CHARACTER  },
    { "character_queststatus",          DTT_CHAR_TABLE },
    { "character_queststatus_daily",    DTT_CHAR_TABLE },
    { "character_queststatus_rewarded", DTT_CHAR_TABLE },
    { "character_reputation",           DTT_CHAR_TABLE },
    { "character_spell",                DTT_CHAR_TABLE },
    { "character_spell_cooldown",       DTT_CHAR_TABLE },
    { "character_action",               DTT_CHAR_TABLE },
    { "character_aura",                 DTT_CHAR_TABLE },
    { "character_homebind",             DTT_CHAR_TABLE },
    { "character_skills",               DTT_CHAR_TABLE },
    { "character_inventory",            DTT_INVENTORY  },
    { "mail",                           DTT_MAIL       },
    { "mail_items",                     DTT_MAIL_ITEM  },
    { "item_instance",                  DTT_ITEM       },
    { "character_gifts",                DTT_ITEM_GIFT  },
    { "item_text",                      DTT_ITEM_TEXT  },
    { "character_pet",                  DTT_PET        },
    { "pet_aura",                       DTT_PET_TABLE  },
    { "pet_spell",                      DTT_PET_TABLE  },
    { "pet_spell_cooldown",             DTT_PET_TABLE  },
};

// Low level functions
static bool findtoknth(std::string &str, int n, std::string::size_type &s, std::string::size_type &e)
{
    int i; s = e = 0;
    std::string::size_type size = str.size();
    for(i = 1; s < size && i < n; s++) if(str[s] == ' ') ++i;
    if (i < n)
        return false;

    e = str.find(' ', s);

    return e != std::string::npos;
}

std::string gettoknth(std::string &str, int n)
{
    std::string::size_type s = 0, e = 0;
    if(!findtoknth(str, n, s, e))
        return "";

    return str.substr(s, e-s);
}

bool findnth(std::string &str, int n, std::string::size_type &s, std::string::size_type &e)
{
    s = str.find("VALUES ('")+9;
    if (s == std::string::npos) return false;

    do
    {
        e = str.find("'",s);
        if (e == std::string::npos) return false;
    } while(str[e-1] == '\\');

    for(int i = 1; i < n; i++)
    {
        do
        {
            s = e+4;
            e = str.find("'",s);
            if (e == std::string::npos) return false;
        } while (str[e-1] == '\\');
    }
    return true;
}

std::string gettablename(std::string &str)
{
    std::string::size_type s = 13;
    std::string::size_type e = str.find("`", s);
    if (e == std::string::npos)
        return "";

    return str.substr(s, e-s);
}

bool changenth(std::string &str, int n, const char *with, bool insert = false, bool nonzero = false)
{
    std::string::size_type s, e;
    if(!findnth(str,n,s,e))
        return false;

    if(nonzero && str.substr(s,e-s) == "0")
        return true;                                        // not an error
    if(!insert)
        str.replace(s,e-s, with);
    else
        str.insert(s, with);

    return true;
}

std::string getnth(std::string &str, int n)
{
    std::string::size_type s, e;
    if(!findnth(str,n,s,e))
        return "";

    return str.substr(s, e-s);
}

bool changetoknth(std::string &str, int n, const char *with, bool insert = false, bool nonzero = false)
{
    std::string::size_type s = 0, e = 0;
    if(!findtoknth(str, n, s, e))
        return false;
    if(nonzero && str.substr(s,e-s) == "0")
        return true;                                        // not an error
    if(!insert)
        str.replace(s, e-s, with);
    else
        str.insert(s, with);

    return true;
}

uint32 registerNewGuid(uint32 oldGuid, std::map<uint32, uint32> &guidMap, uint32 hiGuid)
{
    auto itr = guidMap.find(oldGuid);
    if(itr != guidMap.end())
        return itr->second;

    uint32 newguid = hiGuid + guidMap.size();
    guidMap[oldGuid] = newguid;
    return newguid;
}

bool changeGuid(std::string &str, int n, std::map<uint32, uint32> &guidMap, uint32 hiGuid, bool nonzero = false)
{
    char chritem[20];
    uint32 oldGuid = atoi(getnth(str, n).c_str());
    if (nonzero && oldGuid == 0)
        return true;                                        // not an error

    uint32 newGuid = registerNewGuid(oldGuid, guidMap, hiGuid);
    snprintf(chritem, 20, "%d", newGuid);

    return changenth(str, n, chritem, false, nonzero);
}

bool changetokGuid(std::string &str, int n, std::map<uint32, uint32> &guidMap, uint32 hiGuid, bool nonzero = false)
{
    char chritem[20];
    uint32 oldGuid = atoi(gettoknth(str, n).c_str());
    if (nonzero && oldGuid == 0)
        return true;                                        // not an error

    uint32 newGuid = registerNewGuid(oldGuid, guidMap, hiGuid);
    snprintf(chritem, 20, "%d", newGuid);

    return changetoknth(str, n, chritem, false, nonzero);
}

std::string CreateDumpString(char const* tableName, QueryResult result)
{
    if(!tableName || !result) return "";
    std::ostringstream ss;
    ss << "INSERT INTO `"<< tableName << "` VALUES (";
    Field *fields = result->Fetch();
    for(uint32 i = 0; i < result->GetFieldCount(); i++)
    {
        if (i == 0) ss << "'";
        else ss << ", '";

        std::string s = fields[i].GetString();
        CharacterDatabase.EscapeString(s);
        ss << s;

        ss << "'";
    }
    ss << ");";
    return ss.str();
}

std::string PlayerDumpWriter::GenerateWhereStr(char const* field, uint32 guid)
{
    std::ostringstream wherestr;
    wherestr << field << " = '" << guid << "'";
    return wherestr.str();
}

std::string PlayerDumpWriter::GenerateWhereStr(char const* field, GUIDs const& guids, GUIDs::const_iterator& itr)
{
    std::ostringstream wherestr;
    wherestr << field << " IN ('";
    for(; itr != guids.end(); ++itr)
    {
        wherestr << *itr;

        if(wherestr.str().size() > MAX_QUERY_LEN - 50)      // near to max query
        {
            ++itr;
            break;
        }

        auto itr2 = itr;
        if(++itr2 != guids.end())
            wherestr << "','";
    }
    wherestr << "')";
    return wherestr.str();
}

void StoreGUID(QueryResult result,uint32 field,std::set<uint32>& guids)
{
    Field* fields = result->Fetch();
    uint32 guid = fields[field].GetUInt32();
    if(guid)
        guids.insert(guid);
}

void StoreGUID(QueryResult result,uint32 data,uint32 field, std::set<uint32>& guids)
{
    Field* fields = result->Fetch();
    std::string dataStr = fields[data].GetString();
    uint32 guid = atoi(gettoknth(dataStr, field).c_str());
    if(guid)
        guids.insert(guid);
}

// Writing - High-level functions
void PlayerDumpWriter::DumpTable(std::string& dump, uint32 guid, char const*tableFrom, char const*tableTo, DumpTableType type)
{
    GUIDs const* guids = nullptr;
    char const* fieldname = nullptr;

    switch ( type )
    {
        case DTT_ITEM:      fieldname = "guid";      guids = &items; break;
        case DTT_ITEM_GIFT: fieldname = "item_guid"; guids = &items; break;
        case DTT_PET:       fieldname = "owner";                     break;
        case DTT_PET_TABLE: fieldname = "guid";      guids = &pets;  break;
        case DTT_MAIL:      fieldname = "receiver";                  break;
        case DTT_MAIL_ITEM: fieldname = "mail_id";   guids = &mails; break;
        case DTT_ITEM_TEXT: fieldname = "id";        guids = &texts; break;
        default:            fieldname = "guid";                      break;
    }

    // for guid set stop if set is empty
    if(guids && guids->empty())
        return;                                             // nothing to do

    // setup for guids case start position
    GUIDs::const_iterator guids_itr;
    if(guids)
        guids_itr = guids->begin();

    do
    {
        std::string wherestr;

        if(guids)                                           // set case, get next guids string
            wherestr = GenerateWhereStr(fieldname,*guids,guids_itr);
        else                                                // not set case, get single guid string
            wherestr = GenerateWhereStr(fieldname,guid);

        QueryResult result = CharacterDatabase.PQuery("SELECT * FROM %s WHERE %s", tableFrom, wherestr.c_str());
        if(!result)
            return;

        do
        {
            // collect guids
            switch ( type )
            {
            case DTT_INVENTORY:
                StoreGUID(result,3,items); break;           // item guid collection
            case DTT_ITEM:
                StoreGUID(result,0,ITEM_FIELD_ITEM_TEXT_ID,texts); break;
                // item text id collection
            case DTT_PET:
                StoreGUID(result,0,pets);  break;           // pet guid collection
            case DTT_MAIL:
                StoreGUID(result,0,mails);                  // mail id collection
                StoreGUID(result,6,texts); break;           // item text id collection
            case DTT_MAIL_ITEM:
                StoreGUID(result,1,items); break;           // item guid collection
            default:                       break;
            }

            dump += CreateDumpString(tableTo, result);
            dump += "\n";
        }
        while (result->NextRow());
    }
    while(guids && guids_itr != guids->end());              // not set case iterate single time, set case iterate for all guids
}

std::string PlayerDumpWriter::GetDump(uint32 guid)
{
    std::string dump;
    for(auto & dumpTable : dumpTables)
        DumpTable(dump, guid, dumpTable.name, dumpTable.name, dumpTable.type);

    // TODO: Add instance/group..
    // TODO: Add a dump level option to skip some non-important tables

    return dump;
}

DumpReturn PlayerDumpWriter::WriteDump(const std::string& file, uint32 guid)
{
    FILE *fout = fopen(file.c_str(), "w");
    if (!fout)
        return DUMP_FILE_OPEN_ERROR;

    std::string dump = GetDump(guid);

    fprintf(fout,"%s\n",dump.c_str());
    fclose(fout);
    return DUMP_SUCCESS;
}

// Reading - High-level functions
#define ROLLBACK(DR) {fclose(fin); return (DR);}

//item loading is broken for now
DumpReturn PlayerDumpReader::LoadDump(const std::string& file, uint32 account, std::string name, uint32 guid)
{
    // check character count
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(guid) FROM characters WHERE account = '%d'", account);
        uint8 charcount = 0;
        if ( result )
        {
            Field *fields=result->Fetch();
            charcount = fields[0].GetUInt8();

            if (charcount >= 10)
                return DUMP_TOO_MANY_CHARS;
        }
    }

    FILE *fin = fopen(file.c_str(), "r");
    if(!fin)
        return DUMP_FILE_OPEN_ERROR;

    QueryResult  result = nullptr;
    char newguid[20], chraccount[20], newpetid[20], currpetid[20], lastpetid[20];

    // make sure the same guid doesn't already exist and is safe to use
    bool incHighest = true;
    if (guid != 0 && guid < sObjectMgr->GetGenerator<HighGuid::Player>().GetNextAfterMaxUsed())
    {
        result = CharacterDatabase.PQuery("SELECT * FROM characters WHERE guid = '%d'", guid);
        if (result)
        {
            guid = sObjectMgr->GetGenerator<HighGuid::Player>().GetNextAfterMaxUsed();                     // use first free if exists
        }
        else incHighest = false;
    }
    else
        guid = sObjectMgr->GetGenerator<HighGuid::Player>().GetNextAfterMaxUsed();

    // normalize the name if specified and check if it exists
    if(!normalizePlayerName(name))
        name = "";

    if(ObjectMgr::CheckPlayerName(name,true))
    {
        CharacterDatabase.EscapeString(name);              // for safe, we use name only for sql quearies anyway
        result = CharacterDatabase.PQuery("SELECT * FROM characters WHERE name = '%s'", name.c_str());
        if (result)
        {
            name = "";                                      // use the one from the dump
        }
    }
    else name = "";

    // name encoded or empty

    snprintf(newguid, 20, "%d", guid);
    snprintf(chraccount, 20, "%d", account);
    snprintf(newpetid, 20, "%d", sObjectMgr->GeneratePetNumber());
    snprintf(lastpetid, 20, "%s", "");

    std::map<uint32,uint32> items;
    std::map<uint32,uint32> mails;
    char buf[32000] = "";

    typedef std::map<uint32, uint32> PetIds;                // old->new petid relation
    typedef PetIds::value_type PetIdsPair;
    PetIds petids;

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    while(!feof(fin))
    {
        if(!fgets(buf, 32000, fin))
        {
            if(feof(fin)) break;
            ROLLBACK(DUMP_FILE_BROKEN);
        }

        std::string line; line.assign(buf);

        // skip empty strings
        if(line.find_first_not_of(" \t\n\r\7")==std::string::npos)
            continue;

        // determine table name and load type
        std::string tn = gettablename(line);
        if(tn.empty())
        {
            TC_LOG_ERROR("FIXME","LoadPlayerDump: Can't extract table name from line: '%s'!", line.c_str());
            ROLLBACK(DUMP_FILE_BROKEN);
        }

        DumpTableType type = DTT_CHARACTER;
        uint8 i;
        for(i = 0; i < DUMP_TABLE_COUNT; i++)
        {
            if (tn == dumpTables[i].name)
            {
                type = dumpTables[i].type;
                break;
            }
        }

        if (i == DUMP_TABLE_COUNT)
        {
            TC_LOG_ERROR("misc","LoadPlayerDump: Unknown table: '%s'!", tn.c_str());
            ROLLBACK(DUMP_FILE_BROKEN);
        }

        // change the data to server values
        switch(type)
        {
            case DTT_CHAR_TABLE:
                if(!changenth(line, 1, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                break;

            case DTT_CHARACTER:                             // character t.
            {
                if(!changenth(line, 1, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);

                // guid, data field:guid, items
                if(!changenth(line, 2, chraccount))
                    ROLLBACK(DUMP_FILE_BROKEN);
                std::string vals = getnth(line, 3);
                if(!changetoknth(vals, OBJECT_FIELD_GUID+1, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                for(uint16 field = PLAYER_FIELD_INV_SLOT_HEAD; field < PLAYER_FARSIGHT; field++)
                    if(!changetokGuid(vals, field+1, items, sObjectMgr->GetGenerator<HighGuid::Item>().GetNextAfterMaxUsed(), true))
                        ROLLBACK(DUMP_FILE_BROKEN);
                if(!changenth(line, 3, vals.c_str()))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if (name == "")
                {
                    // check if the original name already exists
                    name = getnth(line, 4);
                    CharacterDatabase.EscapeString(name);

                    result = CharacterDatabase.PQuery("SELECT * FROM characters WHERE name = '%s'", name.c_str());
                    if (result)
                    {
                        // rename on login: `at_login` field 30 in raw field list
                        if(!changenth(line, 30, "1"))
                            ROLLBACK(DUMP_FILE_BROKEN);
                    }
                }
                else if(!changenth(line, 4, name.c_str()))
                    ROLLBACK(DUMP_FILE_BROKEN);

                break;
            }
            case DTT_INVENTORY:                             // character_inventory t.
            {
                if(!changenth(line, 1, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);

                // bag, item
                if(!changeGuid(line, 2, items, sObjectMgr->GetGenerator<HighGuid::Item>().GetNextAfterMaxUsed() , true))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changeGuid(line, 4, items, sObjectMgr->GetGenerator<HighGuid::Item>().GetNextAfterMaxUsed()))
                    ROLLBACK(DUMP_FILE_BROKEN);
                break;
            }
            case DTT_ITEM:                                  // item_instance t.
            {
                // item, owner, data field:item, owner guid
                if(!changeGuid(line, 1, items, sObjectMgr->GetGenerator<HighGuid::Item>().GetNextAfterMaxUsed()))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changenth(line, 2, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                std::string vals = getnth(line,3);
                if(!changetokGuid(vals, OBJECT_FIELD_GUID+1, items, sObjectMgr->GetGenerator<HighGuid::Item>().GetNextAfterMaxUsed()))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changetoknth(vals, ITEM_FIELD_OWNER+1, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changenth(line, 3, vals.c_str()))
                    ROLLBACK(DUMP_FILE_BROKEN);
                break;
            }
            case DTT_ITEM_GIFT:                             // character_gift
            {
                // guid,item_guid,
                if(!changenth(line, 1, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changeGuid(line, 2, items, sObjectMgr->GetGenerator<HighGuid::Item>().GetNextAfterMaxUsed()))
                    ROLLBACK(DUMP_FILE_BROKEN);
                break;
            }
            case DTT_PET:                                   // character_pet t
            {
                //store a map of old pet id to new inserted pet id for use by type 5 tables
                snprintf(currpetid, 20, "%s", getnth(line, 1).c_str());
                if(strlen(lastpetid)==0) snprintf(lastpetid, 20, "%s", currpetid);
                if(strcmp(lastpetid,currpetid)!=0)
                {
                    snprintf(newpetid, 20, "%d", sObjectMgr->GeneratePetNumber());
                    snprintf(lastpetid, 20, "%s", currpetid);
                }

                std::map<uint32, uint32> :: const_iterator petids_iter = petids.find(atoi(currpetid));

                if(petids_iter == petids.end())
                {
                    petids.insert(PetIdsPair(atoi(currpetid), atoi(newpetid)));
                }

                // item, entry, owner, ...
                if(!changenth(line, 1, newpetid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changenth(line, 3, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);

                break;
            }
            case DTT_PET_TABLE:                             // pet_aura, pet_spell, pet_spell_cooldown t
            {
                snprintf(currpetid, 20, "%s", getnth(line, 1).c_str());

                // lookup currpetid and match to new inserted pet id
                std::map<uint32, uint32> :: const_iterator petids_iter = petids.find(atoi(currpetid));
                if(petids_iter == petids.end())             // couldn't find new inserted id
                    ROLLBACK(DUMP_FILE_BROKEN);

                snprintf(newpetid, 20, "%d", petids_iter->second);

                if(!changenth(line, 1, newpetid))
                    ROLLBACK(DUMP_FILE_BROKEN);

                break;
            }
            case DTT_MAIL:                                  // mail
            {
                // id,messageType,stationery,sender,receiver
                if(!changeGuid(line, 1, mails, sObjectMgr->_mailid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changenth(line, 5, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                break;
            }
            case DTT_MAIL_ITEM:                             // mail_items
            {
                // mail_id,item_guid,item_template,receiver
                if(!changeGuid(line, 1, mails, sObjectMgr->_mailid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changeGuid(line, 2, items, sObjectMgr->GetGenerator<HighGuid::Item>().GetNextAfterMaxUsed()))
                    ROLLBACK(DUMP_FILE_BROKEN);
                if(!changenth(line, 4, newguid))
                    ROLLBACK(DUMP_FILE_BROKEN);
                break;
            }
            default:
                TC_LOG_ERROR("FIXME","Unknown dump table type: %u",type);
                break;
        }

        trans->Append(line.c_str());
    }

    CharacterDatabase.CommitTransaction(trans);

    // in case of name conflict player has to rename at login anyway
    //sCharacterCache->AddCharacterCacheEntry(guid, account, name, gender, race, playerClass, level, mails.size(), 0);

    sObjectMgr->GetGenerator<HighGuid::Item>().Set(sObjectMgr->GetGenerator<HighGuid::Item>().GetNextAfterMaxUsed() + items.size());
    sObjectMgr->_mailid     += mails.size();

    if(incHighest)
        sObjectMgr->GetGenerator<HighGuid::Player>().Generate();

    fclose(fin);

    return DUMP_SUCCESS;
}

