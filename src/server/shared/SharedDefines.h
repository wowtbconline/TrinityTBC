#ifndef TRINITY_SHAREDDEFINES_H
#define TRINITY_SHAREDDEFINES_H

#include "Define.h"
#include <cassert>

float const GROUND_HEIGHT_TOLERANCE = 0.05f; // Extra tolerance to z position to check if it is in air or on ground.

enum SpellEffIndex : uint8
{
    EFFECT_0 = 0,
    EFFECT_1 = 1,
    EFFECT_2 = 2
};

// used in script definitions
#define EFFECT_FIRST_FOUND 254
#define EFFECT_ALL 255

// loot modes for creatures and gameobjects, bitmask!
enum LootModes
{
    LOOT_MODE_DEFAULT                  = 0x1,
    LOOT_MODE_HARD_MODE_1              = 0x2,
    LOOT_MODE_HARD_MODE_2              = 0x4,
    LOOT_MODE_HARD_MODE_3              = 0x8,
    LOOT_MODE_HARD_MODE_4              = 0x10,
#ifdef LICH_KING
    LOOT_MODE_JUNK_FISH                = 0x8000
#endif
};

enum Expansions
{
    EXPANSION_CLASSIC                  = 0,
    EXPANSION_THE_BURNING_CRUSADE      = 1,
    EXPANSION_WRATH_OF_THE_LICH_KING   = 2,
    MAX_EXPANSIONS                     = 3
};

inline uint32 GetMaxLevelForExpansion(uint32 expansion)
{
    switch (expansion)
    {
        case EXPANSION_CLASSIC:
            return 60;
        case EXPANSION_THE_BURNING_CRUSADE:
            return 70;
        case EXPANSION_WRATH_OF_THE_LICH_KING:
            return 80;
        default:
            break;
    }
    return 0;
}

enum Gender
{
    GENDER_MALE                        = 0,
    GENDER_FEMALE                      = 1,
    GENDER_NONE                        = 2
};

// Race value is index in ChrRaces.dbc
enum Races
{
    RACE_NONE           = 0,
    RACE_HUMAN          = 1,
    RACE_ORC            = 2,
    RACE_DWARF          = 3,
    RACE_NIGHTELF       = 4,
    RACE_UNDEAD_PLAYER  = 5,
    RACE_TAUREN         = 6,
    RACE_GNOME          = 7,
    RACE_TROLL          = 8,
    //RACE_GOBLIN         = 9,
    RACE_BLOODELF       = 10,
    RACE_DRAENEI        = 11,
    //RACE_FEL_ORC        = 12,
    //RACE_NAGA           = 13,
    //RACE_BROKEN         = 14,
    //RACE_SKELETON       = 15,
    //RACE_VRYKUL         = 16,
    //RACE_TUSKARR        = 17,
    //RACE_FOREST_TROLL   = 18,
    //RACE_TAUNKA         = 19,
    //RACE_NORTHREND_SKELETON = 20,
    //RACE_ICE_TROLL      = 21
};

// max+1 for player race
#define MAX_RACES         12

#define RACEMASK_ALL_PLAYABLE \
    ((1<<(RACE_HUMAN-1))   |(1<<(RACE_ORC-1))          |(1<<(RACE_DWARF-1))   | \
    (1<<(RACE_NIGHTELF-1))|(1<<(RACE_UNDEAD_PLAYER-1))|(1<<(RACE_TAUREN-1))  | \
    (1<<(RACE_GNOME-1))   |(1<<(RACE_TROLL-1))        |(1<<(RACE_BLOODELF-1))| \
    (1<<(RACE_DRAENEI-1)) )

#define RACEMASK_ALLIANCE \
    ((1<<(RACE_HUMAN-1)) | (1<<(RACE_DWARF-1)) | (1<<(RACE_NIGHTELF-1)) | \
     (1<<(RACE_GNOME-1)) | (1<<(RACE_DRAENEI-1)))

#define RACEMASK_HORDE RACEMASK_ALL_PLAYABLE & ~RACEMASK_ALLIANCE

// Class value is index in ChrClasses.dbc
enum Classes : uint32
{
    CLASS_NONE          = 0,
    CLASS_WARRIOR       = 1,
    CLASS_PALADIN       = 2,
    CLASS_HUNTER        = 3,
    CLASS_ROGUE         = 4,
    CLASS_PRIEST        = 5,
    CLASS_DEATH_KNIGHT  = 6,
    CLASS_SHAMAN        = 7,
    CLASS_MAGE          = 8,
    CLASS_WARLOCK       = 9,
    // CLASS_UNK2       = 10,unused
    CLASS_DRUID         = 11,
};

// max+1 for player class
#define MAX_CLASSES       12

#ifdef LICH_KING
#define CLASSMASK_ALL_PLAYABLE \
    ((1<<(CLASS_WARRIOR-1))|(1<<(CLASS_PALADIN-1))|(1<<(CLASS_HUNTER-1))| \
    (1<<(CLASS_ROGUE-1))  |(1<<(CLASS_PRIEST-1)) |(1<<(CLASS_SHAMAN-1))| \
    (1<<(CLASS_MAGE-1))   |(1<<(CLASS_WARLOCK-1))|(1<<(CLASS_DRUID-1))   )
#else
#define CLASSMASK_ALL_PLAYABLE \
    ((1<<(CLASS_WARRIOR-1))|(1<<(CLASS_PALADIN-1))|(1<<(CLASS_HUNTER-1))| \
    (1<<(CLASS_ROGUE-1))  |(1<<(CLASS_PRIEST-1)) |(1<<(CLASS_SHAMAN-1))| \
    (1<<(CLASS_MAGE-1))   |(1<<(CLASS_WARLOCK-1))|(1<<(CLASS_DRUID-1)) | \
    (1<<(CLASS_DEATH_KNIGHT-1)))
#endif

#define MAX_TALENT_TREES 3

enum Specs
{
    SPEC_WARRIOR_ARMS = 0,
    SPEC_WARRIOR_FURY = 1,
    SPEC_WARRIOR_PROTECTION = 2,

    SPEC_PALADIN_HOLY = 0,
    SPEC_PALADIN_PROTECTION = 1,
    SPEC_PALADIN_RETRIBUTION = 2,

    SPEC_HUNTER_BEAST_MASTERY = 0,
    SPEC_HUNTER_MARKSMANSHIP = 1,
    SPEC_HUNTER_SURVIVAL = 2,

    SPEC_ROGUE_ASSASSINATION = 0,
    SPEC_ROGUE_COMBAT = 1,
    SPEC_ROGUE_SUBLETY = 2,

    SPEC_PRIEST_DISCIPLINE = 0,
    SPEC_PRIEST_HOLY = 1,
    SPEC_PRIEST_SHADOW = 2,

#ifdef LICH_KING
    SPEC_DEATH_KNIGHT_BLOOD = 0,
    SPEC_DEATH_KNIGHT_FROST = 1,
    SPEC_DEATH_KNIGHT_UNHOLY = 2,
#endif

    SPEC_SHAMAN_ELEMENTAL = 0,
    SPEC_SHAMAN_ENHANCEMENT = 1,
    SPEC_SHAMAN_RESTORATION = 2,

    SPEC_MAGE_ARCANE = 0,
    SPEC_MAGE_FIRE = 1,
    SPEC_MAGE_FROST = 2,

    SPEC_WARLOCK_AFFLICTION = 0,
    SPEC_WARLOCK_DEMONOLOGY = 1,
    SPEC_WARLOCK_DESTRUCTION = 2,

    SPEC_DRUID_BALANCE = 0,
    SPEC_DRUID_FERAL = 1,
    SPEC_DRUID_RESTORATION = 2
};

// valid classes for creature_template.unit_class
enum UnitClass : uint32
{
    UNIT_CLASS_WARRIOR                  = 1,
    UNIT_CLASS_PALADIN                  = 2,
    UNIT_CLASS_ROGUE                    = 4,
    UNIT_CLASS_MAGE                     = 8
};

#define CLASSMASK_ALL_CREATURES ((1<<(UNIT_CLASS_WARRIOR-1)) | (1<<(UNIT_CLASS_PALADIN-1)) | (1<<(UNIT_CLASS_ROGUE-1)) | (1<<(UNIT_CLASS_MAGE-1)))

#define CLASSMASK_WAND_USERS ((1<<(CLASS_PRIEST-1))|(1<<(CLASS_MAGE-1))|(1<<(CLASS_WARLOCK-1)))

enum SpellFamilyNames
{
    SPELLFAMILY_GENERIC     = 0,
    SPELLFAMILY_UNK1        = 1,                            // events, holidays
    // 2 - unused
    SPELLFAMILY_MAGE        = 3,
    SPELLFAMILY_WARRIOR     = 4,
    SPELLFAMILY_WARLOCK     = 5,
    SPELLFAMILY_PRIEST      = 6,
    SPELLFAMILY_DRUID       = 7,
    SPELLFAMILY_ROGUE       = 8,
    SPELLFAMILY_HUNTER      = 9,
    SPELLFAMILY_PALADIN     = 10,
    SPELLFAMILY_SHAMAN      = 11,
    SPELLFAMILY_UNK2        = 12,
    SPELLFAMILY_POTION      = 13,
#ifdef LICH_KING
    // 14 - unused
    SPELLFAMILY_DEATHKNIGHT = 15,
    // 16 - unused
    SPELLFAMILY_PET         = 17
#endif
};

enum MailResponseType
{
    MAIL_SEND               = 0,
    MAIL_MONEY_TAKEN        = 1,
    MAIL_ITEM_TAKEN         = 2,
    MAIL_RETURNED_TO_SENDER = 3,
    MAIL_DELETED            = 4,
    MAIL_MADE_PERMANENT     = 5
};

enum MailResponseResult
{
    MAIL_OK                            = 0,
    MAIL_ERR_EQUIP_ERROR               = 1,
    MAIL_ERR_CANNOT_SEND_TO_SELF       = 2,
    MAIL_ERR_NOT_ENOUGH_MONEY          = 3,
    MAIL_ERR_RECIPIENT_NOT_FOUND       = 4,
    MAIL_ERR_NOT_YOUR_TEAM             = 5,
    MAIL_ERR_INTERNAL_ERROR            = 6,
    MAIL_ERR_DISABLED_FOR_TRIAL_ACC    = 14,
    MAIL_ERR_RECIPIENT_CAP_REACHED     = 15,
    MAIL_ERR_CANT_SEND_WRAPPED_COD     = 16,
    MAIL_ERR_MAIL_AND_CHAT_SUSPENDED   = 17,
    MAIL_ERR_TOO_MANY_ATTACHMENTS      = 18,
    MAIL_ERR_MAIL_ATTACHMENT_INVALID   = 19,  //Confirmed BC until this one (from CoreCraft)
    MAIL_ERR_ITEM_HAS_EXPIRED          = 21,  //not sure BC
};

#define PLAYER_MAX_BATTLEGROUND_QUEUES 3

enum ReputationRank : uint32
{
    REP_HATED       = 0,
    REP_HOSTILE     = 1,
    REP_UNFRIENDLY  = 2,
    REP_NEUTRAL     = 3,
    REP_FRIENDLY    = 4,
    REP_HONORED     = 5,
    REP_REVERED     = 6,
    REP_EXALTED     = 7
};

#define MIN_REPUTATION_RANK (REP_HATED)
#define MAX_REPUTATION_RANK 8

enum FactionTemplates : uint32
{
    FACTION_NONE                        = 0,
    FACTION_CREATURE                    = 7,
    FACTION_ESCORTEE_A_NEUTRAL_PASSIVE  = 10,
    FACTION_MONSTER                     = 14,
    FACTION_MONSTER_2                   = 16,
    FACTION_TROLL_BLOODSCALP            = 28,
    FACTION_PREY                        = 31,
    FACTION_ESCORTEE_H_NEUTRAL_PASSIVE  = 33,
    FACTION_FRIENDLY                    = 35,
    FACTION_OGRE                        = 45,
    FACTION_ORC_DRAGONMAW               = 62,
    FACTION_HORDE_GENERIC               = 83,
    FACTION_ALLIANCE_GENERIC            = 84,
    FACTION_DEMON                       = 90,
    FACTION_ELEMENTAL                   = 91,
    FACTION_DRAGONFLIGHT_BLACK          = 103,
    FACTION_ESCORTEE_N_NEUTRAL_PASSIVE  = 113,
    FACTION_ENEMY                       = 168,
    FACTION_ESCORTEE_A_NEUTRAL_ACTIVE   = 231,
    FACTION_ESCORTEE_H_NEUTRAL_ACTIVE   = 232,
    FACTION_ESCORTEE_N_NEUTRAL_ACTIVE   = 250,
    FACTION_ESCORTEE_N_FRIEND_PASSIVE   = 290,
    FACTION_TITAN                       = 415,
    FACTION_ESCORTEE_N_FRIEND_ACTIVE    = 495,
    FACTION_GOBLIN_DARK_IRON_BAR_PATRON = 736,
    FACTION_DARK_IRON_DWARVES           = 754,
    FACTION_ESCORTEE_A_PASSIVE          = 774,
    FACTION_ESCORTEE_H_PASSIVE          = 775,
    FACTION_UNDEAD_SCOURGE              = 974,
#ifdef LICH_KING
    FACTION_EARTHEN_RING                = 1726,
    FACTION_ALLIANCE_GENERIC_WG         = 1732,
    FACTION_HORDE_GENERIC_WG            = 1735,
    FACTION_ARAKKOA                     = 1738,
    FACTION_ASHTONGUE_DEATHSWORN        = 1820,
    FACTION_FLAYER_HUNTER               = 1840,
    FACTION_MONSTER_SPAR_BUDDY          = 1868,
    FACTION_ESCORTEE_N_ACTIVE           = 1986,
    FACTION_ESCORTEE_H_ACTIVE           = 2046,
    FACTION_UNDEAD_SCOURGE_2            = 2068,
    FACTION_UNDEAD_SCOURGE_3            = 2084,
    FACTION_SCARLET_CRUSADE             = 2089
#endif
};

enum MoneyConstants : uint32
{
    COPPER = 1,
    SILVER = COPPER*100,
    GOLD   = SILVER*100
};

enum Stats : uint32
{
    STAT_STRENGTH                      = 0,
    STAT_AGILITY                       = 1,
    STAT_STAMINA                       = 2,
    STAT_INTELLECT                     = 3,
    STAT_SPIRIT                        = 4
};

#define MAX_STATS                        5

enum Powers : uint32
{
    POWER_MANA                          = 0,
    POWER_RAGE                          = 1,
    POWER_FOCUS                         = 2,
    POWER_ENERGY                        = 3,
    POWER_HAPPINESS                     = 4,
    MAX_POWERS                          = 5,
    POWER_HEALTH                        = 0xFFFFFFFE    // Used as uint32 in the code, -2 if signed
};

enum SpellSchools : uint32
{
    SPELL_SCHOOL_NORMAL                 = 0,
    SPELL_SCHOOL_HOLY                   = 1,
    SPELL_SCHOOL_FIRE                   = 2,
    SPELL_SCHOOL_NATURE                 = 3,
    SPELL_SCHOOL_FROST                  = 4,
    SPELL_SCHOOL_SHADOW                 = 5,
    SPELL_SCHOOL_ARCANE                 = 6
};

#define MAX_SPELL_SCHOOL                  7

enum SpellSchoolMask : uint32
{
    SPELL_SCHOOL_MASK_NONE    = 0x00,                       // not exist
    SPELL_SCHOOL_MASK_NORMAL  = (1 << SPELL_SCHOOL_NORMAL), // PHYSICAL (Armor)
    SPELL_SCHOOL_MASK_HOLY    = (1 << SPELL_SCHOOL_HOLY  ),
    SPELL_SCHOOL_MASK_FIRE    = (1 << SPELL_SCHOOL_FIRE  ),
    SPELL_SCHOOL_MASK_NATURE  = (1 << SPELL_SCHOOL_NATURE),
    SPELL_SCHOOL_MASK_FROST   = (1 << SPELL_SCHOOL_FROST ),
    SPELL_SCHOOL_MASK_SHADOW  = (1 << SPELL_SCHOOL_SHADOW),
    SPELL_SCHOOL_MASK_ARCANE  = (1 << SPELL_SCHOOL_ARCANE),

    // unions

    // 124, not include normal and holy damage
    SPELL_SCHOOL_MASK_SPELL   = ( SPELL_SCHOOL_MASK_FIRE   |
                                  SPELL_SCHOOL_MASK_NATURE | SPELL_SCHOOL_MASK_FROST  |
                                  SPELL_SCHOOL_MASK_SHADOW | SPELL_SCHOOL_MASK_ARCANE ),
    // 126
    SPELL_SCHOOL_MASK_MAGIC   = ( SPELL_SCHOOL_MASK_HOLY | SPELL_SCHOOL_MASK_SPELL ),

    // 127
    SPELL_SCHOOL_MASK_ALL     = ( SPELL_SCHOOL_MASK_NORMAL | SPELL_SCHOOL_MASK_MAGIC )
};

inline SpellSchools GetFirstSchoolInMask(SpellSchoolMask mask)
{
    for(int i = 0; i < MAX_SPELL_SCHOOL; ++i)
        if(mask & (1 << i))
            return SpellSchools(i);

    return SPELL_SCHOOL_NORMAL;
}

enum ItemQualities
{
    ITEM_QUALITY_POOR                  = 0,                 //GREY
    ITEM_QUALITY_NORMAL                = 1,                 //WHITE
    ITEM_QUALITY_UNCOMMON              = 2,                 //GREEN
    ITEM_QUALITY_RARE                  = 3,                 //BLUE
    ITEM_QUALITY_EPIC                  = 4,                 //PURPLE
    ITEM_QUALITY_LEGENDARY             = 5,                 //ORANGE
    ITEM_QUALITY_ARTIFACT              = 6,                 //LIGHT YELLOW
#ifdef LICH_KING
    ITEM_QUALITY_HEIRLOOM              = 7,
#endif

    MAX_ITEM_QUALITY
};

size_t constexpr MAX_QUEST_DIFFICULTY = 5; // Okay for TBC?
uint32 constexpr QuestDifficultyColors[MAX_QUEST_DIFFICULTY] = {
    0xff40c040,
    0xff808080,
    0xffffff00,
    0xffff8040,
    0xffff2020
};

enum SpellCategory
{
    SPELL_CATEGORY_FOOD             = 11,
    SPELL_CATEGORY_DRINK            = 59,
};

uint32 constexpr ItemQualityColors[MAX_ITEM_QUALITY] =
{
    0xff9d9d9d,        //GREY
    0xffffffff,        //WHITE
    0xff1eff00,        //GREEN
    0xff0070dd,        //BLUE
    0xffa335ee,        //PURPLE
    0xffff8000,        //ORANGE
    0xffe6cc80,        //LIGHT YELLOW
};

// ***********************************
// Spell Attributes definitions
// ***********************************
enum SpellAttr0
{
    SPELL_ATTR0_UNK0                           = 0x00000001,           // 0 (Nost has SPELL_ATTR_DONT_DISPLAY_SPELL_RESULT)
    SPELL_ATTR0_RANGED                         = 0x00000002,           // 1 All ranged abilities have this flag
    SPELL_ATTR0_ON_NEXT_SWING                  = 0x00000004,           // 2 on next swing
    SPELL_ATTR0_IS_REPLENISHMENT               = 0x00000008,           // 3 not set in 2.4.2
    SPELL_ATTR0_ABILITY                        = 0x00000010,           // 4 client puts 'ability' instead of 'spell' in game strings for these spells
    SPELL_ATTR0_TRADESPELL                     = 0x00000020,           // 5 trade spells (recipes), will be added by client to a sublist of profession spell
    SPELL_ATTR0_PASSIVE                        = 0x00000040,           // 6 Passive spell
    SPELL_ATTR0_HIDDEN_CLIENTSIDE              = 0x00000080,           // 7 Spells with this attribute are not visible in spellbook or aura bar
    SPELL_ATTR0_HIDE_IN_COMBAT_LOG             = 0x00000100,           // 8 This attribute controls whether spell appears in combat logs
    SPELL_ATTR0_TARGET_MAINHAND_ITEM           = 0x00000200,           // 9  Client automatically selects item from mainhand slot as a cast target
    SPELL_ATTR0_ON_NEXT_SWING_2                = 0x00000400,           // 10 on next swing 2
    SPELL_ATTR0_UNK11                          = 0x00000800,           // 11
    SPELL_ATTR0_DAYTIME_ONLY                   = 0x00001000,           // 12 only useable at daytime, not set in 2.4.2
    SPELL_ATTR0_NIGHT_ONLY                     = 0x00002000,           // 13 only useable at night, not set in 2.4.2
    SPELL_ATTR0_INDOORS_ONLY                   = 0x00004000,           // 14 only useable indoors, not set in 2.4.2
    SPELL_ATTR0_OUTDOORS_ONLY                  = 0x00008000,           // 15 Only useable outdoors.
    SPELL_ATTR0_NOT_SHAPESHIFT                 = 0x00010000,           // 16 Not while shapeshifted
    SPELL_ATTR0_ONLY_STEALTHED                 = 0x00020000,           // 17 Must be in stealth
    SPELL_ATTR0_AFFECT_WEAPON                  = 0x00040000,           // 18 (OLD : SPELL_ATTR0_DONT_AFFECT_SHEATH_STATE : client won't hide unit weapons in sheath on cast/channel) New name to confirm, but for the example, compare theses spells : 6057, 12714, 16542, 20200. First two have the flag and should apply to weapon, the two lasts haven't and should apply to all damages.
    SPELL_ATTR0_LEVEL_DAMAGE_CALCULATION       = 0x00080000,           // 19 spelldamage depends on caster level
    SPELL_ATTR0_STOP_ATTACK_TARGET             = 0x00100000,           // 20 Stop attack after use this spell (and not begin attack if use)
    SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK   = 0x00200000,           // 21 Cannot be dodged/parried/blocked
    SPELL_ATTR0_CAST_TRACK_TARGET              = 0x00400000,           // 22 Client automatically forces player to face target when casting
    SPELL_ATTR0_CASTABLE_WHILE_DEAD            = 0x00800000,           // 23 castable while dead
    SPELL_ATTR0_CASTABLE_WHILE_MOUNTED         = 0x01000000,           // 24 castable while mounted
    SPELL_ATTR0_DISABLED_WHILE_ACTIVE          = 0x02000000,           // 25 Activate and start cooldown after aura fade or remove summoned creature or go
    SPELL_ATTR0_AURA_IS_DEBUFF                 = 0x04000000,           // 26 (from cmangos, was SPELL_ATTR0_NEGATIVE_1 on TC)
    SPELL_ATTR0_CASTABLE_WHILE_SITTING         = 0x08000000,           // 27 castable while sitting
    SPELL_ATTR0_CANT_USED_IN_COMBAT            = 0x10000000,           // 28 Cannot be used in combat
    SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY  = 0x20000000,           // 29 unaffected by invulnerability (hmm possible not...)
    SPELL_ATTR0_HEARTBEAT_RESIST_CHECK         = 0x40000000,           // 30 NYI random chance the effect will end
    SPELL_ATTR0_CANT_CANCEL                    = 0x80000000,           // 31 positive aura can't be canceled
};

enum SpellAttr1
{
    SPELL_ATTR1_DISMISS_PET                 = 0x00000001,           // 0 for spells without this flag client doesn't allow to summon pet if caster has a pet
    SPELL_ATTR1_DRAIN_ALL_POWER             = 0x00000002,           // 1 use all power (Only paladin Lay of Hands and Bunyanize)
    SPELL_ATTR1_CHANNELED_1                 = 0x00000004,           // 2 channeled 1
    SPELL_ATTR1_CANT_BE_REDIRECTED          = 0x00000008,           // 3
    SPELL_ATTR1_UNK4                        = 0x00000010,           // 4 May be "Starts cooldown after aura fades" according to WH
    SPELL_ATTR1_NOT_BREAK_STEALTH           = 0x00000020,           // 5 Not break stealth
    SPELL_ATTR1_CHANNELED_2                 = 0x00000040,           // 6 channeled 2
    SPELL_ATTR1_CANT_BE_REFLECTED           = 0x00000080,           // 7
    SPELL_ATTR1_CANT_TARGET_IN_COMBAT       = 0x00000100,           // 8 Spell req target not to be in combat state
    SPELL_ATTR1_MELEE_COMBAT_START          = 0x00000200,           // 9 player starts melee combat after this spell is cast
    SPELL_ATTR1_NO_THREAT                   = 0x00000400,           // 10 no generates threat on cast 100% (old NO_INITIAL_AGGRO)
    SPELL_ATTR1_UNK11                       = 0x00000800,           // 11
    SPELL_ATTR1_IS_PICKPOCKET               = 0x00001000,           // 12
    SPELL_ATTR1_FARSIGHT                    = 0x00002000,           // 13 Client removes farsight on aura loss
    SPELL_ATTR1_CHANNEL_TRACK_TARGET        = 0x00004000,           // 14 Client automatically forces player to face target when channeling
    SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY    = 0x00008000,           // 15 remove auras on immunity
    SPELL_ATTR1_UNAFFECTED_BY_SCHOOL_IMMUNE = 0x00010000,           // 16 unaffected by school immunity
    SPELL_ATTR1_UNAUTOCASTABLE_BY_PET       = 0x00020000,           // 17
    SPELL_ATTR1_UNK18                       = 0x00040000,           // 18
    SPELL_ATTR1_CANT_TARGET_SELF            = 0x00080000,           // 19 sun: spell can actually be casted at self but not apply to self, such as 39298
    SPELL_ATTR1_REQ_COMBO_POINTS1           = 0x00100000,           // 20 Req combo points on target
    SPELL_ATTR1_UNK21                       = 0x00200000,           // 21
    SPELL_ATTR1_REQ_COMBO_POINTS2           = 0x00400000,           // 22 Req combo points on target
    SPELL_ATTR1_UNK23                       = 0x00800000,           // 23
    SPELL_ATTR1_IS_FISHING                  = 0x01000000,           // 24 only fishing spells
    SPELL_ATTR1_UNK25                       = 0x02000000,           // 25 not set in 2.4.2
    SPELL_ATTR1_UNK26                       = 0x04000000,           // 26 //stop processing spell if any effect does not find it's target ?
    SPELL_ATTR1_UNK27                       = 0x08000000,           // 27
    SPELL_ATTR1_DONT_DISPLAY_IN_AURA_BAR    = 0x10000000,           // 28 client doesn't display these spells in aura bar
    SPELL_ATTR1_CHANNEL_DISPLAY_SPELL_NAME  = 0x20000000,           // 29 spell name is displayed in cast bar instead of 'channeling' text
    SPELL_ATTR1_ENABLE_AT_DODGE             = 0x40000000,           // 30 overpower
    SPELL_ATTR1_UNK31                       = 0x80000000,           // 31
};

enum SpellAttr2
{
    SPELL_ATTR2_CAN_TARGET_DEAD            = 0x00000001,           // 0
    SPELL_ATTR2_UNK1                       = 0x00000002,           // 1
    SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS      = 0x00000004,           // 2
    SPELL_ATTR2_UNK3                       = 0x00000008,           // 3 sun: maybe remove aura if target leaves caster raid? (see Soul Stone patch notes)
    SPELL_ATTR2_DISPLAY_IN_STANCE_BAR      = 0x00000010,           // 4 client displays icon in stance bar when learned, even if not shapeshift
    SPELL_ATTR2_AUTOREPEAT_FLAG            = 0x00000020,           // 5
    SPELL_ATTR2_CANT_TARGET_TAPPED         = 0x00000040,           // 6 target must be tapped by caster
    SPELL_ATTR2_UNK7                       = 0x00000080,           // 7
    SPELL_ATTR2_UNK8                       = 0x00000100,           // 8 not set in 2.4.2
    SPELL_ATTR2_UNK9                       = 0x00000200,           // 9
    SPELL_ATTR2_UNK10                      = 0x00000400,           // 10
    SPELL_ATTR2_HEALTH_FUNNEL              = 0x00000800,           // 11
    SPELL_ATTR2_UNK12                      = 0x00001000,           // 12
    SPELL_ATTR2_PRESERVE_ENCHANT_IN_ARENA  = 0x00002000,           // 13
    SPELL_ATTR2_UNK14                      = 0x00004000,           // 14
    SPELL_ATTR2_UNK15                      = 0x00008000,           // 15 not set in 2.4.2
    SPELL_ATTR2_TAME_BEAST                 = 0x00010000,           // 16
    SPELL_ATTR2_NOT_RESET_AUTO_ACTIONS     = 0x00020000,           // 17 don't reset timers for melee autoattacks (swings) or ranged autoattacks (autoshoots)
    SPELL_ATTR2_REQ_DEAD_PET               = 0x00040000,           // 18 Only Revive pet
    SPELL_ATTR2_NOT_NEED_SHAPESHIFT        = 0x00080000,           // 19 does not necessarly need shapeshift
    SPELL_ATTR2_BEHIND_TARGET              = 0x00100000,           // 20 must be behind target
    SPELL_ATTR2_DAMAGE_REDUCED_SHIELD      = 0x00200000,           // 21 for ice blocks, pala immunity buffs, priest absorb shields, but used also for other spells -> not sure! //TC SPELL_ATTR2_DAMAGE_REDUCED_SHIELD
    SPELL_ATTR2_UNK22                      = 0x00400000,           // 22
    SPELL_ATTR2_UNK23                      = 0x00800000,           // 23 Only mage Arcane Concentration have this flag //TC SPELL_ATTR2_IS_ARCANE_CONCENTRATION
    SPELL_ATTR2_UNK24                      = 0x01000000,           // 24
    SPELL_ATTR2_UNK25                      = 0x02000000,           // 25
	SPELL_ATTR2_UNAFFECTED_BY_AURA_SCHOOL_IMMUNE = 0x04000000,     // 26 unaffected by school immunity
    SPELL_ATTR2_UNK27                      = 0x08000000,           // 27
    SPELL_ATTR2_UNK28                      = 0x10000000,           // 28 
    SPELL_ATTR2_CANT_CRIT                  = 0x20000000,           // 29 Spell can't crit
    SPELL_ATTR2_TRIGGERED_CAN_TRIGGER_PROC = 0x40000000,           // 30 spell can trigger even if triggered
    SPELL_ATTR2_FOOD_BUFF                  = 0x80000000,           // 31 food, well-fed, and a few others
};

enum SpellAttr3
{
    SPELL_ATTR3_UNK0                         = 0x00000001,           // 0
    SPELL_ATTR3_IGNORE_PROC_SUBCLASS_MASK    = 0x00000002,           // 1 Ignores subclass mask check when checking proc
    SPELL_ATTR3_UNK2                         = 0x00000004,           // 2
    SPELL_ATTR3_BLOCKABLE_SPELL              = 0x00000008,           // 3 Only dmg class melee in 3.1.3
    SPELL_ATTR3_IGNORE_RESURRECTION_TIMER    = 0x00000010,           // 4 Druid Rebirth only this spell have this flag
    SPELL_ATTR3_UNK5                         = 0x00000020,           // 5
    SPELL_ATTR3_UNK6                         = 0x00000040,           // 6
    SPELL_ATTR3_STACK_FOR_DIFF_CASTERS       = 0x00000080,           // 7 separate stack for every caster
    SPELL_ATTR3_ONLY_TARGET_PLAYERS          = 0x00000100,           // 8 can only target players
    SPELL_ATTR3_TRIGGERED_CAN_TRIGGER_PROC_2 = 0x00000200,           //  9 triggered from effect?
    SPELL_ATTR3_MAIN_HAND                    = 0x00000400,           // 10 Main hand weapon required
    SPELL_ATTR3_BATTLEGROUND                 = 0x00000800,           // 11 Can casted only on battleground
    SPELL_ATTR3_ONLY_TARGET_GHOSTS           = 0x00001000,           // 12
    SPELL_ATTR3_DONT_DISPLAY_CHANNEL_BAR     = 0x00002000,           // 13 Clientside attribute - will not display channeling bar
    SPELL_ATTR3_IS_HONORLESS_TARGET          = 0x00004000,           // 14 "Honorless Target" only this spells have this flag
    SPELL_ATTR3_UNK15                        = 0x00008000,           // 15 Auto Shoot, Shoot, Throw,  - this is autoshot flag
    SPELL_ATTR3_CANT_TRIGGER_PROC            = 0x00010000,           // 16 confirmed with many patchnotes
    SPELL_ATTR3_NO_INITIAL_AGGRO             = 0x00020000,           // 17 no initial aggro
    SPELL_ATTR3_IGNORE_HIT_RESULT            = 0x00040000,           // 18 Spell should always hit its target
    SPELL_ATTR3_DISABLE_PROC                 = 0x00080000,           // 19 during aura proc no spells can trigger (20178, 20375)
    SPELL_ATTR3_DEATH_PERSISTENT             = 0x00100000,           // 20 Death persistent spells
    SPELL_ATTR3_UNK21                        = 0x00200000,           // 21
    SPELL_ATTR3_REQ_WAND                     = 0x00400000,           // 22 Req wand
    SPELL_ATTR3_UNK23                        = 0x00800000,           // 23
    SPELL_ATTR3_REQ_OFFHAND                  = 0x01000000,           // 24 Req offhand weapon
    SPELL_ATTR3_TREAT_AS_PERIODIC            = 0x02000000,           // 25 Makes the spell appear as periodic in client combat logs - used by spells that trigger another spell on each tick
    SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED      = 0x04000000,           // 26 auras with this attribute can proc from triggered spell casts with SPELL_ATTR3_TRIGGERED_CAN_TRIGGER_PROC_2 (67736 + 52999)
    SPELL_ATTR3_DRAIN_SOUL                   = 0x08000000,           // 27
    SPELL_ATTR3_UNK28                        = 0x10000000,           // 28
    SPELL_ATTR3_NO_DONE_BONUS                = 0x20000000,           // 29 Ignore caster spellpower and done damage mods?  client doesn't apply spellmods for those spells
    SPELL_ATTR3_DONT_DISPLAY_RANGE           = 0x40000000,           // 30 client doesn't display range in tooltip for those spells
    SPELL_ATTR3_UNK31                        = 0x80000000,           // 31
};

enum SpellAttr4
{
    SPELL_ATTR4_IGNORE_RESISTANCES         = 0x00000001,           // 0 spells with this attribute will completely ignore the target's resistance (these spells can't be resisted)
    SPELL_ATTR4_PROC_ONLY_ON_CASTER        = 0x00000002,           // 1 proc only on effects with TARGET_UNIT_CASTER?
    SPELL_ATTR4_EXPIRE_OFFLINE             = 0x00000004,           // 2 Continues while logged out  //TC SPELL_ATTR4_FADES_WHILE_LOGGED_OUT
    SPELL_ATTR4_CANT_PROC_FROM_SELFCAST    = 0x00000008,           // 3 may be wrong
    SPELL_ATTR4_UNK4                       = 0x00000010,           // 4
    SPELL_ATTR4_UNK5                       = 0x00000020,           // 5
    SPELL_ATTR4_NOT_STEALABLE              = 0x00000040,           // 6 although such auras might be dispellable, they cannot be stolen
    SPELL_ATTR4_CAN_CAST_WHILE_CASTING     = 0x00000080,           // 7 Can be cast while another cast is in progress - see CanCastWhileCasting(SpellRec const*,CGUnit_C *,int &)
    SPELL_ATTR4_FIXED_DAMAGE               = 0x00000100,           // 8 Ignores resilience and any (except mechanic related) damage or % damage taken auras on target.
    SPELL_ATTR4_TRIGGER_ACTIVATE           = 0x00000200,           // 9 initially disabled / trigger activate from event (Execute, Riposte, Deep Freeze end other)
    SPELL_ATTR4_SPELL_VS_EXTEND_COST       = 0x00000400,           // 10 Rogue Shiv have this flag
    SPELL_ATTR4_UNK11                      = 0x00000800,           // 11
    SPELL_ATTR4_UNK12                      = 0x00001000,           // 12
    SPELL_ATTR4_UNK13                      = 0x00002000,           // 13
    SPELL_ATTR4_DAMAGE_DOESNT_BREAK_AURAS  = 0x00004000,           // 14 doesn't break auras by damage from these spells
    SPELL_ATTR4_UNK15                      = 0x00008000,           // 15
    SPELL_ATTR4_NOT_USABLE_IN_ARENA        = 0x00010000,           // 16 not usable in arena
    SPELL_ATTR4_USABLE_IN_ARENA            = 0x00020000,           // 17 usable in arena
    SPELL_ATTR4_AREA_TARGET_CHAIN          = 0x00040000,           // 18 (NYI) hits area targets one after another instead of all at once
    SPELL_ATTR4_UNK19                      = 0x00080000,           // 19 proc delayed, after damage or don't proc on absorb?
    SPELL_ATTR4_NOT_CHECK_SELFCAST_POWER   = 0x00100000,           // 20 NYI supersedes message "More powerful spell applied" for self casts.
    SPELL_ATTR4_UNK21                      = 0x00200000,           // 21
    SPELL_ATTR4_CANT_TRIGGER_ITEM_SPELLS   = 0x00800000,           // 23 spells with this flag should not trigger item spells / enchants (mostly in conjunction with SPELL_ATTR0_STOP_ATTACK_TARGET)
    SPELL_ATTR4_UNK23                      = 0x00800000,           // 23
    SPELL_ATTR4_AUTOSHOT                   = 0x01000000,           // 24
    SPELL_ATTR4_IS_PET_SCALING             = 0x02000000,           // 25 pet scaling auras
    SPELL_ATTR4_CAST_ONLY_IN_OUTLAND       = 0x04000000,           // 26 Can only be used in Outland. Only for flying mounts
    SPELL_ATTR4_INHERIT_CRIT_FROM_AURA     = 0x08000000,           // 27 Volley, Arcane Missiles, Penance -> related to critical on channeled periodical damage spell
    SPELL_ATTR4_UNK28                      = 0x10000000,           // 28
    SPELL_ATTR4_UNK29                      = 0x20000000,           // 29
    SPELL_ATTR4_UNK30                      = 0x40000000,           // 30
    SPELL_ATTR4_UNK31                      = 0x80000000,           // 31
};

enum SpellAttr5
{
    SPELL_ATTR5_CAN_CHANNEL_WHEN_MOVING         = 0x00000001,           // 0 available casting channel spell when moving //sunstrider: not sure about this... player clients send channel cancel even with those
    SPELL_ATTR5_NO_REAGENT_WHILE_PREP           = 0x00000002,           // 1 not need reagents if UNIT_FLAG_PREPARATION
    SPELL_ATTR5_REMOVE_ON_ARENA_ENTER           = 0x00000004,           // 2 remove this aura on arena enter
    SPELL_ATTR5_USABLE_WHILE_STUNNED            = 0x00000008,           // 3 usable while stunned
    SPELL_ATTR5_UNK4                            = 0x00000010,           // 4
    SPELL_ATTR5_SINGLE_TARGET_SPELL             = 0x00000020,           // 5 Only one target can be apply at a time
    SPELL_ATTR5_UNK6                            = 0x00000040,           // 6
    SPELL_ATTR5_UNK7                            = 0x00000080,           // 7
    SPELL_ATTR5_UNK8                            = 0x00000100,           // 8
    SPELL_ATTR5_START_PERIODIC_AT_APPLY         = 0x00000200,           // 9 begin periodic tick at aura apply
    SPELL_ATTR5_HIDE_DURATION                   = 0x00000400,           // 10 do not send duration to client
    SPELL_ATTR5_ALLOW_TARGET_OF_TARGET_AS_TARGET= 0x00000800,           // 11 (NYI) uses target's target as target if original target not valid (intervene for example)
    SPELL_ATTR5_UNK12                           = 0x00001000,           // 12
    SPELL_ATTR5_HASTE_AFFECT_DURATION           = 0x00002000,           // 13 haste effects decrease duration of this
    SPELL_ATTR5_UNK14                           = 0x00004000,           // 14
    SPELL_ATTR5_UNK15                           = 0x00008000,           // 15
    SPELL_ATTR5_SPECIAL_ITEM_CLASS_CHECK        = 0x00010000,           // 16 only "One-Handed Weapon Specialization" (20196 -> 20200). This allows spells with EquippedItemClass to affect spells from other items if the required item is equipped
    SPELL_ATTR5_USABLE_WHILE_FEARED             = 0x00020000,           // 17 usable while feared
    SPELL_ATTR5_USABLE_WHILE_CONFUSED           = 0x00040000,           // 18 usable while confused
    SPELL_ATTR5_DONT_TURN_DURING_CAST           = 0x00080000,           // 19 Blocks caster's turning when casting (client does not automatically turn caster's model to face UNIT_FIELD_TARGET)
    SPELL_ATTR5_UNK20                           = 0x00100000,           // 20
    SPELL_ATTR5_UNK21                           = 0x00200000,           // 21
    SPELL_ATTR5_UNK22                           = 0x00400000,           // 22
    SPELL_ATTR5_UNK23                           = 0x00800000,           // 23
    SPELL_ATTR5_UNK24                           = 0x01000000,           // 24
    SPELL_ATTR5_UNK25                           = 0x02000000,           // 25
    SPELL_ATTR5_SKIP_CHECKCAST_LOS_CHECK        = 0x04000000,           // 26 aoe related - Only two spells on BC have this : 13532 (Thunder Clap) && 27285 (Seed of Corruption)
    SPELL_ATTR5_DONT_SHOW_AURA_IF_SELF_CAST     = 0x08000000,           // 27 Auras with this attribute are not visible on units that are the caster
    SPELL_ATTR5_DONT_SHOW_AURA_IF_NOT_SELF_CAST = 0x10000000,           // 28 Auras with this attribute are not visible on units that are not the caster
    SPELL_ATTR5_UNK29                           = 0x20000000,           // 29
    SPELL_ATTR5_UNK30                           = 0x40000000,           // 30
    SPELL_ATTR5_UNK31                           = 0x80000000,           // 31 Forces all nearby enemies to focus attacks caster
};

enum SpellAttr6
{
    SPELL_ATTR6_DONT_DISPLAY_COOLDOWN           = 0x00000001,           // 0 client doesn't display cooldown in tooltip for these spells
    SPELL_ATTR6_UNK1                            = 0x00000002,           // 1 not set in 2.4.2 TC SPELL_ATTR6_ONLY_IN_ARENA
    SPELL_ATTR6_IGNORE_CASTER_AURAS             = 0x00000004,           // 2 From TC
    SPELL_ATTR6_ASSIST_IGNORE_IMMUNE_FLAG       = 0x00000008,           // 3 skips checking UNIT_FLAG_IMMUNE_TO_PC and UNIT_FLAG_IMMUNE_TO_NPC flags on assist
    SPELL_ATTR6_UNK4                            = 0x00000010,           // 4 not set in 2.4.2
    SPELL_ATTR6_DONT_CONSUME_PROC_CHARGES       = 0x00000020,           // 5 Only Ritual of Summoning on 2.4.3 (698) 
    SPELL_ATTR6_USE_SPELL_CAST_EVENT            = 0x00000040,           // 6 Auras with this attribute trigger SPELL_CAST combat log event instead of SPELL_AURA_START (clientside attribute)
    SPELL_ATTR6_UNK7                            = 0x00000080,           // 7
    SPELL_ATTR6_CANT_TARGET_CROWD_CONTROLLED    = 0x00000100,           // 8
    SPELL_ATTR6_UNK9                            = 0x00000200,           // 9 not set in 2.4.2
    SPELL_ATTR6_UNK10                           = 0x00000400,           // 10 only spells "Find Minerals" (2580, 8388) // TC SPELL_ATTR6_CAN_TARGET_POSSESSED_FRIENDS
    SPELL_ATTR6_NOT_IN_RAID_INSTANCE            = 0x00000800,           // 11 not usable in raid instance
    SPELL_ATTR6_CASTABLE_WHILE_ON_VEHICLE       = 0x00001000,           // 12 not set in 2.4.2 // castable while caster is on vehicle
    SPELL_ATTR6_CAN_TARGET_INVISIBLE            = 0x00002000,           // 13 not set in 2.4.2 // ignore visibility requirement for spell target (phases, invisibility, etc.)
    SPELL_ATTR6_UNK14                           = 0x00004000,           // 14 not set in 2.4.2
    SPELL_ATTR6_UNK15                           = 0x00008000,           // 15 not set in 2.4.2 // only 54368, 67892
    SPELL_ATTR6_UNK16                           = 0x00010000,           // 16 not set in 2.4.2
    SPELL_ATTR6_UNK17                           = 0x00020000,           // 17 not set in 2.4.2 // Mount spell
    SPELL_ATTR6_CAST_BY_CHARMER                 = 0x00040000,           // 18 not set in 2.4.2 // client won't allow to cast these spells when unit is not possessed && charmer of caster will be original caster
    SPELL_ATTR6_UNK19                           = 0x00080000,           // 19 not set in 2.4.2
    SPELL_ATTR6_ONLY_VISIBLE_TO_CASTER          = 0x00100000,           // 20 not set in 2.4.2 // Auras with this attribute are only visible to their caster (or pet's owner)
    SPELL_ATTR6_CLIENT_UI_TARGET_EFFECTS        = 0x00200000,           // 21 not set in 2.4.2 // it's only client-side attribute
    SPELL_ATTR6_UNK22                           = 0x00400000,           // 22 not set in 2.4.2
    SPELL_ATTR6_UNK23                           = 0x00800000,           // 23 not set in 2.4.2
    SPELL_ATTR6_CAN_TARGET_UNTARGETABLE         = 0x01000000,           // 24 not set in 2.4.2
    SPELL_ATTR6_NOT_RESET_SWING_IF_INSTANT      = 0x02000000,           // 25 NYI, not set in 2.4.2
    SPELL_ATTR6_UNK26                           = 0x04000000,           // 26 not set in 2.4.2
    SPELL_ATTR6_LIMIT_PCT_HEALING_MODS          = 0x08000000,           // 27 not set in 2.4.2
    SPELL_ATTR6_UNK28                           = 0x10000000,           // 28 not set in 2.4.2
    SPELL_ATTR6_LIMIT_PCT_DAMAGE_MODS           = 0x20000000,           // 29 not set in 2.4.2
    SPELL_ATTR6_UNK30                           = 0x40000000,           // 30 not set in 2.4.2
    SPELL_ATTR6_IGNORE_CATEGORY_COOLDOWN_MODS   = 0x80000000,           // 31 not set in 2.4.2 // Spells with this attribute skip applying modifiers to category cooldowns
};

#ifdef LICH_KING
enum SpellAttr7
{
    SPELL_ATTR7_UNK0                             = 0x00000001, //  0 Shaman's new spells (Call of the ...), Feign Death.
    SPELL_ATTR7_IGNORE_DURATION_MODS             = 0x00000002, //  1 Duration is not affected by duration modifiers
    SPELL_ATTR7_REACTIVATE_AT_RESURRECT          = 0x00000004, //  2 Paladin's auras and 65607 only.
    SPELL_ATTR7_IS_CHEAT_SPELL                   = 0x00000008, //  3 Cannot cast if caster doesn't have UnitFlag2 & UNIT_FLAG2_ALLOW_CHEAT_SPELLS
    SPELL_ATTR7_UNK4                             = 0x00000010, //  4 Only 47883 (Soulstone Resurrection) and test spell.
    SPELL_ATTR7_SUMMON_PLAYER_TOTEM              = 0x00000020, //  5 Only Shaman player totems.
    SPELL_ATTR7_NO_PUSHBACK_ON_DAMAGE            = 0x00000040, //  6 Does not cause spell pushback on damage
    SPELL_ATTR7_UNK7                             = 0x00000080, //  7 66218 (Launch) spell.
    SPELL_ATTR7_HORDE_ONLY                       = 0x00000100, //  8 Teleports, mounts and other spells.
    SPELL_ATTR7_ALLIANCE_ONLY                    = 0x00000200, //  9 Teleports, mounts and other spells.
    SPELL_ATTR7_DISPEL_CHARGES                   = 0x00000400, // 10 Dispel and Spellsteal individual charges instead of whole aura.
    SPELL_ATTR7_INTERRUPT_ONLY_NONPLAYER         = 0x00000800, // 11 Only non-player casts interrupt, though Feral Charge - Bear has it.
    SPELL_ATTR7_UNK12                            = 0x00001000, // 12 Not set in 3.2.2a.
    SPELL_ATTR7_UNK13                            = 0x00002000, // 13 Not set in 3.2.2a.
    SPELL_ATTR7_UNK14                            = 0x00004000, // 14 Only 52150 (Raise Dead - Pet) spell.
    SPELL_ATTR7_UNK15                            = 0x00008000, // 15 Exorcism. Usable on players? 100% crit chance on undead and demons?
    SPELL_ATTR7_CAN_RESTORE_SECONDARY_POWER      = 0x00010000, // 16 These spells can replenish a powertype, which is not the current powertype.
    SPELL_ATTR7_UNK17                            = 0x00020000, // 17 Only 27965 (Suicide) spell.
    SPELL_ATTR7_HAS_CHARGE_EFFECT                = 0x00040000, // 18 Only spells that have Charge among effects.
    SPELL_ATTR7_ZONE_TELEPORT                    = 0x00080000, // 19 Teleports to specific zones.
    SPELL_ATTR7_UNK20                            = 0x00100000, // 20 Blink, Divine Shield, Ice Block
    SPELL_ATTR7_UNK21                            = 0x00200000, // 21 Not set
    SPELL_ATTR7_UNK22                            = 0x00400000, // 22
    SPELL_ATTR7_UNK23                            = 0x00800000, // 23 Motivate, Mutilate, Shattering Throw
    SPELL_ATTR7_UNK24                            = 0x01000000, // 24 Motivate, Mutilate, Perform Speech, Shattering Throw
    SPELL_ATTR7_UNK25                            = 0x02000000, // 25
    SPELL_ATTR7_UNK26                            = 0x04000000, // 26
    SPELL_ATTR7_UNK27                            = 0x08000000, // 27 Not set
    SPELL_ATTR7_CONSOLIDATED_RAID_BUFF           = 0x10000000, // 28 May be collapsed in raid buff frame (clientside attribute)
    SPELL_ATTR7_UNK29                            = 0x20000000, // 29 only 69028, 71237
    SPELL_ATTR7_UNK30                            = 0x40000000, // 30 Burning Determination, Divine Sacrifice, Earth Shield, Prayer of Mending
    SPELL_ATTR7_CLIENT_INDICATOR                 = 0x80000000
};

#endif

#define MIN_TALENT_SPECS        1
#define MAX_TALENT_SPECS        2

enum SheathTypes
{
    SHEATHETYPE_NONE                   = 0,
    SHEATHETYPE_MAINHAND               = 1,
    SHEATHETYPE_OFFHAND                = 2,
    SHEATHETYPE_LARGEWEAPONLEFT        = 3,
    SHEATHETYPE_LARGEWEAPONRIGHT       = 4,
    SHEATHETYPE_HIPWEAPONLEFT          = 5,
    SHEATHETYPE_HIPWEAPONRIGHT         = 6,
    SHEATHETYPE_SHIELD                 = 7
};

#define MAX_SHEATHETYPE                  8

enum CharacterSlot
{
    SLOT_HEAD                          = 0,
    SLOT_NECK                          = 1,
    SLOT_SHOULDERS                     = 2,
    SLOT_SHIRT                         = 3,
    SLOT_CHEST                         = 4,
    SLOT_WAIST                         = 5,
    SLOT_LEGS                          = 6,
    SLOT_FEET                          = 7,
    SLOT_WRISTS                        = 8,
    SLOT_HANDS                         = 9,
    SLOT_FINGER1                       = 10,
    SLOT_FINGER2                       = 11,
    SLOT_TRINKET1                      = 12,
    SLOT_TRINKET2                      = 13,
    SLOT_BACK                          = 14,
    SLOT_MAIN_HAND                     = 15,
    SLOT_OFF_HAND                      = 16,
    SLOT_RANGED                        = 17,
    SLOT_TABARD                        = 18,
    SLOT_EMPTY                         = 19
};

enum Language
{
    LANG_UNIVERSAL      = 0,
    LANG_ORCISH         = 1,
    LANG_DARNASSIAN     = 2,
    LANG_TAURAHE        = 3,
    LANG_DWARVISH       = 6,
    LANG_COMMON         = 7,
    LANG_DEMONIC        = 8,
    LANG_TITAN          = 9,
    LANG_THALASSIAN     = 10,
    LANG_DRACONIC       = 11,
    LANG_KALIMAG        = 12,
    LANG_GNOMISH        = 13,
    LANG_TROLL          = 14,
    LANG_GUTTERSPEAK    = 33,
    LANG_DRAENEI        = 35,
    LANG_ZOMBIE         = 36,
    LANG_GNOMISH_BINARY = 37,
    LANG_GOBLIN_BINARY  = 38,
    LANG_ADDON          = 0xFFFFFFFF                        // used by addons, in 2.4.0 not exit, replaced by messagetype?
};

#define LANGUAGES_COUNT   19

enum TeamId
{
	TEAM_ALLIANCE = 0,
	TEAM_HORDE,
	TEAM_NEUTRAL
};

enum Team
{
    HORDE               = 67,
    ALLIANCE            = 469,
    //TEAM_STEAMWHEEDLE_CARTEL = 169,                       // not used in code
    //ALLIANCE_FORCES     = 891,
    //HORDE_FORCES        = 892,
    //TEAM_SANCTUARY           = 936,
    //TEAM_OUTLAND             = 980,
    TEAM_OTHER               = 0,                         // if ReputationListId > 0 && Flags != FACTION_FLAG_TEAM_HEADER
};

enum SpellEffects
{
    SPELL_EFFECT_INSTAKILL                 = 1,
    SPELL_EFFECT_SCHOOL_DAMAGE             = 2,
    SPELL_EFFECT_DUMMY                     = 3,
    SPELL_EFFECT_PORTAL_TELEPORT           = 4,
    SPELL_EFFECT_TELEPORT_UNITS            = 5,
    SPELL_EFFECT_APPLY_AURA                = 6,
    SPELL_EFFECT_ENVIRONMENTAL_DAMAGE      = 7,
    SPELL_EFFECT_POWER_DRAIN               = 8,
    SPELL_EFFECT_HEALTH_LEECH              = 9,
    SPELL_EFFECT_HEAL                      = 10,
    SPELL_EFFECT_BIND                      = 11,
    SPELL_EFFECT_PORTAL                    = 12,
    SPELL_EFFECT_RITUAL_BASE               = 13,
    SPELL_EFFECT_RITUAL_SPECIALIZE         = 14,
    SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL    = 15,
    SPELL_EFFECT_QUEST_COMPLETE            = 16,
    SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL    = 17,
    SPELL_EFFECT_RESURRECT                 = 18,
    SPELL_EFFECT_ADD_EXTRA_ATTACKS         = 19,
    SPELL_EFFECT_DODGE                     = 20,
    SPELL_EFFECT_EVADE                     = 21,
    SPELL_EFFECT_PARRY                     = 22,
    SPELL_EFFECT_BLOCK                     = 23,
    SPELL_EFFECT_CREATE_ITEM               = 24,
    SPELL_EFFECT_WEAPON                    = 25,
    SPELL_EFFECT_DEFENSE                   = 26,
    SPELL_EFFECT_PERSISTENT_AREA_AURA      = 27,
    SPELL_EFFECT_SUMMON                    = 28,
    SPELL_EFFECT_LEAP                      = 29,
    SPELL_EFFECT_ENERGIZE                  = 30,
    SPELL_EFFECT_WEAPON_PERCENT_DAMAGE     = 31,
    SPELL_EFFECT_TRIGGER_MISSILE           = 32,
    SPELL_EFFECT_OPEN_LOCK                 = 33,
    SPELL_EFFECT_SUMMON_CHANGE_ITEM        = 34,
    SPELL_EFFECT_APPLY_AREA_AURA_PARTY     = 35,
    SPELL_EFFECT_LEARN_SPELL               = 36,
    SPELL_EFFECT_SPELL_DEFENSE             = 37,
    SPELL_EFFECT_DISPEL                    = 38,
    SPELL_EFFECT_LANGUAGE                  = 39,
    SPELL_EFFECT_DUAL_WIELD                = 40,
    SPELL_EFFECT_41                        = 41, // old SPELL_EFFECT_SUMMON_WILD
    SPELL_EFFECT_42                        = 42, // old SPELL_EFFECT_SUMMON_GUARDIAN
    SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER= 43,
    SPELL_EFFECT_SKILL_STEP                = 44,
    SPELL_EFFECT_UNDEFINED_45              = 45,
    SPELL_EFFECT_SPAWN                     = 46,
    SPELL_EFFECT_TRADE_SKILL               = 47,
    SPELL_EFFECT_STEALTH                   = 48,
    SPELL_EFFECT_DETECT                    = 49,
    SPELL_EFFECT_TRANS_DOOR                = 50,
    SPELL_EFFECT_FORCE_CRITICAL_HIT        = 51,
    SPELL_EFFECT_GUARANTEE_HIT             = 52,
    SPELL_EFFECT_ENCHANT_ITEM              = 53,
    SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY    = 54,
    SPELL_EFFECT_TAMECREATURE              = 55,
    SPELL_EFFECT_SUMMON_PET                = 56,
    SPELL_EFFECT_LEARN_PET_SPELL           = 57,
    SPELL_EFFECT_WEAPON_DAMAGE             = 58,
    SPELL_EFFECT_OPEN_LOCK_ITEM            = 59,
    SPELL_EFFECT_PROFICIENCY               = 60,
    SPELL_EFFECT_SEND_EVENT                = 61,
    SPELL_EFFECT_POWER_BURN                = 62,
    SPELL_EFFECT_THREAT                    = 63,
    SPELL_EFFECT_TRIGGER_SPELL             = 64,
    SPELL_EFFECT_HEALTH_FUNNEL             = 65,
    SPELL_EFFECT_POWER_FUNNEL              = 66,
    SPELL_EFFECT_HEAL_MAX_HEALTH           = 67,
    SPELL_EFFECT_INTERRUPT_CAST            = 68,
    SPELL_EFFECT_DISTRACT                  = 69,
    SPELL_EFFECT_PULL                      = 70,
    SPELL_EFFECT_PICKPOCKET                = 71,
    SPELL_EFFECT_ADD_FARSIGHT              = 72,
    SPELL_EFFECT_73                        = 73, // old SPELL_EFFECT_SUMMON_POSSESSED
    SPELL_EFFECT_74                        = 74, // old SPELL_EFFECT_SUMMON_TOTEM
    SPELL_EFFECT_HEAL_MECHANICAL           = 75,
    SPELL_EFFECT_SUMMON_OBJECT_WILD        = 76,
    SPELL_EFFECT_SCRIPT_EFFECT             = 77,
    SPELL_EFFECT_ATTACK                    = 78,
    SPELL_EFFECT_SANCTUARY                 = 79,
    SPELL_EFFECT_ADD_COMBO_POINTS          = 80,
    SPELL_EFFECT_CREATE_HOUSE              = 81,
    SPELL_EFFECT_BIND_SIGHT                = 82,
    SPELL_EFFECT_DUEL                      = 83,
    SPELL_EFFECT_STUCK                     = 84,
    SPELL_EFFECT_SUMMON_PLAYER             = 85,
    SPELL_EFFECT_ACTIVATE_OBJECT           = 86,
    SPELL_EFFECT_87                        = 87, // old SPELL_EFFECT_SUMMON_TOTEM_SLOT1
    SPELL_EFFECT_88                        = 88, // old SPELL_EFFECT_SUMMON_TOTEM_SLOT2
    SPELL_EFFECT_89                        = 89, // old SPELL_EFFECT_SUMMON_TOTEM_SLOT3
    SPELL_EFFECT_90                        = 90, // old SPELL_EFFECT_SUMMON_TOTEM_SLOT4
    SPELL_EFFECT_THREAT_ALL                = 91,
    SPELL_EFFECT_ENCHANT_HELD_ITEM         = 92,
    SPELL_EFFECT_93                        = 93, // old SPELL_EFFECT_SUMMON_PHANTASM
    SPELL_EFFECT_SELF_RESURRECT            = 94,
    SPELL_EFFECT_SKINNING                  = 95,
    SPELL_EFFECT_CHARGE                    = 96,
    SPELL_EFFECT_97                        = 97, // old SPELL_EFFECT_SUMMON_CRITTER
    SPELL_EFFECT_KNOCK_BACK                = 98,
    SPELL_EFFECT_DISENCHANT                = 99,
    SPELL_EFFECT_INEBRIATE                 = 100,
    SPELL_EFFECT_FEED_PET                  = 101,
    SPELL_EFFECT_DISMISS_PET               = 102,
    SPELL_EFFECT_REPUTATION                = 103,
    SPELL_EFFECT_SUMMON_OBJECT_SLOT1       = 104,
    SPELL_EFFECT_SUMMON_OBJECT_SLOT2       = 105,
    SPELL_EFFECT_SUMMON_OBJECT_SLOT3       = 106,
    SPELL_EFFECT_SUMMON_OBJECT_SLOT4       = 107,
    SPELL_EFFECT_DISPEL_MECHANIC           = 108,
    SPELL_EFFECT_RESURRECT_PET             = 109,
    SPELL_EFFECT_DESTROY_ALL_TOTEMS        = 110,
    SPELL_EFFECT_DURABILITY_DAMAGE         = 111,
    SPELL_EFFECT_112                       = 112, // old SPELL_EFFECT_SUMMON_DEMON
    SPELL_EFFECT_RESURRECT_NEW             = 113,
    SPELL_EFFECT_ATTACK_ME                 = 114,
    SPELL_EFFECT_DURABILITY_DAMAGE_PCT     = 115,
    SPELL_EFFECT_SKIN_PLAYER_CORPSE        = 116,
    SPELL_EFFECT_SPIRIT_HEAL               = 117,
    SPELL_EFFECT_SKILL                     = 118,
    SPELL_EFFECT_APPLY_AREA_AURA_PET       = 119,
    SPELL_EFFECT_TELEPORT_GRAVEYARD        = 120,
    SPELL_EFFECT_NORMALIZED_WEAPON_DMG     = 121,
    SPELL_EFFECT_122                       = 122,
    SPELL_EFFECT_SEND_TAXI                 = 123,
    SPELL_EFFECT_PLAYER_PULL               = 124,
    SPELL_EFFECT_MODIFY_THREAT_PERCENT     = 125,
    SPELL_EFFECT_STEAL_BENEFICIAL_BUFF     = 126,
    SPELL_EFFECT_PROSPECTING               = 127,
    SPELL_EFFECT_APPLY_AREA_AURA_FRIEND    = 128,
    SPELL_EFFECT_APPLY_AREA_AURA_ENEMY     = 129,
    SPELL_EFFECT_REDIRECT_THREAT           = 130,
    SPELL_EFFECT_PLAY_SOUND                = 131,
    SPELL_EFFECT_PLAY_MUSIC                = 132,
    SPELL_EFFECT_UNLEARN_SPECIALIZATION    = 133,
    SPELL_EFFECT_KILL_CREDIT               = 134,
    SPELL_EFFECT_135                       = 135,
    SPELL_EFFECT_HEAL_PCT                  = 136,
    SPELL_EFFECT_ENERGIZE_PCT              = 137,
    SPELL_EFFECT_138                       = 138,
    SPELL_EFFECT_139                       = 139,
    SPELL_EFFECT_FORCE_CAST                = 140,
    SPELL_EFFECT_FORCE_CAST_WITH_VALUE     = 141,
    SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE  = 142,
    SPELL_EFFECT_APPLY_AREA_AURA_OWNER     = 143,
    SPELL_EFFECT_KNOCK_BACK_DEST           = 144,
    SPELL_EFFECT_PULL_TOWARDS_DEST         = 145,
    SPELL_EFFECT_146                       = 146,
    SPELL_EFFECT_QUEST_FAIL                = 147,
    SPELL_EFFECT_148                       = 148, //unused on BC
    SPELL_EFFECT_149                       = 149,
    SPELL_EFFECT_150                       = 150,
    SPELL_EFFECT_TRIGGER_SPELL_2           = 151,
    SPELL_EFFECT_152                       = 152,
    SPELL_EFFECT_153                       = 153,
    TOTAL_SPELL_EFFECTS                    = 154
};

enum SpellCastResult : int
{
    SPELL_CAST_OK                               = -1,
    SPELL_FAILED_AFFECTING_COMBAT               = 0x00,
    SPELL_FAILED_ALREADY_AT_FULL_HEALTH         = 0x01,
    SPELL_FAILED_ALREADY_AT_FULL_MANA           = 0x02,
    SPELL_FAILED_ALREADY_AT_FULL_POWER          = 0x03,
    SPELL_FAILED_ALREADY_BEING_TAMED            = 0x04,
    SPELL_FAILED_ALREADY_HAVE_CHARM             = 0x05,
    SPELL_FAILED_ALREADY_HAVE_SUMMON            = 0x06,
    SPELL_FAILED_ALREADY_OPEN                   = 0x07,
    SPELL_FAILED_AURA_BOUNCED                   = 0x08,
    SPELL_FAILED_AUTOTRACK_INTERRUPTED          = 0x09,
    SPELL_FAILED_BAD_IMPLICIT_TARGETS           = 0x0A,
    SPELL_FAILED_BAD_TARGETS                    = 0x0B,
    SPELL_FAILED_CANT_BE_CHARMED                = 0x0C,
    SPELL_FAILED_CANT_BE_DISENCHANTED           = 0x0D,
    SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL     = 0x0E,
    SPELL_FAILED_CANT_BE_PROSPECTED             = 0x0F,
    SPELL_FAILED_CANT_CAST_ON_TAPPED            = 0x10,
    SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE      = 0x11,
    SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED      = 0x12,
    SPELL_FAILED_CANT_STEALTH                   = 0x13,
    SPELL_FAILED_CASTER_AURASTATE               = 0x14,
    SPELL_FAILED_CASTER_DEAD                    = 0x15,
    SPELL_FAILED_CHARMED                        = 0x16,
    SPELL_FAILED_CHEST_IN_USE                   = 0x17,
    SPELL_FAILED_CONFUSED                       = 0x18,
    SPELL_FAILED_DONT_REPORT                    = 0x19,
    SPELL_FAILED_EQUIPPED_ITEM                  = 0x1A,
    SPELL_FAILED_EQUIPPED_ITEM_CLASS            = 0x1B,
    SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND   = 0x1C,
    SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND    = 0x1D,
    SPELL_FAILED_ERROR                          = 0x1E,
    SPELL_FAILED_FIZZLE                         = 0x1F,
    SPELL_FAILED_FLEEING                        = 0x20,
    SPELL_FAILED_FOOD_LOWLEVEL                  = 0x21,
    SPELL_FAILED_HIGHLEVEL                      = 0x22,
    SPELL_FAILED_HUNGER_SATIATED                = 0x23,
    SPELL_FAILED_IMMUNE                         = 0x24,
    SPELL_FAILED_INTERRUPTED                    = 0x25,
    SPELL_FAILED_INTERRUPTED_COMBAT             = 0x26,
    SPELL_FAILED_ITEM_ALREADY_ENCHANTED         = 0x27,
    SPELL_FAILED_ITEM_GONE                      = 0x28,
    SPELL_FAILED_ITEM_NOT_FOUND                 = 0x29,
    SPELL_FAILED_ITEM_NOT_READY                 = 0x2A,
    SPELL_FAILED_LEVEL_REQUIREMENT              = 0x2B,
    SPELL_FAILED_LINE_OF_SIGHT                  = 0x2C,
    SPELL_FAILED_LOWLEVEL                       = 0x2D,
    SPELL_FAILED_LOW_CASTLEVEL                  = 0x2E,
    SPELL_FAILED_MAINHAND_EMPTY                 = 0x2F,
    SPELL_FAILED_MOVING                         = 0x30,
    SPELL_FAILED_NEED_AMMO                      = 0x31,
    SPELL_FAILED_NEED_AMMO_POUCH                = 0x32,
    SPELL_FAILED_NEED_EXOTIC_AMMO               = 0x33,
    SPELL_FAILED_NOPATH                         = 0x34,
    SPELL_FAILED_NOT_BEHIND                     = 0x35,
    SPELL_FAILED_NOT_FISHABLE                   = 0x36,
    SPELL_FAILED_NOT_FLYING                     = 0x37,
    SPELL_FAILED_NOT_HERE                       = 0x38,
    SPELL_FAILED_NOT_INFRONT                    = 0x39,
    SPELL_FAILED_NOT_IN_CONTROL                 = 0x3A,
    SPELL_FAILED_NOT_KNOWN                      = 0x3B,
    SPELL_FAILED_NOT_MOUNTED                    = 0x3C,
    SPELL_FAILED_NOT_ON_TAXI                    = 0x3D,
    SPELL_FAILED_NOT_ON_TRANSPORT               = 0x3E,
    SPELL_FAILED_NOT_READY                      = 0x3F,
    SPELL_FAILED_NOT_SHAPESHIFT                 = 0x40,
    SPELL_FAILED_NOT_STANDING                   = 0x41,
    SPELL_FAILED_NOT_TRADEABLE                  = 0x42,
    SPELL_FAILED_NOT_TRADING                    = 0x43,
    SPELL_FAILED_NOT_UNSHEATHED                 = 0x44,
    SPELL_FAILED_NOT_WHILE_GHOST                = 0x45,
    SPELL_FAILED_NO_AMMO                        = 0x46,
    SPELL_FAILED_NO_CHARGES_REMAIN              = 0x47,
    SPELL_FAILED_NO_CHAMPION                    = 0x48,
    SPELL_FAILED_NO_COMBO_POINTS                = 0x49,
    SPELL_FAILED_NO_DUELING                     = 0x4A,
    SPELL_FAILED_NO_ENDURANCE                   = 0x4B,
    SPELL_FAILED_NO_FISH                        = 0x4C,
    SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED    = 0x4D,
    SPELL_FAILED_NO_MOUNTS_ALLOWED              = 0x4E,
    SPELL_FAILED_NO_PET                         = 0x4F,
    SPELL_FAILED_NO_POWER                       = 0x50,
    SPELL_FAILED_NOTHING_TO_DISPEL              = 0x51,
    SPELL_FAILED_NOTHING_TO_STEAL               = 0x52,
    SPELL_FAILED_ONLY_ABOVEWATER                = 0x53,
    SPELL_FAILED_ONLY_DAYTIME                   = 0x54,
    SPELL_FAILED_ONLY_INDOORS                   = 0x55,
    SPELL_FAILED_ONLY_MOUNTED                   = 0x56,
    SPELL_FAILED_ONLY_NIGHTTIME                 = 0x57,
    SPELL_FAILED_ONLY_OUTDOORS                  = 0x58,
    SPELL_FAILED_ONLY_SHAPESHIFT                = 0x59,
    SPELL_FAILED_ONLY_STEALTHED                 = 0x5A,
    SPELL_FAILED_ONLY_UNDERWATER                = 0x5B,
    SPELL_FAILED_OUT_OF_RANGE                   = 0x5C,
    SPELL_FAILED_PACIFIED                       = 0x5D,
    SPELL_FAILED_POSSESSED                      = 0x5E,
    SPELL_FAILED_REAGENTS                       = 0x5F,
    SPELL_FAILED_REQUIRES_AREA                  = 0x60,
    SPELL_FAILED_REQUIRES_SPELL_FOCUS           = 0x61,
    SPELL_FAILED_ROOTED                         = 0x62,
    SPELL_FAILED_SILENCED                       = 0x63,
    SPELL_FAILED_SPELL_IN_PROGRESS              = 0x64,
    SPELL_FAILED_SPELL_LEARNED                  = 0x65,
    SPELL_FAILED_SPELL_UNAVAILABLE              = 0x66,
    SPELL_FAILED_STUNNED                        = 0x67,
    SPELL_FAILED_TARGETS_DEAD                   = 0x68,
    SPELL_FAILED_TARGET_AFFECTING_COMBAT        = 0x69,
    SPELL_FAILED_TARGET_AURASTATE               = 0x6A,
    SPELL_FAILED_TARGET_DUELING                 = 0x6B,
    SPELL_FAILED_TARGET_ENEMY                   = 0x6C,
    SPELL_FAILED_TARGET_ENRAGED                 = 0x6D,
    SPELL_FAILED_TARGET_FRIENDLY                = 0x6E,
    SPELL_FAILED_TARGET_IN_COMBAT               = 0x6F,
    SPELL_FAILED_TARGET_IS_PLAYER               = 0x70,
    SPELL_FAILED_TARGET_IS_PLAYER_CONTROLLED    = 0x71,
    SPELL_FAILED_TARGET_NOT_DEAD                = 0x72,
    SPELL_FAILED_TARGET_NOT_IN_PARTY            = 0x73,
    SPELL_FAILED_TARGET_NOT_LOOTED              = 0x74,
    SPELL_FAILED_TARGET_NOT_PLAYER              = 0x75,
    SPELL_FAILED_TARGET_NO_POCKETS              = 0x76,
    SPELL_FAILED_TARGET_NO_WEAPONS              = 0x77,
    SPELL_FAILED_TARGET_UNSKINNABLE             = 0x78,
    SPELL_FAILED_THIRST_SATIATED                = 0x79,
    SPELL_FAILED_TOO_CLOSE                      = 0x7A,
    SPELL_FAILED_TOO_MANY_OF_ITEM               = 0x7B,
    SPELL_FAILED_TOTEM_CATEGORY                 = 0x7C,
    SPELL_FAILED_TOTEMS                         = 0x7D,
    SPELL_FAILED_TRAINING_POINTS                = 0x7E,
    SPELL_FAILED_TRY_AGAIN                      = 0x7F,
    SPELL_FAILED_UNIT_NOT_BEHIND                = 0x80,
    SPELL_FAILED_UNIT_NOT_INFRONT               = 0x81,
    SPELL_FAILED_WRONG_PET_FOOD                 = 0x82,
    SPELL_FAILED_NOT_WHILE_FATIGUED             = 0x83,
    SPELL_FAILED_TARGET_NOT_IN_INSTANCE         = 0x84,
    SPELL_FAILED_NOT_WHILE_TRADING              = 0x85,
    SPELL_FAILED_TARGET_NOT_IN_RAID             = 0x86,
    SPELL_FAILED_DISENCHANT_WHILE_LOOTING       = 0x87,
    SPELL_FAILED_PROSPECT_WHILE_LOOTING         = 0x88,
    SPELL_FAILED_PROSPECT_NEED_MORE             = 0x89,
    SPELL_FAILED_TARGET_FREEFORALL              = 0x8A,
    SPELL_FAILED_NO_EDIBLE_CORPSES              = 0x8B,
    SPELL_FAILED_ONLY_BATTLEGROUNDS             = 0x8C,
    SPELL_FAILED_TARGET_NOT_GHOST               = 0x8D,
    SPELL_FAILED_TOO_MANY_SKILLS                = 0x8E,
    SPELL_FAILED_TRANSFORM_UNUSABLE             = 0x8F,
    SPELL_FAILED_WRONG_WEATHER                  = 0x90,
    SPELL_FAILED_DAMAGE_IMMUNE                  = 0x91,
    SPELL_FAILED_PREVENTED_BY_MECHANIC          = 0x92,
    SPELL_FAILED_PLAY_TIME                      = 0x93,
    SPELL_FAILED_REPUTATION                     = 0x94,
    SPELL_FAILED_MIN_SKILL                      = 0x95,
    SPELL_FAILED_NOT_IN_ARENA                   = 0x96,
    SPELL_FAILED_NOT_ON_SHAPESHIFT              = 0x97,
    SPELL_FAILED_NOT_ON_STEALTHED               = 0x98,
    SPELL_FAILED_NOT_ON_DAMAGE_IMMUNE           = 0x99,
    SPELL_FAILED_NOT_ON_MOUNTED                 = 0x9A,
    SPELL_FAILED_TOO_SHALLOW                    = 0x9B,
    SPELL_FAILED_TARGET_NOT_IN_SANCTUARY        = 0x9C,
    SPELL_FAILED_TARGET_IS_TRIVIAL              = 0x9D,
    SPELL_FAILED_BM_OR_INVISGOD                 = 0x9E,
    SPELL_FAILED_EXPERT_RIDING_REQUIREMENT      = 0x9F,
    SPELL_FAILED_ARTISAN_RIDING_REQUIREMENT     = 0xA0,
    SPELL_FAILED_NOT_IDLE                       = 0xA1,
    SPELL_FAILED_NOT_INACTIVE                   = 0xA2,
    SPELL_FAILED_PARTIAL_PLAYTIME               = 0xA3,
    SPELL_FAILED_NO_PLAYTIME                    = 0xA4,
    SPELL_FAILED_NOT_IN_BATTLEGROUND            = 0xA5,
    SPELL_FAILED_ONLY_IN_ARENA                  = 0xA6,
    SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE = 0xA7,
#ifdef LICH_KING
        SPELL_FAILED_ONLY_IN_ARENA = 168,
        SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE = 169,
        SPELL_FAILED_ON_USE_ENCHANT = 170,
        SPELL_FAILED_NOT_ON_GROUND = 171,
        SPELL_FAILED_CUSTOM_ERROR = 172,
        SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW = 173,
        SPELL_FAILED_TOO_MANY_SOCKETS = 174,
        SPELL_FAILED_INVALID_GLYPH = 175,
        SPELL_FAILED_UNIQUE_GLYPH = 176,
        SPELL_FAILED_GLYPH_SOCKET_LOCKED = 177,
        SPELL_FAILED_NO_VALID_TARGETS = 178,
        SPELL_FAILED_ITEM_AT_MAX_CHARGES = 179,
        SPELL_FAILED_NOT_IN_BARBERSHOP = 180,
        SPELL_FAILED_FISHING_TOO_LOW = 181,
        SPELL_FAILED_ITEM_ENCHANT_TRADE_WINDOW = 182,
        SPELL_FAILED_SUMMON_PENDING = 183,
        SPELL_FAILED_MAX_SOCKETS = 184,
        SPELL_FAILED_PET_CAN_RENAME = 185,
        SPELL_FAILED_TARGET_CANNOT_BE_RESURRECTED = 186,
        SPELL_FAILED_UNKNOWN = 187, // actually doesn't exist in client
#else
    SPELL_FAILED_UNKNOWN                        = 0xA8,
#endif

    MAX_SPELL_FAILED_RESULT,
};

enum StealthType
{
    STEALTH_GENERAL     = 0,
    STEALTH_TRAP        = 1,

    TOTAL_STEALTH_TYPES = 2
};

enum InvisibilityType
{
    INVISIBILITY_GENERAL     =  0,
    INVISIBILITY_UNK1        =  1,
    INVISIBILITY_UNK2        =  2,
    INVISIBILITY_TRAP        =  3,
    INVISIBILITY_UNK4        =  4,
    INVISIBILITY_UNK5        =  5,
    INVISIBILITY_DRUNK       =  6,
    INVISIBILITY_UNK7        =  7,
    INVISIBILITY_UNK8        =  8,
    INVISIBILITY_UNK9        =  9,
    INVISIBILITY_UNK10       = 10,
    INVISIBILITY_UNK11       = 11,

    TOTAL_INVISIBILITY_TYPES = 12
};

enum ServerSideVisibilityType
{
    SERVERSIDE_VISIBILITY_GM          = 0,
    SERVERSIDE_VISIBILITY_GHOST       = 1,

    TOTAL_SERVERSIDE_VISIBILITY_TYPES = 2
};

enum GhostVisibilityType
{
    GHOST_VISIBILITY_ALIVE = 0x1,
    GHOST_VISIBILITY_GHOST = 0x2
};

// Spell aura states
enum AuraStateType : uint32
{   // (C) used in caster aura state     (T) used in target aura state
    // (c) used in caster aura state-not (t) used in target aura state-not
    AURA_STATE_NONE                         = 0,            // C   |
    AURA_STATE_DEFENSE                      = 1,            // C   |
    AURA_STATE_HEALTHLESS_20_PERCENT        = 2,            // CcT |
    AURA_STATE_BERSERKING                   = 3,            // C T |
    AURA_STATE_FROZEN                       = 4,            //  c t| some limitation to charge spells (?) and target test spells
    AURA_STATE_JUDGEMENT                    = 5,            // C   |
    //AURA_STATE_UNKNOWN6                   = 6,            //     | not used
    AURA_STATE_HUNTER_PARRY                 = 7,            // C   |
    //AURA_STATE_UNKNOWN7                   = 7,            //  c  | random/focused bursts spells (?)
    //AURA_STATE_UNKNOWN8                   = 8,            //     | not used
    //AURA_STATE_UNKNOWN9                   = 9,            //     | not used
    AURA_STATE_WARRIOR_VICTORY_RUSH         = 10,           // C   | warrior victory rush
    AURA_STATE_HUNTER_CRIT_STRIKE           = 10,           // C   | hunter crit strike
    AURA_STATE_CRIT                         = 11,           // C   |
    AURA_STATE_FAERIE_FIRE                  = 12,           //  c t|
    AURA_STATE_HEALTHLESS_35_PERCENT        = 13,           // C T |
    AURA_STATE_CONFLAGRATE                  = 14,           //   T |
    AURA_STATE_SWIFTMEND                    = 15,           //   T |
    AURA_STATE_DEADLY_POISON                = 16,           //   T |
    AURA_STATE_FORBEARANCE                  = 17,           //  c t|
    AURA_STATE_WEAKENED_SOUL                = 18,           //    t|
    AURA_STATE_HYPOTHERMIA                  = 19            //  c  |
};
#define PER_CASTER_AURA_STATE_MASK ( (1<<(AURA_STATE_CONFLAGRATE-1))|(1<<(AURA_STATE_DEADLY_POISON-1)) )

// Spell mechanics
enum Mechanics : uint32
{
    MECHANIC_NONE             = 0,
    MECHANIC_CHARM            = 1,
    MECHANIC_DISORIENTED      = 2,
    MECHANIC_DISARM           = 3,
    MECHANIC_DISTRACT         = 4,
    MECHANIC_FEAR             = 5,
    MECHANIC_FUMBLE           = 6,
    MECHANIC_ROOT             = 7,
    MECHANIC_PACIFY           = 8,                          //0 spells use this mechanic
    MECHANIC_SILENCE          = 9,
    MECHANIC_SLEEP            = 10,
    MECHANIC_SNARE            = 11,
    MECHANIC_STUN             = 12,
    MECHANIC_FREEZE           = 13,
    MECHANIC_KNOCKOUT         = 14,
    MECHANIC_BLEED            = 15,
    MECHANIC_BANDAGE          = 16,
    MECHANIC_POLYMORPH        = 17,
    MECHANIC_BANISH           = 18,
    MECHANIC_SHIELD           = 19,
    MECHANIC_SHACKLE          = 20,
    MECHANIC_MOUNT            = 21,
    MECHANIC_PERSUADE         = 22,                         //0 spells use this mechanic
    MECHANIC_TURN             = 23,
    MECHANIC_HORROR           = 24,
    MECHANIC_INVULNERABILITY  = 25,
    MECHANIC_INTERRUPT        = 26,
    MECHANIC_DAZE             = 27,
    MECHANIC_DISCOVERY        = 28,
    MECHANIC_IMMUNE_SHIELD    = 29,                         // Divine (Blessing) Shield/Protection and Ice Block
    MECHANIC_SAPPED           = 30,

    MAX_MECHANIC,
};

// Used for spell 42292 Immune Movement Impairment and Loss of Control (0x49967da6)
#define IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK ( \
    (1<<MECHANIC_CHARM   )|(1<<MECHANIC_DISORIENTED )|(1<<MECHANIC_FEAR  )| \
    (1<<MECHANIC_ROOT    )|(1<<MECHANIC_PACIFY   )|(1<<MECHANIC_SLEEP )| \
    (1<<MECHANIC_SNARE   )|(1<<MECHANIC_STUN     )|(1<<MECHANIC_FREEZE)| \
    (1<<MECHANIC_KNOCKOUT)|(1<<MECHANIC_POLYMORPH)|(1<<MECHANIC_BANISH)| \
    (1<<MECHANIC_SHACKLE )|(1<<MECHANIC_TURN     )|(1<<MECHANIC_HORROR)| \
    (1<<MECHANIC_DAZE    )|(1<<MECHANIC_SAPPED   ) )

// Spell dispel type
enum DispelType
{
    DISPEL_NONE         = 0,
    DISPEL_MAGIC        = 1,
    DISPEL_CURSE        = 2,
    DISPEL_DISEASE      = 3,
    DISPEL_POISON       = 4,
    DISPEL_STEALTH      = 5,
    DISPEL_INVISIBILITY = 6,
    DISPEL_ALL          = 7,
    DISPEL_SPE_NPC_ONLY = 8,
    DISPEL_ENRAGE       = 9,
    DISPEL_ZG_TICKET    = 10
};

#define DISPEL_ALL_MASK ( (1<<DISPEL_MAGIC) | (1<<DISPEL_CURSE) | (1<<DISPEL_DISEASE) | (1<<DISPEL_POISON) )

//To all Immune system,if target has immunes,
//some spell that related to ImmuneToDispel or ImmuneToSchool or ImmuneToDamage type can't cast to it,
//some spell_effects that related to ImmuneToEffect<effect>(only this effect in the spell) can't cast to it,
//some aura(related to Mechanics or ImmuneToState<aura>) can't apply to it.
enum SpellImmunity
{
    IMMUNITY_EFFECT                = 0,                     // enum SpellEffects
    IMMUNITY_STATE                 = 1,                     // enum AuraType
    IMMUNITY_SCHOOL                = 2,                     // enum SpellSchoolMask
    IMMUNITY_DAMAGE                = 3,                     // enum SpellSchoolMask
    IMMUNITY_DISPEL                = 4,                     // enum DispelType
    IMMUNITY_MECHANIC              = 5,                     // enum Mechanics
    IMMUNITY_ID                    = 6
};

#define MAX_SPELL_IMMUNITY           7

enum Targets
{
    TARGET_UNIT_CASTER                 = 1,
    TARGET_UNIT_NEARBY_ENEMY           = 2, // only one spell has that, but regardless, it's a target type after all
    TARGET_UNIT_NEARBY_ALLY            = 3,
    TARGET_UNIT_NEARBY_ALLY_UNK        = 4,
    TARGET_UNIT_PET                    = 5,
    TARGET_UNIT_TARGET_ENEMY           = 6,
    TARGET_UNIT_SRC_AREA_ENTRY         = 7,
    TARGET_UNIT_DEST_AREA_ENTRY        = 8,
    TARGET_DEST_HOME                   = 9,  // uses in teleport to innkeeper spells
    TARGET_UNIT_TARGET_DEST_CASTER     = 11, // teleport target to caster
    TARGET_UNIT_SRC_AREA_ENEMY         = 15,
    TARGET_UNIT_DEST_AREA_ENEMY        = 16,
    TARGET_DEST_DB                     = 17, // uses in teleport spells and some other
    TARGET_DEST_CASTER                 = 18,
    TARGET_UNIT_CASTER_AREA_PARTY      = 20,
    TARGET_UNIT_TARGET_ALLY            = 21,
    TARGET_SRC_CASTER                  = 22,
    TARGET_GAMEOBJECT_TARGET           = 23,
    TARGET_UNIT_CONE_ENEMY             = 24,
    TARGET_UNIT_TARGET_ANY             = 25,
    TARGET_GAMEOBJECT_ITEM_TARGET      = 26,
    //TARGET_OBJECT_ITEM_PICKLOCK
    TARGET_UNIT_MASTER                 = 27,
    TARGET_DEST_DYNOBJ_ENEMY           = 28,
    TARGET_DEST_DYNOBJ_ALLY            = 29, // only for effect 27
    TARGET_UNIT_SRC_AREA_ALLY          = 30, // in TargetB used only with TARGET_SRC_CASTER and in self casting range in TargetA
    TARGET_UNIT_DEST_AREA_ALLY         = 31,
    TARGET_DEST_CASTER_SUMMON          = 32,
    TARGET_UNIT_SRC_AREA_PARTY         = 33,
    TARGET_UNIT_DEST_AREA_PARTY        = 34, // used in Tranquility
    TARGET_UNIT_TARGET_PARTY           = 35,
    TARGET_DEST_CASTER_UNK_36          = 36, //unknown
#ifdef LICH_KING
    TARGET_UNIT_LASTTARGET_AREA_PARTY  = 37,
#else
    TARGET_UNIT_AREA_PARTY = 37, //spell on BC simply have this target while LK use this as B target and specify target in A
#endif
    TARGET_UNIT_NEARBY_ENTRY           = 38,
    TARGET_DEST_CASTER_FISHING         = 39,
    TARGET_GAMEOBJECT_NEARBY_ENTRY     = 40,
    //the next four may not be in the right order, but do we care
    TARGET_DEST_CASTER_FRONT_LEFT      = 41, //earth totem
    TARGET_DEST_CASTER_BACK_LEFT       = 42, //water totem
    TARGET_DEST_CASTER_BACK_RIGHT      = 43, //air totem
    TARGET_DEST_CASTER_FRONT_RIGHT     = 44, //fire totem
    TARGET_UNIT_TARGET_CHAINHEAL_ALLY  = 45,
    TARGET_DEST_NEARBY_ENTRY           = 46,
    TARGET_DEST_CASTER_FRONT           = 47,
    TARGET_DEST_CASTER_BACK            = 48,
    TARGET_DEST_CASTER_RIGHT           = 49,
    TARGET_DEST_CASTER_LEFT            = 50,
    TARGET_GAMEOBJECT_SRC_AREA         = 51,
    TARGET_GAMEOBJECT_DEST_AREA        = 52,
    TARGET_DEST_TARGET_ENEMY           = 53, // set unit coordinates as dest, only 16 target B imlemented
    TARGET_UNIT_CONE_ENEMY_UNKNOWN     = 54, // 180 degree, or different angle
    TARGET_DEST_CASTER_FRONT_LEAP      = 55, // for a leap spell
    TARGET_UNIT_CASTER_AREA_RAID       = 56,
    TARGET_UNIT_TARGET_RAID            = 57,
    TARGET_UNIT_NEARBY_RAID            = 58,
    TARGET_UNIT_CONE_ALLY              = 59,
    TARGET_UNIT_CONE_ENTRY             = 60,
    TARGET_UNIT_TARGET_AREA_RAID_CLASS = 61,
    TARGET_TEST                        = 62, // for a test spell
    TARGET_DEST_TARGET_ANY             = 63,
    TARGET_DEST_TARGET_FRONT           = 64,
    TARGET_DEST_TARGET_BACK            = 65,                // uses in teleport behind spells
    TARGET_DEST_TARGET_RIGHT           = 66,
    TARGET_DEST_TARGET_LEFT            = 67,
    TARGET_DEST_TARGET_FRONT_LEFT      = 68,
    TARGET_DEST_TARGET_BACK_LEFT       = 69,
    TARGET_DEST_TARGET_BACK_RIGHT      = 70,
    TARGET_DEST_TARGET_FRONT_RIGHT     = 71,
    TARGET_DEST_CASTER_RANDOM          = 72,
    TARGET_DEST_CASTER_RADIUS          = 73,
    TARGET_DEST_TARGET_RANDOM          = 74,
    TARGET_DEST_TARGET_RADIUS          = 75,
    TARGET_DEST_CHANNEL_TARGET         = 76,
    TARGET_UNIT_CHANNEL_TARGET         = 77,
    TARGET_DEST_DEST_FRONT             = 78,
    TARGET_DEST_DEST_BACK              = 79,
    TARGET_DEST_DEST_RIGHT             = 80,
    TARGET_DEST_DEST_LEFT              = 81,
    TARGET_DEST_DEST_FRONT_LEFT        = 82,
    TARGET_DEST_DEST_BACK_LEFT         = 83,
    TARGET_DEST_DEST_BACK_RIGHT        = 84,
    TARGET_DEST_DEST_FRONT_RIGHT       = 85,
    TARGET_DEST_DEST_RANDOM            = 86,
    TARGET_DEST_DEST                   = 87,
    TARGET_DEST_DYNOBJ_NONE            = 88,
    TARGET_DEST_TRAJ                   = 89,
    TARGET_UNIT_TARGET_MINIPET         = 90,
    TARGET_DEST_DEST_RADIUS            = 91,
    TARGET_UNIT_SUMMONER               = 92,
    TARGET_CORPSE_SRC_AREA_ENEMY       = 93, 
#ifdef LICH_KING
    TARGET_UNIT_VEHICLE = 94,
    TARGET_UNIT_TARGET_PASSENGER = 95,
    TARGET_UNIT_PASSENGER_0 = 96,
    TARGET_UNIT_PASSENGER_1 = 97,
    TARGET_UNIT_PASSENGER_2 = 98,
    TARGET_UNIT_PASSENGER_3 = 99,
    TARGET_UNIT_PASSENGER_4 = 100,
    TARGET_UNIT_PASSENGER_5 = 101,
    TARGET_UNIT_PASSENGER_6 = 102,
    TARGET_UNIT_PASSENGER_7 = 103,
    TARGET_UNIT_CONE_ENEMY_104 = 104,
    TARGET_UNIT_UNK_105 = 105, // 1 spell
    TARGET_DEST_CHANNEL_CASTER = 106,
    TARGET_UNK_DEST_AREA_UNK_107 = 107, // not enough info - only generic spells avalible
    TARGET_GAMEOBJECT_CONE = 108,
    TARGET_DEST_UNK_110 = 110, // 1 spell
    TOTAL_SPELL_TARGETS
#endif
};

#define TOTAL_SPELL_TARGETS              94

enum SpellMissInfo : uint32
{
    SPELL_MISS_NONE                    = 0,
    SPELL_MISS_MISS                    = 1,
    SPELL_MISS_RESIST                  = 2,
    SPELL_MISS_DODGE                   = 3,
    SPELL_MISS_PARRY                   = 4,
    SPELL_MISS_BLOCK                   = 5,
    SPELL_MISS_EVADE                   = 6,
    SPELL_MISS_IMMUNE                  = 7,
    SPELL_MISS_IMMUNE2                 = 8,
    SPELL_MISS_DEFLECT                 = 9,
    SPELL_MISS_ABSORB                  = 10,
    SPELL_MISS_REFLECT                 = 11,

    SPELL_MISS_TOTAL,
    //sun: default value for spell force hit system, not forcing anything
    SPELL_FORCE_HIT_DEFAULT = SPELL_MISS_TOTAL,
};

enum SpellHitType
{
    SPELL_HIT_TYPE_CRIT_DEBUG           = 0x01,
    SPELL_HIT_TYPE_CRIT                 = 0x02,
    SPELL_HIT_TYPE_HIT_DEBUG            = 0x04,
    SPELL_HIT_TYPE_SPLIT                = 0x08,
    SPELL_HIT_TYPE_VICTIM_IS_ATTACKER   = 0x10,
    SPELL_HIT_TYPE_ATTACK_TABLE_DEBUG   = 0x20,
};

enum SpellDmgClass
{
    SPELL_DAMAGE_CLASS_NONE     = 0,
    SPELL_DAMAGE_CLASS_MAGIC    = 1,
    SPELL_DAMAGE_CLASS_MELEE    = 2,
    SPELL_DAMAGE_CLASS_RANGED   = 3
};

enum SpellPreventionType
{
    SPELL_PREVENTION_TYPE_NONE      = 0,
    SPELL_PREVENTION_TYPE_SILENCE   = 1,
    SPELL_PREVENTION_TYPE_PACIFY    = 2
};

enum GameobjectTypes
{
    GAMEOBJECT_TYPE_DOOR                   = 0,
    GAMEOBJECT_TYPE_BUTTON                 = 1,
    GAMEOBJECT_TYPE_QUESTGIVER             = 2,
    GAMEOBJECT_TYPE_CHEST                  = 3,
    GAMEOBJECT_TYPE_BINDER                 = 4,
    GAMEOBJECT_TYPE_GENERIC                = 5,
    GAMEOBJECT_TYPE_TRAP                   = 6,
    GAMEOBJECT_TYPE_CHAIR                  = 7,
    GAMEOBJECT_TYPE_SPELL_FOCUS            = 8,
    GAMEOBJECT_TYPE_TEXT                   = 9,
    GAMEOBJECT_TYPE_GOOBER                 = 10,
    GAMEOBJECT_TYPE_TRANSPORT              = 11,
    GAMEOBJECT_TYPE_AREADAMAGE             = 12,
    GAMEOBJECT_TYPE_CAMERA                 = 13,
    GAMEOBJECT_TYPE_MAP_OBJECT             = 14,
    GAMEOBJECT_TYPE_MO_TRANSPORT           = 15,
    GAMEOBJECT_TYPE_DUEL_ARBITER           = 16,
    GAMEOBJECT_TYPE_FISHINGNODE            = 17,
    GAMEOBJECT_TYPE_SUMMONING_RITUAL       = 18,
    GAMEOBJECT_TYPE_MAILBOX                = 19,
    GAMEOBJECT_TYPE_AUCTIONHOUSE           = 20,
    GAMEOBJECT_TYPE_GUARDPOST              = 21,
    GAMEOBJECT_TYPE_SPELLCASTER            = 22,
    GAMEOBJECT_TYPE_MEETINGSTONE           = 23,
    GAMEOBJECT_TYPE_FLAGSTAND              = 24,
    GAMEOBJECT_TYPE_FISHINGHOLE            = 25,
    GAMEOBJECT_TYPE_FLAGDROP               = 26,
    GAMEOBJECT_TYPE_MINI_GAME              = 27,
    GAMEOBJECT_TYPE_LOTTERY_KIOSK          = 28,
    GAMEOBJECT_TYPE_CAPTURE_POINT          = 29,
    GAMEOBJECT_TYPE_AURA_GENERATOR         = 30,
    GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY     = 31,
    GAMEOBJECT_TYPE_BARBER_CHAIR           = 32,
    GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING  = 33,
    GAMEOBJECT_TYPE_GUILD_BANK             = 34,
#ifdef LICH_KING
    GAMEOBJECT_TYPE_TRAPDOOR               = 35,
#endif

    MAX_GAMEOBJECT_TYPE,                         // sending to client this or greater value can crash client.
};

#define MAX_GAMEOBJECT_DATA                  24             // Max number of uint32 vars in gameobject_template data field

#define GAMEOBJECT_FISHINGNODE_ENTRY        35591           // Better to define it somewhere instead of hardcoding everywhere

enum GameObjectFlags
{
    GO_FLAG_IN_USE           = 0x00000001,                   //disables interaction while animated
    GO_FLAG_LOCKED           = 0x00000002,                   //require key, spell, event, etc to be opened. Makes "Locked" appear in tooltip
    GO_FLAG_INTERACT_COND    = 0x00000004,                   //cannot interact (condition to interact - requires GO_DYNFLAG_LO_ACTIVATE to enable interaction clientside)
    GO_FLAG_TRANSPORT        = 0x00000008,                   //any kind of transport? Object can transport (elevator, boat, car)
    GO_FLAG_NOT_SELECTABLE   = 0x00000010,                   //not selectable even in GM mode
    GO_FLAG_NODESPAWN        = 0x00000020,                   //never despawn, typically for doors, they just change state
    GO_FLAG_AI_OBSTACLE      = 0x00000040,                   //makes the client register the object in something called AIObstacleMgr, unknown what it does
#ifdef LICH_KING
    GO_FLAG_FREEZE_ANIMATION = 0x00000080,
    GO_FLAG_DAMAGED          = 0x00000200,
    GO_FLAG_DESTROYED        = 0x00000400,
#endif
};

enum GameObjectDynamicLowFlags
{
    GO_DYNFLAG_LO_ACTIVATE          = 0x01,                 // enables interaction with GO
    GO_DYNFLAG_LO_ANIMATE           = 0x02,                 // possibly more distinct animation of GO
    GO_DYNFLAG_LO_NO_INTERACT       = 0x04,                 // appears to disable interaction (not fully verified)
    GO_DYNFLAG_LO_SPARKLE           = 0x08,                 // makes GO sparkle
    GO_DYNFLAG_LO_STOPPED           = 0x10                  // Transport is stopped
};

// client side GO show states
enum GOState: uint32
{
    GO_STATE_ACTIVE             = 0,                        // show in world as used and not reset (closed door open)
    GO_STATE_READY              = 1,                        // show in world as ready (closed door close)
    GO_STATE_ACTIVE_ALTERNATIVE = 2,                        // show in world as used in alt way and not reset (closed door open by cannon fire)
};

#define MAX_GO_STATE              3

enum TextEmotes
{
    TEXTEMOTE_AGREE                = 1,
    TEXTEMOTE_AMAZE                = 2,
    TEXTEMOTE_ANGRY                = 3,
    TEXTEMOTE_APOLOGIZE            = 4,
    TEXTEMOTE_APPLAUD              = 5,
    TEXTEMOTE_BASHFUL              = 6,
    TEXTEMOTE_BECKON               = 7,
    TEXTEMOTE_BEG                  = 8,
    TEXTEMOTE_BITE                 = 9,
    TEXTEMOTE_BLEED                = 10,
    TEXTEMOTE_BLINK                = 11,
    TEXTEMOTE_BLUSH                = 12,
    TEXTEMOTE_BONK                 = 13,
    TEXTEMOTE_BORED                = 14,
    TEXTEMOTE_BOUNCE               = 15,
    TEXTEMOTE_BRB                  = 16,
    TEXTEMOTE_BOW                  = 17,
    TEXTEMOTE_BURP                 = 18,
    TEXTEMOTE_BYE                  = 19,
    TEXTEMOTE_CACKLE               = 20,
    TEXTEMOTE_CHEER                = 21,
    TEXTEMOTE_CHICKEN              = 22,
    TEXTEMOTE_CHUCKLE              = 23,
    TEXTEMOTE_CLAP                 = 24,
    TEXTEMOTE_CONFUSED             = 25,
    TEXTEMOTE_CONGRATULATE         = 26,
    TEXTEMOTE_COUGH                = 27,
    TEXTEMOTE_COWER                = 28,
    TEXTEMOTE_CRACK                = 29,
    TEXTEMOTE_CRINGE               = 30,
    TEXTEMOTE_CRY                  = 31,
    TEXTEMOTE_CURIOUS              = 32,
    TEXTEMOTE_CURTSEY              = 33,
    TEXTEMOTE_DANCE                = 34,
    TEXTEMOTE_DRINK                = 35,
    TEXTEMOTE_DROOL                = 36,
    TEXTEMOTE_EAT                  = 37,
    TEXTEMOTE_EYE                  = 38,
    TEXTEMOTE_FART                 = 39,
    TEXTEMOTE_FIDGET               = 40,
    TEXTEMOTE_FLEX                 = 41,
    TEXTEMOTE_FROWN                = 42,
    TEXTEMOTE_GASP                 = 43,
    TEXTEMOTE_GAZE                 = 44,
    TEXTEMOTE_GIGGLE               = 45,
    TEXTEMOTE_GLARE                = 46,
    TEXTEMOTE_GLOAT                = 47,
    TEXTEMOTE_GREET                = 48,
    TEXTEMOTE_GRIN                 = 49,
    TEXTEMOTE_GROAN                = 50,
    TEXTEMOTE_GROVEL               = 51,
    TEXTEMOTE_GUFFAW               = 52,
    TEXTEMOTE_HAIL                 = 53,
    TEXTEMOTE_HAPPY                = 54,
    TEXTEMOTE_HELLO                = 55,
    TEXTEMOTE_HUG                  = 56,
    TEXTEMOTE_HUNGRY               = 57,
    TEXTEMOTE_KISS                 = 58,
    TEXTEMOTE_KNEEL                = 59,
    TEXTEMOTE_LAUGH                = 60,
    TEXTEMOTE_LAYDOWN              = 61,
    TEXTEMOTE_MESSAGE              = 62,
    TEXTEMOTE_MOAN                 = 63,
    TEXTEMOTE_MOON                 = 64,
    TEXTEMOTE_MOURN                = 65,
    TEXTEMOTE_NO                   = 66,
    TEXTEMOTE_NOD                  = 67,
    TEXTEMOTE_NOSEPICK             = 68,
    TEXTEMOTE_PANIC                = 69,
    TEXTEMOTE_PEER                 = 70,
    TEXTEMOTE_PLEAD                = 71,
    TEXTEMOTE_POINT                = 72,
    TEXTEMOTE_POKE                 = 73,
    TEXTEMOTE_PRAY                 = 74,
    TEXTEMOTE_ROAR                 = 75,
    TEXTEMOTE_ROFL                 = 76,
    TEXTEMOTE_RUDE                 = 77,
    TEXTEMOTE_SALUTE               = 78,
    TEXTEMOTE_SCRATCH              = 79,
    TEXTEMOTE_SEXY                 = 80,
    TEXTEMOTE_SHAKE                = 81,
    TEXTEMOTE_SHOUT                = 82,
    TEXTEMOTE_SHRUG                = 83,
    TEXTEMOTE_SHY                  = 84,
    TEXTEMOTE_SIGH                 = 85,
    TEXTEMOTE_SIT                  = 86,
    TEXTEMOTE_SLEEP                = 87,
    TEXTEMOTE_SNARL                = 88,
    TEXTEMOTE_SPIT                 = 89,
    TEXTEMOTE_STARE                = 90,
    TEXTEMOTE_SURPRISED            = 91,
    TEXTEMOTE_SURRENDER            = 92,
    TEXTEMOTE_TALK                 = 93,
    TEXTEMOTE_TALKEX               = 94,
    TEXTEMOTE_TALKQ                = 95,
    TEXTEMOTE_TAP                  = 96,
    TEXTEMOTE_THANK                = 97,
    TEXTEMOTE_THREATEN             = 98,
    TEXTEMOTE_TIRED                = 99,
    TEXTEMOTE_VICTORY              = 100,
    TEXTEMOTE_WAVE                 = 101,
    TEXTEMOTE_WELCOME              = 102,
    TEXTEMOTE_WHINE                = 103,
    TEXTEMOTE_WHISTLE              = 104,
    TEXTEMOTE_WORK                 = 105,
    TEXTEMOTE_YAWN                 = 106,
    TEXTEMOTE_BOGGLE               = 107,
    TEXTEMOTE_CALM                 = 108,
    TEXTEMOTE_COLD                 = 109,
    TEXTEMOTE_COMFORT              = 110,
    TEXTEMOTE_CUDDLE               = 111,
    TEXTEMOTE_DUCK                 = 112,
    TEXTEMOTE_INSULT               = 113,
    TEXTEMOTE_INTRODUCE            = 114,
    TEXTEMOTE_JK                   = 115,
    TEXTEMOTE_LICK                 = 116,
    TEXTEMOTE_LISTEN               = 117,
    TEXTEMOTE_LOST                 = 118,
    TEXTEMOTE_MOCK                 = 119,
    TEXTEMOTE_PONDER               = 120,
    TEXTEMOTE_POUNCE               = 121,
    TEXTEMOTE_PRAISE               = 122,
    TEXTEMOTE_PURR                 = 123,
    TEXTEMOTE_PUZZLE               = 124,
    TEXTEMOTE_RAISE                = 125,
    TEXTEMOTE_READY                = 126,
    TEXTEMOTE_SHIMMY               = 127,
    TEXTEMOTE_SHIVER               = 128,
    TEXTEMOTE_SHOO                 = 129,
    TEXTEMOTE_SLAP                 = 130,
    TEXTEMOTE_SMIRK                = 131,
    TEXTEMOTE_SNIFF                = 132,
    TEXTEMOTE_SNUB                 = 133,
    TEXTEMOTE_SOOTHE               = 134,
    TEXTEMOTE_STINK                = 135,
    TEXTEMOTE_TAUNT                = 136,
    TEXTEMOTE_TEASE                = 137,
    TEXTEMOTE_THIRSTY              = 138,
    TEXTEMOTE_VETO                 = 139,
    TEXTEMOTE_SNICKER              = 140,
    TEXTEMOTE_STAND                = 141,
    TEXTEMOTE_TICKLE               = 142,
    TEXTEMOTE_VIOLIN               = 143,
    TEXTEMOTE_SMILE                = 163,
    TEXTEMOTE_RASP                 = 183,
    TEXTEMOTE_PITY                 = 203,
    TEXTEMOTE_GROWL                = 204,
    TEXTEMOTE_BARK                 = 205,
    TEXTEMOTE_SCARED               = 223,
    TEXTEMOTE_FLOP                 = 224,
    TEXTEMOTE_LOVE                 = 225,
    TEXTEMOTE_MOO                  = 226,
    TEXTEMOTE_OPENFIRE             = 327,
    TEXTEMOTE_FLIRT                = 328,
    TEXTEMOTE_JOKE                 = 329,
    TEXTEMOTE_COMMEND              = 243,
	TEXTEMOTE_TRAIN                = 264,
    TEXTEMOTE_WINK                 = 363,
    TEXTEMOTE_PAT                  = 364,
    TEXTEMOTE_SERIOUS              = 365,
    TEXTEMOTE_MOUNTSPECIAL         = 366,
    TEXTEMOTE_GOODLUCK             = 367,
    TEXTEMOTE_BLAME                = 368,
    TEXTEMOTE_BLANK                = 369,
    TEXTEMOTE_BRANDISH             = 370,
    TEXTEMOTE_BREATH               = 371,
    TEXTEMOTE_DISAGREE             = 372,
    TEXTEMOTE_DOUBT                = 373,
    TEXTEMOTE_EMBARRASS            = 374,
    TEXTEMOTE_ENCOURAGE            = 375,
    TEXTEMOTE_ENEMY                = 376,
    TEXTEMOTE_EYEBROW              = 377,
    TEXTEMOTE_TOAST                = 378
};

enum Emote
{
    EMOTE_ONESHOT_NONE                 = 0,
    EMOTE_ONESHOT_TALK                 = 1,
    EMOTE_ONESHOT_BOW                  = 2,
    EMOTE_ONESHOT_WAVE                 = 3,
    EMOTE_ONESHOT_CHEER                = 4,
    EMOTE_ONESHOT_EXCLAMATION          = 5,
    EMOTE_ONESHOT_QUESTION             = 6,
    EMOTE_ONESHOT_EAT                  = 7,
    EMOTE_STATE_DANCE                  = 10,
    EMOTE_ONESHOT_LAUGH                = 11,
    EMOTE_STATE_SLEEP                  = 12,
    EMOTE_STATE_SIT                    = 13,
    EMOTE_ONESHOT_RUDE                 = 14,
    EMOTE_ONESHOT_ROAR                 = 15,
    EMOTE_ONESHOT_KNEEL                = 16,
    EMOTE_ONESHOT_KISS                 = 17,
    EMOTE_ONESHOT_CRY                  = 18,
    EMOTE_ONESHOT_CHICKEN              = 19,
    EMOTE_ONESHOT_BEG                  = 20,
    EMOTE_ONESHOT_APPLAUD              = 21,
    EMOTE_ONESHOT_SHOUT                = 22,
    EMOTE_ONESHOT_FLEX                 = 23,
    EMOTE_ONESHOT_SHY                  = 24,
    EMOTE_ONESHOT_POINT                = 25,
    EMOTE_STATE_STAND                  = 26,
    EMOTE_STATE_READYUNARMED           = 27,
    EMOTE_STATE_WORK                   = 28,
    EMOTE_STATE_POINT                  = 29,
    EMOTE_STATE_NONE                   = 30,
    EMOTE_ONESHOT_WOUND                = 33,
    EMOTE_ONESHOT_WOUND_CRITICAL       = 34,
    EMOTE_ONESHOT_ATTACK_UNARMED       = 35,
    EMOTE_ONESHOT_ATTACK1H             = 36,
    EMOTE_ONESHOT_ATTACK2HTIGHT        = 37,
    EMOTE_ONESHOT_ATTACK2HLOOSE        = 38,
    EMOTE_ONESHOT_PARRY_UNARMED        = 39,
    EMOTE_ONESHOT_PARRY_SHIELD         = 43,
    EMOTE_ONESHOT_READY_UNARMED        = 44,
    EMOTE_ONESHOT_READY1H              = 45,
    EMOTE_ONESHOT_READY_BOW            = 48,
    EMOTE_ONESHOT_SPELLPRECAST         = 50,
    EMOTE_ONESHOT_SPELLCAST            = 51,
    EMOTE_ONESHOT_BATTLE_ROAR          = 53,
    EMOTE_ONESHOT_SPECIALATTACK1H      = 54,
    EMOTE_ONESHOT_KICK                 = 60,
    EMOTE_ONESHOT_ATTACKTHROWN         = 61,
    EMOTE_STATE_STUN                   = 64,
    EMOTE_STATE_DEAD                   = 65,
    EMOTE_ONESHOT_SALUTE               = 66,
    EMOTE_STATE_KNEEL                  = 68,
    EMOTE_STATE_USE_STANDING           = 69,
    EMOTE_ONESHOT_WAVE_NO_SHEATHE      = 70,
    EMOTE_ONESHOT_CHEER_NO_SHEATHE     = 71,
    EMOTE_ONESHOT_EAT_NO_SHEATHE       = 92,
    EMOTE_STATE_STUN_NO_SHEATHE        = 93,
    EMOTE_ONESHOT_DANCE                = 94,
    EMOTE_ONESHOT_SALUTE_NO_SHEATH     = 113,
    EMOTE_STATE_USE_STANDING_NO_SHEATHE= 133,
    EMOTE_ONESHOT_LAUGH_NO_SHEATHE     = 153,
    EMOTE_STATE_WORK_NO_SHEATHE        = 173,
    EMOTE_STATE_SPELLPRECAST           = 193,
    EMOTE_ONESHOT_READYRIFLE           = 213,
    EMOTE_STATE_READYRIFLE             = 214,
    EMOTE_STATE_WORK_NO_SHEATHE_MINING = 233,
    EMOTE_STATE_WORK_NO_SHEATHE_CHOPWOOD= 234,
    EMOTE_zzOLDONESHOT_LIFTOFF         = 253,
    EMOTE_ONESHOT_LIFTOFF              = 254,
    EMOTE_ONESHOT_YES                  = 273,
    EMOTE_ONESHOT_NO                   = 274,
    EMOTE_ONESHOT_TRAIN                = 275,
    EMOTE_ONESHOT_LAND                 = 293,
    EMOTE_STATE_AT_EASE                = 313,
    EMOTE_STATE_READY1H                = 333,
    EMOTE_STATE_SPELLKNEELSTART        = 353,
    EMOTE_STATE_SUBMERGED              = 373,
    EMOTE_ONESHOT_SUBMERGE             = 374,
    EMOTE_STATE_READY2H                = 375,
    EMOTE_STATE_READYBOW               = 376,
    EMOTE_ONESHOT_MOUNTSPECIAL         = 377,
    EMOTE_STATE_TALK                   = 378,
    EMOTE_STATE_FISHING                = 379,
    EMOTE_ONESHOT_FISHING              = 380,
    EMOTE_ONESHOT_LOOT                 = 381,
    EMOTE_STATE_WHIRLWIND              = 382,
    EMOTE_STATE_DROWNED                = 383,
    EMOTE_STATE_HOLD_BOW               = 384,
    EMOTE_STATE_HOLD_RIFLE             = 385,
    EMOTE_STATE_HOLD_THROWN            = 386,
    EMOTE_ONESHOT_DROWN                = 387,
    EMOTE_ONESHOT_STOMP                = 388,
    EMOTE_ONESHOT_ATTACKOFF            = 389,
    EMOTE_ONESHOT_ATTACKOFFPIERCE      = 390,
    EMOTE_STATE_ROAR                   = 391,
    EMOTE_STATE_LAUGH                  = 392,
    EMOTE_ONESHOT_CREATURE_SPECIAL     = 393,
    EMOTE_ONESHOT_JUMPLANDRUN          = 394,
    EMOTE_ONESHOT_JUMPEND              = 395,
    EMOTE_ONESHOT_TALK_NO_SHEATHE      = 396,
    EMOTE_ONESHOT_POINT_NO_SHEATHE     = 397,
    EMOTE_STATE_CANNIBALIZE            = 398,
    EMOTE_ONESHOT_JUMPSTART            = 399,
    EMOTE_STATE_DANCESPECIAL           = 400,
    EMOTE_ONESHOT_DANCESPECIAL         = 401,
    EMOTE_ONESHOT_CUSTOMSPELL01        = 402,
    EMOTE_ONESHOT_CUSTOMSPELL02        = 403,
    EMOTE_ONESHOT_CUSTOMSPELL03        = 404,
    EMOTE_ONESHOT_CUSTOMSPELL04        = 405,
    EMOTE_ONESHOT_CUSTOMSPELL05        = 406,
    EMOTE_ONESHOT_CUSTOMSPELL06        = 407,
    EMOTE_ONESHOT_CUSTOMSPELL07        = 408,
    EMOTE_ONESHOT_CUSTOMSPELL08        = 409,
    EMOTE_ONESHOT_CUSTOMSPELL09        = 410,
    EMOTE_ONESHOT_CUSTOMSPELL10        = 411,
    EMOTE_STATE_EXCLAIM                = 412,
    EMOTE_STATE_SIT_CHAIR_MED          = 415,
    EMOTE_STATE_SPELLEFFECT_HOLD       = 422
};

enum Anim
{
    ANIM_STAND                     = 0x0,
    ANIM_DEATH                     = 0x1,
    ANIM_SPELL                     = 0x2,
    ANIM_STOP                      = 0x3,
    ANIM_WALK                      = 0x4,
    ANIM_RUN                       = 0x5,
    ANIM_DEAD                      = 0x6,
    ANIM_RISE                      = 0x7,
    ANIM_STANDWOUND                = 0x8,
    ANIM_COMBATWOUND               = 0x9,
    ANIM_COMBATCRITICAL            = 0xA,
    ANIM_SHUFFLE_LEFT              = 0xB,
    ANIM_SHUFFLE_RIGHT             = 0xC,
    ANIM_WALK_BACKWARDS            = 0xD,
    ANIM_STUN                      = 0xE,
    ANIM_HANDS_CLOSED              = 0xF,
    ANIM_ATTACKUNARMED             = 0x10,
    ANIM_ATTACK1H                  = 0x11,
    ANIM_ATTACK2HTIGHT             = 0x12,
    ANIM_ATTACK2HLOOSE             = 0x13,
    ANIM_PARRYUNARMED              = 0x14,
    ANIM_PARRY1H                   = 0x15,
    ANIM_PARRY2HTIGHT              = 0x16,
    ANIM_PARRY2HLOOSE              = 0x17,
    ANIM_PARRYSHIELD               = 0x18,
    ANIM_READYUNARMED              = 0x19,
    ANIM_READY1H                   = 0x1A,
    ANIM_READY2HTIGHT              = 0x1B,
    ANIM_READY2HLOOSE              = 0x1C,
    ANIM_READYBOW                  = 0x1D,
    ANIM_DODGE                     = 0x1E,
    ANIM_SPELLPRECAST              = 0x1F,
    ANIM_SPELLCAST                 = 0x20,
    ANIM_SPELLCASTAREA             = 0x21,
    ANIM_NPCWELCOME                = 0x22,
    ANIM_NPCGOODBYE                = 0x23,
    ANIM_BLOCK                     = 0x24,
    ANIM_JUMPSTART                 = 0x25,
    ANIM_JUMP                      = 0x26,
    ANIM_JUMPEND                   = 0x27,
    ANIM_FALL                      = 0x28,
    ANIM_SWIMIDLE                  = 0x29,
    ANIM_SWIM                      = 0x2A,
    ANIM_SWIM_LEFT                 = 0x2B,
    ANIM_SWIM_RIGHT                = 0x2C,
    ANIM_SWIM_BACKWARDS            = 0x2D,
    ANIM_ATTACKBOW                 = 0x2E,
    ANIM_FIREBOW                   = 0x2F,
    ANIM_READYRIFLE                = 0x30,
    ANIM_ATTACKRIFLE               = 0x31,
    ANIM_LOOT                      = 0x32,
    ANIM_SPELL_PRECAST_DIRECTED    = 0x33,
    ANIM_SPELL_PRECAST_OMNI        = 0x34,
    ANIM_SPELL_CAST_DIRECTED       = 0x35,
    ANIM_SPELL_CAST_OMNI           = 0x36,
    ANIM_SPELL_BATTLEROAR          = 0x37,
    ANIM_SPELL_READYABILITY        = 0x38,
    ANIM_SPELL_SPECIAL1H           = 0x39,
    ANIM_SPELL_SPECIAL2H           = 0x3A,
    ANIM_SPELL_SHIELDBASH          = 0x3B,
    ANIM_EMOTE_TALK                = 0x3C,
    ANIM_EMOTE_EAT                 = 0x3D,
    ANIM_EMOTE_WORK                = 0x3E,
    ANIM_EMOTE_USE_STANDING        = 0x3F,
    ANIM_EMOTE_EXCLAMATION         = 0x40,
    ANIM_EMOTE_QUESTION            = 0x41,
    ANIM_EMOTE_BOW                 = 0x42,
    ANIM_EMOTE_WAVE                = 0x43,
    ANIM_EMOTE_CHEER               = 0x44,
    ANIM_EMOTE_DANCE               = 0x45,
    ANIM_EMOTE_LAUGH               = 0x46,
    ANIM_EMOTE_SLEEP               = 0x47,
    ANIM_EMOTE_SIT_GROUND          = 0x48,
    ANIM_EMOTE_RUDE                = 0x49,
    ANIM_EMOTE_ROAR                = 0x4A,
    ANIM_EMOTE_KNEEL               = 0x4B,
    ANIM_EMOTE_KISS                = 0x4C,
    ANIM_EMOTE_CRY                 = 0x4D,
    ANIM_EMOTE_CHICKEN             = 0x4E,
    ANIM_EMOTE_BEG                 = 0x4F,
    ANIM_EMOTE_APPLAUD             = 0x50,
    ANIM_EMOTE_SHOUT               = 0x51,
    ANIM_EMOTE_FLEX                = 0x52,
    ANIM_EMOTE_SHY                 = 0x53,
    ANIM_EMOTE_POINT               = 0x54,
    ANIM_ATTACK1HPIERCE            = 0x55,
    ANIM_ATTACK2HLOOSEPIERCE       = 0x56,
    ANIM_ATTACKOFF                 = 0x57,
    ANIM_ATTACKOFFPIERCE           = 0x58,
    ANIM_SHEATHE                   = 0x59,
    ANIM_HIPSHEATHE                = 0x5A,
    ANIM_MOUNT                     = 0x5B,
    ANIM_RUN_LEANRIGHT             = 0x5C,
    ANIM_RUN_LEANLEFT              = 0x5D,
    ANIM_MOUNT_SPECIAL             = 0x5E,
    ANIM_KICK                      = 0x5F,
    ANIM_SITDOWN                   = 0x60,
    ANIM_SITTING                   = 0x61,
    ANIM_SITUP                     = 0x62,
    ANIM_SLEEPDOWN                 = 0x63,
    ANIM_SLEEPING                  = 0x64,
    ANIM_SLEEPUP                   = 0x65,
    ANIM_SITCHAIRLOW               = 0x66,
    ANIM_SITCHAIRMEDIUM            = 0x67,
    ANIM_SITCHAIRHIGH              = 0x68,
    ANIM_LOADBOW                   = 0x69,
    ANIM_LOADRIFLE                 = 0x6A,
    ANIM_ATTACKTHROWN              = 0x6B,
    ANIM_READYTHROWN               = 0x6C,
    ANIM_HOLDBOW                   = 0x6D,
    ANIM_HOLDRIFLE                 = 0x6E,
    ANIM_HOLDTHROWN                = 0x6F,
    ANIM_LOADTHROWN                = 0x70,
    ANIM_EMOTE_SALUTE              = 0x71,
    ANIM_KNEELDOWN                 = 0x72,
    ANIM_KNEELING                  = 0x73,
    ANIM_KNEELUP                   = 0x74,
    ANIM_ATTACKUNARMEDOFF          = 0x75,
    ANIM_SPECIALUNARMED            = 0x76,
    ANIM_STEALTHWALK               = 0x77,
    ANIM_STEALTHSTAND              = 0x78,
    ANIM_KNOCKDOWN                 = 0x79,
    ANIM_EATING                    = 0x7A,
    ANIM_USESTANDINGLOOP           = 0x7B,
    ANIM_CHANNELCASTDIRECTED       = 0x7C,
    ANIM_CHANNELCASTOMNI           = 0x7D,
    ANIM_WHIRLWIND                 = 0x7E,
    ANIM_BIRTH                     = 0x7F,
    ANIM_USESTANDINGSTART          = 0x80,
    ANIM_USESTANDINGEND            = 0x81,
    ANIM_HOWL                      = 0x82,
    ANIM_DROWN                     = 0x83,
    ANIM_DROWNED                   = 0x84,
    ANIM_FISHINGCAST               = 0x85,
    ANIM_FISHINGLOOP               = 0x86,
    ANIM_FLY                       = 0x87,
    ANIM_EMOTE_WORK_NO_SHEATHE     = 0x88,
    ANIM_EMOTE_STUN_NO_SHEATHE     = 0x89,
    ANIM_EMOTE_USE_STANDING_NO_SHEATHE= 0x8A,
    ANIM_SPELL_SLEEP_DOWN          = 0x8B,
    ANIM_SPELL_KNEEL_START         = 0x8C,
    ANIM_SPELL_KNEEL_LOOP          = 0x8D,
    ANIM_SPELL_KNEEL_END           = 0x8E,
    ANIM_SPRINT                    = 0x8F,
    ANIM_IN_FIGHT                  = 0x90,

    ANIM_GAMEOBJ_SPAWN             = 145,
    ANIM_GAMEOBJ_CLOSE             = 146,
    ANIM_GAMEOBJ_CLOSED            = 147,
    ANIM_GAMEOBJ_OPEN              = 148,
    ANIM_GAMEOBJ_OPENED            = 149,
    ANIM_GAMEOBJ_DESTROY           = 150,
    ANIM_GAMEOBJ_DESTROYED         = 151,
    ANIM_GAMEOBJ_REBUILD           = 152,
    ANIM_GAMEOBJ_CUSTOM0           = 153,
    ANIM_GAMEOBJ_CUSTOM1           = 154,
    ANIM_GAMEOBJ_CUSTOM2           = 155,
    ANIM_GAMEOBJ_CUSTOM3           = 156,
    ANIM_GAMEOBJ_DESPAWN           = 157,
    ANIM_HOLD                      = 158,
    ANIM_DECAY                     = 159,
    ANIM_BOWPULL                   = 160,
    ANIM_BOWRELEASE                = 161,
    ANIM_SHIPSTART                 = 162,
    ANIM_SHIPMOVEING               = 163,
    ANIM_SHIPSTOP                  = 164,
    ANIM_GROUPARROW                = 165,
    ANIM_ARROW                     = 166,
    ANIM_CORPSEARROW               = 167,
    ANIM_GUIDEARROW                = 168,
    ANIM_SWAY                      = 169,
    ANIM_DRUIDCATPOUNCE            = 170,
    ANIM_DRUIDCATRIP               = 171,
    ANIM_DRUIDCATRAKE              = 172,
    ANIM_DRUIDCATRAVAGE            = 173,
    ANIM_DRUIDCATCLAW              = 174,
    ANIM_DRUIDCATCOWER             = 175,
    ANIM_DRUIDBEARSWIPE            = 176,
    ANIM_DRUIDBEARBITE             = 177,
    ANIM_DRUIDBEARMAUL             = 178,
    ANIM_DRUIDBEARBASH             = 179,
    ANIM_DRAGONTAIL                = 180,
    ANIM_DRAGONSTOMP               = 181,
    ANIM_DRAGONSPIT                = 182,
    ANIM_DRAGONSPITHOVER           = 183,
    ANIM_DRAGONSPITFLY             = 184,
    ANIM_EMOTEYES                  = 185,
    ANIM_EMOTENO                   = 186,
    ANIM_JUMPLANDRUN               = 187,
    ANIM_LOOTHOLD                  = 188,
    ANIM_LOOTUP                    = 189,
    ANIM_STANDHIGH                 = 190,
    ANIM_IMPACT                    = 191,
    ANIM_LIFTOFF                   = 192,
    ANIM_HOVER                     = 193,
    ANIM_SUCCUBUSENTICE            = 194,
    ANIM_EMOTETRAIN                = 195,
    ANIM_EMOTEDEAD                 = 196,
    ANIM_EMOTEDANCEONCE            = 197,
    ANIM_DEFLECT                   = 198,
    ANIM_EMOTEEATNOSHEATHE         = 199,
    ANIM_LAND                      = 200,
    ANIM_SUBMERGE                  = 201,
    ANIM_SUBMERGED                 = 202,
    ANIM_CANNIBALIZE               = 203,
    ANIM_ARROWBIRTH                = 204,
    ANIM_GROURARROWBIRTH           = 205,
    ANIM_CORPSEARROWBIRTH          = 206,
    ANIM_GUIDEARROWBIRTH           = 207,
    ANIM_EMOTETALKNOSHEATHE        = 208,
    ANIM_EMOTEPOINTNOSHEATHE       = 209,
    ANIM_EMOTESALUTENOSHEATHE      = 210,
    ANIM_EMOTEDANCESPECIAL         = 211,
    ANIM_MUTILATE                  = 212,
    ANIM_CUSTOMSPELL01             = 213,
    ANIM_CUSTOMSPELL02             = 214,
    ANIM_CUSTOMSPELL03             = 215,
    ANIM_CUSTOMSPELL04             = 216,
    ANIM_CUSTOMSPELL05             = 217,
    ANIM_CUSTOMSPELL06             = 218,
    ANIM_CUSTOMSPELL07             = 219,
    ANIM_CUSTOMSPELL08             = 220,
    ANIM_CUSTOMSPELL09             = 221,
    ANIM_CUSTOMSPELL10             = 222,
    ANIM_StealthRun                = 223,
        //ANIM_EMERGE? found it in decompiled client
        //ANIM_COWER? found it in decompiled client
};

enum LockKeyType
{
    LOCK_KEY_NONE  = 0,
    LOCK_KEY_ITEM  = 1,
    LOCK_KEY_SKILL = 2
};

enum LockType
{
    LOCKTYPE_PICKLOCK              = 1,
    LOCKTYPE_HERBALISM             = 2,
    LOCKTYPE_MINING                = 3,
    LOCKTYPE_DISARM_TRAP           = 4,
    LOCKTYPE_OPEN                  = 5,
    LOCKTYPE_TREASURE              = 6,
    LOCKTYPE_CALCIFIED_ELVEN_GEMS  = 7,
    LOCKTYPE_CLOSE                 = 8,
    LOCKTYPE_ARM_TRAP              = 9,
    LOCKTYPE_QUICK_OPEN            = 10,
    LOCKTYPE_QUICK_CLOSE           = 11,
    LOCKTYPE_OPEN_TINKERING        = 12,
    LOCKTYPE_OPEN_KNEELING         = 13,
    LOCKTYPE_OPEN_ATTACKING        = 14,
    LOCKTYPE_GAHZRIDIAN            = 15,
    LOCKTYPE_BLASTING              = 16,
    LOCKTYPE_SLOW_OPEN             = 17,
    LOCKTYPE_SLOW_CLOSE            = 18,
    LOCKTYPE_FISHING               = 19
};

enum CreatureType
{
    CREATURE_TYPE_BEAST            = 1,
    CREATURE_TYPE_DRAGONKIN        = 2,
    CREATURE_TYPE_DEMON            = 3,
    CREATURE_TYPE_ELEMENTAL        = 4,
    CREATURE_TYPE_GIANT            = 5,
    CREATURE_TYPE_UNDEAD           = 6,
    CREATURE_TYPE_HUMANOID         = 7,
    CREATURE_TYPE_CRITTER          = 8,
    CREATURE_TYPE_MECHANICAL       = 9,
    CREATURE_TYPE_NOT_SPECIFIED    = 10,
    CREATURE_TYPE_TOTEM            = 11,
    CREATURE_TYPE_NON_COMBAT_PET   = 12,
    CREATURE_TYPE_GAS_CLOUD        = 13
};

uint32 const CREATURE_TYPEMASK_HUMANOID_OR_UNDEAD = (1 << (CREATURE_TYPE_HUMANOID-1)) | (1 << (CREATURE_TYPE_UNDEAD-1));

enum CreatureFamily
{
    CREATURE_FAMILY_NONE           = 0,
    CREATURE_FAMILY_WOLF           = 1,
    CREATURE_FAMILY_CAT            = 2,
    CREATURE_FAMILY_SPIDER         = 3,
    CREATURE_FAMILY_BEAR           = 4,
    CREATURE_FAMILY_BOAR           = 5,
    CREATURE_FAMILY_CROCOLISK      = 6,
    CREATURE_FAMILY_CARRION_BIRD   = 7,
    CREATURE_FAMILY_CRAB           = 8,
    CREATURE_FAMILY_GORILLA        = 9,
    CREATURE_FAMILY_RAPTOR         = 11,
    CREATURE_FAMILY_TALLSTRIDER    = 12,
    CREATURE_FAMILY_FELHUNTER      = 15,
    CREATURE_FAMILY_VOIDWALKER     = 16,
    CREATURE_FAMILY_SUCCUBUS       = 17,
    CREATURE_FAMILY_DOOMGUARD      = 19,
    CREATURE_FAMILY_SCORPID        = 20,
    CREATURE_FAMILY_TURTLE         = 21,
    CREATURE_FAMILY_IMP            = 23,
    CREATURE_FAMILY_BAT            = 24,
    CREATURE_FAMILY_HYENA          = 25,
    CREATURE_FAMILY_OWL            = 26,
    CREATURE_FAMILY_WIND_SERPENT   = 27,
    CREATURE_FAMILY_REMOTE_CONTROL = 28,
    CREATURE_FAMILY_FELGUARD       = 29,
    CREATURE_FAMILY_DRAGONHAWK     = 30,
    CREATURE_FAMILY_RAVAGER        = 31,
    CREATURE_FAMILY_WARP_STALKER   = 32,
    CREATURE_FAMILY_SPOREBAT       = 33,
    CREATURE_FAMILY_NETHER_RAY     = 34,
    CREATURE_FAMILY_SERPENT        = 35,
    CREATURE_FAMILY_SEA_LION       = 36,
#ifdef LICH_KING
    CREATURE_FAMILY_MOTH           = 37,
    CREATURE_FAMILY_CHIMAERA       = 38,
    CREATURE_FAMILY_DEVILSAUR      = 39,
    CREATURE_FAMILY_GHOUL          = 40,
    CREATURE_FAMILY_SILITHID       = 41,
    CREATURE_FAMILY_WORM           = 42,
    CREATURE_FAMILY_RHINO          = 43,
    CREATURE_FAMILY_WASP           = 44,
    CREATURE_FAMILY_CORE_HOUND     = 45,
    CREATURE_FAMILY_SPIRIT_BEAST   = 46,
#endif
};

enum CreatureTypeFlags
{
    CREATURE_TYPE_FLAG_TAMEABLE_PET                         = 0x0001, // Makes the mob tameable (must also be a beast and have family set)
    CREATURE_TYPE_FLAG_GHOST_VISIBLE                        = 0x0002, // Creature are also visible for not alive player. Allow gossip interaction if npcflag allow?
    CREATURE_TYPE_FLAG_BOSS_MOB                             = 0x0004, // Changes creature's visible level to "??" in the creature's portrait - Immune Knockback.
    CREATURE_TYPE_FLAG_DO_NOT_PLAY_WOUND_PARRY_ANIMATION    = 0x0008,
    CREATURE_TYPE_FLAG_HIDE_FACTION_TOOLTIP                 = 0x0010,
    CREATURE_TYPEFLAGS_UNK5                                 = 0x0020, // Sound related
    CREATURE_TYPE_FLAG_SPELL_ATTACKABLE                     = 0x0040,
    CREATURE_TYPE_FLAG_CAN_INTERACT_WHILE_DEAD              = 0x0080,       // Player can interact with the creature if its dead (not player dead)
    CREATURE_TYPE_FLAG_HERB_SKINNING_SKILL                  = 0x0100,       // Can be looted by herbalist
    CREATURE_TYPE_FLAG_MINING_SKINNING_SKILL                = 0x0200,       // Can be looted by miner
    CREATURE_TYPE_FLAG_DO_NOT_LOG_DEATH                     = 0x0400,       // Death event will not show up in combat log
    CREATURE_TYPE_FLAG_MOUNTED_COMBAT_ALLOWED               = 0x0800,       // Creature can remain mounted when entering combat
    CREATURE_TYPE_FLAG_CAN_ASSIST                           = 0x01000,       // Assist player in combat if in range
#ifdef LICH_KING
  CREATURE_TYPE_FLAG_IS_PET_BAR_USED                        = 0x00002000,
    CREATURE_TYPE_FLAG_MASK_UID                             = 0x00004000,
    CREATURE_TYPE_FLAG_ENGINEERING_SKINNING_SKILL           = 0x00008000,   // Can be looted by engineer
    CREATURE_TYPE_FLAG_EXOTIC_PET                           = 0x00010000,   // Can be tamed by hunter as exotic pet
    CREATURE_TYPE_FLAG_USE_DEFAULT_COLLISION_BOX            = 0x00020000,   // Collision related. (always using default collision box?)
    CREATURE_TYPE_FLAG_IS_SIEGE_WEAPON                      = 0x00040000,
    CREATURE_TYPE_FLAG_CAN_COLLIDE_WITH_MISSILES            = 0x00080000,   // Projectiles can collide with this creature - interacts with TARGET_DEST_TRAJ
    CREATURE_TYPE_FLAG_HIDE_NAME_PLATE                      = 0x00100000,
    CREATURE_TYPE_FLAG_DO_NOT_PLAY_MOUNTED_ANIMATIONS       = 0x00200000,
    CREATURE_TYPE_FLAG_IS_LINK_ALL                          = 0x00400000,
    CREATURE_TYPE_FLAG_INTERACT_ONLY_WITH_CREATOR           = 0x00800000,
    CREATURE_TYPE_FLAG_DO_NOT_PLAY_UNIT_EVENT_SOUNDS        = 0x01000000,
    CREATURE_TYPE_FLAG_HAS_NO_SHADOW_BLOB                   = 0x02000000,
    CREATURE_TYPE_FLAG_TREAT_AS_RAID_UNIT                   = 0x04000000,   // ! Creature can be targeted by spells that require target to be in caster's party/raid
    CREATURE_TYPE_FLAG_FORCE_GOSSIP                         = 0x08000000,   // Allows the creature to display a single gossip option.
    CREATURE_TYPE_FLAG_DO_NOT_SHEATHE                       = 0x10000000,
    CREATURE_TYPE_FLAG_DO_NOT_TARGET_ON_INTERACTION         = 0x20000000,
    CREATURE_TYPE_FLAG_DO_NOT_RENDER_OBJECT_NAME            = 0x40000000,
    CREATURE_TYPE_FLAG_UNIT_IS_QUEST_BOSS                   = 0x80000000    // Not verified
#endif
};

enum CreatureEliteType
{
    CREATURE_ELITE_NORMAL          = 0,
    CREATURE_ELITE_ELITE           = 1,
    CREATURE_ELITE_RAREELITE       = 2,
    CREATURE_ELITE_WORLDBOSS       = 3,
    CREATURE_ELITE_RARE            = 4,
    CREATURE_UNKNOWN               = 5                      // found in 2.2.3 for 2 mobs
};

// Use holiday id in WLK. values based at Holidays.dbc
enum HolidayIds
{
    HOLIDAY_NONE                     = 0,

    HOLIDAY_FIREWORKS_SPECTACULAR    = 62,
    HOLIDAY_FEAST_OF_WINTER_VEIL     = 141,
    HOLIDAY_NOBLEGARDEN              = 181,
    HOLIDAY_CHILDRENS_WEEK           = 201,
    HOLIDAY_CALL_TO_ARMS_AV          = 283,
    HOLIDAY_CALL_TO_ARMS_WS          = 284,
    HOLIDAY_CALL_TO_ARMS_AB          = 285,
    HOLIDAY_FISHING_EXTRAVAGANZA     = 301,
    HOLIDAY_HARVEST_FESTIVAL         = 321,
    HOLIDAY_HALLOWS_END              = 324,
    HOLIDAY_LUNAR_FESTIVAL           = 327,
    // HOLIDAY_LOVE_IS_IN_THE_AIR    = 335, unused/duplicated
    HOLIDAY_FIRE_FESTIVAL            = 341,
    HOLIDAY_CALL_TO_ARMS_EY          = 353,
    HOLIDAY_BREWFEST                 = 372,
    HOLIDAY_DARKMOON_FAIRE_ELWYNN    = 374,
    HOLIDAY_DARKMOON_FAIRE_THUNDER   = 375,
    HOLIDAY_DARKMOON_FAIRE_SHATTRATH = 376,
    HOLIDAY_PIRATES_DAY              = 398,
    HOLIDAY_CALL_TO_ARMS_SA          = 400,
    HOLIDAY_PILGRIMS_BOUNTY          = 404,
    HOLIDAY_WOTLK_LAUNCH             = 406,
    HOLIDAY_DAY_OF_DEAD              = 409,
    HOLIDAY_CALL_TO_ARMS_IC          = 420,
    HOLIDAY_LOVE_IS_IN_THE_AIR       = 423,
    HOLIDAY_KALU_AK_FISHING_DERBY    = 424
};

// values based at QuestInfo.dbc
enum QuestTypes
{
    QUEST_TYPE_ELITE               = 1,
    QUEST_TYPE_LIFE                = 21,
    QUEST_TYPE_PVP                 = 41,
    QUEST_TYPE_RAID                = 62,
    QUEST_TYPE_DUNGEON             = 81,
    QUEST_TYPE_WORLD_EVENT         = 82,
    QUEST_TYPE_LEGENDARY           = 83,
    QUEST_TYPE_ESCORT              = 84,
    QUEST_TYPE_HEROIC              = 85, //last in QuestInfo.dbc for TBC
#ifdef LICH_KING
    QUEST_TYPE_RAID_10             = 88,
    QUEST_TYPE_RAID_25             = 89
#endif
};

// values based at QuestSort.dbc
enum QuestSort
{
    QUEST_SORT_EPIC                = 1,
    QUEST_SORT_WAILING_CAVERNS_OLD = 21,
    QUEST_SORT_SEASONAL            = 22,
    QUEST_SORT_UNDERCITY_OLD       = 23,
    QUEST_SORT_HERBALISM           = 24,
    QUEST_SORT_SCARLET_MONASTERY_OLD= 25,
    QUEST_SORT_ULDAMN_OLD          = 41,
    QUEST_SORT_WARLOCK             = 61,
    QUEST_SORT_WARRIOR             = 81,
    QUEST_SORT_SHAMAN              = 82,
    QUEST_SORT_FISHING             = 101,
    QUEST_SORT_BLACKSMITHING       = 121,
    QUEST_SORT_PALADIN             = 141,
    QUEST_SORT_MAGE                = 161,
    QUEST_SORT_ROGUE               = 162,
    QUEST_SORT_ALCHEMY             = 181,
    QUEST_SORT_LEATHERWORKING      = 182,
    QUEST_SORT_ENGINERING          = 201,
    QUEST_SORT_TREASURE_MAP        = 221,
    QUEST_SORT_SUNKEN_TEMPLE_OLD   = 241,
    QUEST_SORT_HUNTER              = 261,
    QUEST_SORT_PRIEST              = 262,
    QUEST_SORT_DRUID               = 263,
    QUEST_SORT_TAILORING           = 264,
    QUEST_SORT_SPECIAL             = 284,
    QUEST_SORT_COOKING             = 304,
    QUEST_SORT_FIRST_AID           = 324,
    QUEST_SORT_LEGENDARY           = 344,
    QUEST_SORT_DARKMOON_FAIRE      = 364,
    QUEST_SORT_AHN_QIRAJ_WAR       = 365,
    QUEST_SORT_LUNAR_FESTIVAL      = 366,
    QUEST_SORT_REPUTATION          = 367,
    QUEST_SORT_INVASION            = 368,
    QUEST_SORT_MIDSUMMER           = 369,
    QUEST_SORT_BREWFEST            = 370, //last entry in QuestSort.dbc for tbc
#ifdef LICH_KING
    QUEST_SORT_INSCRIPTION         = 371,
    QUEST_SORT_DEATH_KNIGHT        = 372,
    QUEST_SORT_JEWELCRAFTING       = 373,
    QUEST_SORT_NOBLEGARDEN         = 374,
    QUEST_SORT_PILGRIMS_BOUNTY     = 375,
    QUEST_SORT_LOVE_IS_IN_THE_AIR  = 376, //event was present before LK but wans't yet present in QuestSort.dbc for some reason
#endif
};

constexpr uint8 ClassByQuestSort(int32 QuestSort)
{
    switch(QuestSort)
    {
        case QUEST_SORT_WARLOCK: return CLASS_WARLOCK;
        case QUEST_SORT_WARRIOR: return CLASS_WARRIOR;
        case QUEST_SORT_SHAMAN:  return CLASS_SHAMAN;
        case QUEST_SORT_PALADIN: return CLASS_PALADIN;
        case QUEST_SORT_MAGE:    return CLASS_MAGE;
        case QUEST_SORT_ROGUE:   return CLASS_ROGUE;
        case QUEST_SORT_HUNTER:  return CLASS_HUNTER;
        case QUEST_SORT_PRIEST:  return CLASS_PRIEST;
        case QUEST_SORT_DRUID:   return CLASS_DRUID;
    }
    return 0;
}

enum SkillType
{
    SKILL_NONE                     = 0,

    SKILL_FROST                    = 6,
    SKILL_FIRE                     = 8,
    SKILL_ARMS                     = 26,
    SKILL_COMBAT                   = 38,
    SKILL_SUBTLETY                 = 39,
    SKILL_POISONS                  = 40,
    SKILL_SWORDS                   = 43,
    SKILL_AXES                     = 44,
    SKILL_BOWS                     = 45,
    SKILL_GUNS                     = 46,
    SKILL_BEAST_MASTERY            = 50,
    SKILL_SURVIVAL                 = 51,
    SKILL_MACES                    = 54,
    SKILL_HOLY                     = 56,
    SKILL_2H_SWORDS                = 55,
    SKILL_SHADOW                   = 78,
    SKILL_DEFENSE                  = 95,
    SKILL_LANG_COMMON              = 98,
    SKILL_RACIAL_DWARVEN           = 101,
    SKILL_LANG_ORCISH              = 109,
    SKILL_LANG_DWARVEN             = 111,
    SKILL_LANG_DARNASSIAN          = 113,
    SKILL_LANG_TAURAHE             = 115,
    SKILL_DUAL_WIELD               = 118,
    SKILL_RACIAL_TAUREN            = 124,
    SKILL_ORC_RACIAL               = 125,
    SKILL_RACIAL_NIGHT_ELF         = 126,
    SKILL_FIRST_AID                = 129,
    SKILL_FERAL_COMBAT             = 134,
    SKILL_STAVES                   = 136,
    SKILL_LANG_THALASSIAN          = 137,
    SKILL_LANG_DRACONIC            = 138,
    SKILL_LANG_DEMON_TONGUE        = 139,
    SKILL_LANG_TITAN               = 140,
    SKILL_LANG_OLD_TONGUE          = 141,
    SKILL_SURVIVAL2                = 142,
    SKILL_RIDING_HORSE             = 148,
    SKILL_RIDING_WOLF              = 149,
    SKILL_RIDING_RAM               = 152,
    SKILL_RIDING_TIGER             = 150,
    SKILL_SWIMING                  = 155,
    SKILL_2H_MACES                 = 160,
    SKILL_UNARMED                  = 162,
    SKILL_MARKSMANSHIP             = 163,
    SKILL_BLACKSMITHING            = 164,
    SKILL_LEATHERWORKING           = 165,
    SKILL_ALCHEMY                  = 171,
    SKILL_2H_AXES                  = 172,
    SKILL_DAGGERS                  = 173,
    SKILL_THROWN                   = 176,
    SKILL_HERBALISM                = 182,
    SKILL_GENERIC_DND              = 183,
    SKILL_RETRIBUTION              = 184,
    SKILL_COOKING                  = 185,
    SKILL_MINING                   = 186,
    SKILL_PET_IMP                  = 188,
    SKILL_PET_FELHUNTER            = 189,
    SKILL_TAILORING                = 197,
    SKILL_ENGINEERING               = 202,
    SKILL_PET_SPIDER               = 203,
    SKILL_PET_VOIDWALKER           = 204,
    SKILL_PET_SUCCUBUS             = 205,
    SKILL_PET_INFERNAL             = 206,
    SKILL_PET_DOOMGUARD            = 207,
    SKILL_PET_WOLF                 = 208,
    SKILL_PET_CAT                  = 209,
    SKILL_PET_BEAR                 = 210,
    SKILL_PET_BOAR                 = 211,
    SKILL_PET_CROCILISK            = 212,
    SKILL_PET_CARRION_BIRD         = 213,
    SKILL_PET_GORILLA              = 215,
    SKILL_PET_CRAB                 = 214,
    SKILL_PET_RAPTOR               = 217,
    SKILL_PET_TALLSTRIDER          = 218,
    SKILL_RACIAL_UNDED             = 220,
    SKILL_WEAPON_TALENTS           = 222,
    SKILL_CROSSBOWS                = 226,
    SKILL_SPEARS                   = 227,
    SKILL_WANDS                    = 228,
    SKILL_POLEARMS                 = 229,
    SKILL_PET_SCORPID              = 236,
    SKILL_ARCANE                   = 237,
    SKILL_OPEN_LOCK                = 242,
    SKILL_PET_TURTLE               = 251,
    SKILL_ASSASSINATION            = 253,
    SKILL_FURY                     = 256,
    SKILL_PROTECTION               = 257,
    SKILL_BEAST_TRAINING           = 261,
    SKILL_PROTECTION2              = 267,
    SKILL_PET_TALENTS              = 270,
    SKILL_PLATE_MAIL               = 293,
    SKILL_LANG_GNOMISH             = 313,
    SKILL_LANG_TROLL               = 315,
    SKILL_ENCHANTING               = 333,
    SKILL_DEMONOLOGY               = 354,
    SKILL_AFFLICTION               = 355,
    SKILL_FISHING                  = 356,
    SKILL_ENHANCEMENT              = 373,
    SKILL_RESTORATION              = 374,
    SKILL_ELEMENTAL_COMBAT         = 375,
    SKILL_SKINNING                 = 393,
    SKILL_MAIL                     = 413,
    SKILL_LEATHER                  = 414,
    SKILL_CLOTH                    = 415,
    SKILL_SHIELD                   = 433,
    SKILL_FIST_WEAPONS             = 473,
    SKILL_RIDING_RAPTOR            = 533,
    SKILL_RIDING_MECHANOSTRIDER    = 553,
    SKILL_RIDING_UNDEAD_HORSE      = 554,
    SKILL_RESTORATION2             = 573,
    SKILL_BALANCE                  = 574,
    SKILL_DESTRUCTION              = 593,
    SKILL_HOLY2                    = 594,
    SKILL_DISCIPLINE               = 613,
    SKILL_LOCKPICKING              = 633,
    SKILL_PET_BAT                  = 653,
    SKILL_PET_HYENA                = 654,
    SKILL_PET_OWL                  = 655,
    SKILL_PET_WIND_SERPENT         = 656,
    SKILL_LANG_GUTTERSPEAK         = 673,
    SKILL_RIDING_KODO              = 713,
    SKILL_RACIAL_TROLL             = 733,
    SKILL_RACIAL_GNOME             = 753,
    SKILL_RACIAL_HUMAN             = 754,
    SKILL_JEWELCRAFTING            = 755,
    SKILL_RACIAL_BLOODELF          = 756,
    SKILL_PET_EVENT_RC             = 758,
    SKILL_LANG_DRAENEI             = 759,
    SKILL_RACIAL_DRAENEI           = 760,
    SKILL_PET_FELGUARD             = 761,
    SKILL_RIDING                   = 762,
    SKILL_PET_DRAGONHAWK           = 763,
    SKILL_PET_NETHER_RAY           = 764,
    SKILL_PET_SPOREBAT             = 765,
    SKILL_PET_WARP_STALKER         = 766,
    SKILL_PET_RAVAGER              = 767,
    SKILL_PET_SERPENT              = 768,
    SKILL_INTERNAL                 = 769,

#ifdef LICH_KING
    SKILL_DK_BLOOD                 = 770,
    SKILL_DK_FROST                 = 771,
    SKILL_DK_UNHOLY                = 772,
    SKILL_INSCRIPTION              = 773,
    SKILL_PET_MOTH                 = 775,
    SKILL_RUNEFORGING              = 776,
    SKILL_MOUNTS                   = 777,
    SKILL_COMPANIONS               = 778,
    SKILL_PET_EXOTIC_CHIMAERA      = 780,
    SKILL_PET_EXOTIC_DEVILSAUR     = 781,
    SKILL_PET_GHOUL                = 782,
    SKILL_PET_EXOTIC_SILITHID      = 783,
    SKILL_PET_EXOTIC_WORM          = 784,
    SKILL_PET_WASP                 = 785,
    SKILL_PET_EXOTIC_RHINO         = 786,
    SKILL_PET_EXOTIC_CORE_HOUND    = 787,
    SKILL_PET_EXOTIC_SPIRIT_BEAST  = 788,
#endif

    MAX_SKILL_TYPE
};

constexpr SkillType SkillByLockType(LockType locktype)
{
    switch (locktype)
    {
        case LOCKTYPE_PICKLOCK:    return SKILL_LOCKPICKING;
        case LOCKTYPE_HERBALISM:   return SKILL_HERBALISM;
        case LOCKTYPE_MINING:      return SKILL_MINING;
        case LOCKTYPE_FISHING:     return SKILL_FISHING;
#ifdef LICH_KING
        case LOCKTYPE_INSCRIPTION: return SKILL_INSCRIPTION;
#endif
        default: break;
    }
    return SKILL_NONE;
}

constexpr uint32 SkillByQuestSort(int32 QuestSort)
{
    switch(QuestSort)
    {
        case QUEST_SORT_HERBALISM:      return SKILL_HERBALISM;
        case QUEST_SORT_FISHING:        return SKILL_FISHING;
        case QUEST_SORT_BLACKSMITHING:  return SKILL_BLACKSMITHING;
        case QUEST_SORT_ALCHEMY:        return SKILL_ALCHEMY;
        case QUEST_SORT_LEATHERWORKING: return SKILL_LEATHERWORKING;
        case QUEST_SORT_ENGINERING:     return SKILL_ENGINEERING;
        case QUEST_SORT_TAILORING:      return SKILL_TAILORING;
        case QUEST_SORT_COOKING:        return SKILL_COOKING;
        case QUEST_SORT_FIRST_AID:      return SKILL_FIRST_AID;
    }
    return 0;
}

enum SkillCategory
{
    SKILL_CATEGORY_ATTRIBUTES    =  5,
    SKILL_CATEGORY_WEAPON        =  6,
    SKILL_CATEGORY_CLASS         =  7,
    SKILL_CATEGORY_ARMOR         =  8,
    SKILL_CATEGORY_SECONDARY     =  9,                      // secondary professions
    SKILL_CATEGORY_LANGUAGES     = 10,
    SKILL_CATEGORY_PROFESSION    = 11,                      // primary professions
    SKILL_CATEGORY_NOT_DISPLAYED = 12
};

enum TotemCategory
{
    TC_SKINNING_KNIFE              = 1,
    TC_EARTH_TOTEM                 = 2,
    TC_AIR_TOTEM                   = 3,
    TC_FIRE_TOTEM                  = 4,
    TC_WATER_TOTEM                 = 5,
    TC_COPPER_ROD                  = 6,
    TC_SILVER_ROD                  = 7,
    TC_GOLDEN_ROD                  = 8,
    TC_TRUESILVER_ROD              = 9,
    TC_ARCANITE_ROD                = 10,
    TC_MINING_PICK                 = 11,
    TC_PHILOSOPHERS_STONE          = 12,
    TC_BLACKSMITH_HAMMER           = 13,
    TC_ARCLIGHT_SPANNER            = 14,
    TC_GYROMATIC_MA                = 15,
    TC_MASTER_TOTEM                = 21,
    TC_FEL_IRON_ROD                = 41,
    TC_ADAMANTITE_ROD              = 62,
    TC_ETERNIUM_ROD                = 63
};

enum UnitDynFlags
{
    UNIT_DYNFLAG_NONE              = 0x0000,
    UNIT_DYNFLAG_LOOTABLE          = 0x0001,
    UNIT_DYNFLAG_TRACK_UNIT        = 0x0002,
    UNIT_DYNFLAG_TAPPED            = 0x0004,  // Lua_UnitIsTapped
    UNIT_DYNFLAG_TAPPED_BY_PLAYER  = 0x0008,  // Lua_UnitIsTappedByPlayer
    UNIT_DYNFLAG_SPECIALINFO       = 0x0010,
    UNIT_DYNFLAG_DEAD              = 0x0020,  // Lua_UnitIsTappedByAllThreatList
    //LK only ?
    UNIT_DYNFLAG_REFER_A_FRIEND             = 0x0040,
    UNIT_DYNFLAG_TAPPED_BY_ALL_THREAT_LIST  = 0x0080,        // Lua_UnitIsTappedByAllThreatList
};

enum CorpseDynFlags
{
    CORPSE_DYNFLAG_LOOTABLE        = 0x0001
};

// Passive Spell codes explicit used in code
#define SPELL_ID_GENERIC_LEARN                   483
#define SPELL_ID_PASSIVE_BATTLE_STANCE           2457
#define SPELL_ID_PASSIVE_RESURRECTION_SICKNESS   15007
#define SPELL_ID_WEAPON_SWITCH_COOLDOWN_1_5s     6119
#define SPELL_ID_WEAPON_SWITCH_COOLDOWN_1_0s     6123

#define PLAYER_CORPSE_LOOT_ENTRY 1

enum WeatherType : uint32
{
    WEATHER_TYPE_FINE       = 0,
    WEATHER_TYPE_RAIN       = 1,
    WEATHER_TYPE_SNOW       = 2,
    WEATHER_TYPE_STORM      = 3,
    WEATHER_TYPE_THUNDERS   = 86,
    WEATHER_TYPE_BLACKRAIN  = 90
};

#define MAX_WEATHER_TYPE 4

enum ChatMsg : uint32
{
    CHAT_MSG_ADDON                  = 0xFFFFFFFF,
    CHAT_MSG_SYSTEM                 = 0x00,
    CHAT_MSG_SAY                    = 0x01,
    CHAT_MSG_PARTY                  = 0x02,
    CHAT_MSG_RAID                   = 0x03,
    CHAT_MSG_GUILD                  = 0x04,
    CHAT_MSG_OFFICER                = 0x05,
    CHAT_MSG_YELL                   = 0x06,
    CHAT_MSG_WHISPER                = 0x07,
#ifdef LICH_KING
    CHAT_MSG_WHISPER_FOREIGN        = 0x08,
#else
    CHAT_MSG_08_OBSOLETE            = 0x08,
#endif
    CHAT_MSG_WHISPER_INFORM         = 0x09,
    CHAT_MSG_EMOTE                  = 0x0A,
    CHAT_MSG_TEXT_EMOTE             = 0x0B,
    CHAT_MSG_MONSTER_SAY            = 0x0C,
    CHAT_MSG_MONSTER_PARTY          = 0x0D,
    CHAT_MSG_MONSTER_YELL           = 0x0E,
    CHAT_MSG_MONSTER_WHISPER        = 0x0F,
    CHAT_MSG_MONSTER_EMOTE          = 0x10,
    CHAT_MSG_CHANNEL                = 0x11,
    CHAT_MSG_CHANNEL_JOIN           = 0x12,
    CHAT_MSG_CHANNEL_LEAVE          = 0x13,
    CHAT_MSG_CHANNEL_LIST           = 0x14,
    CHAT_MSG_CHANNEL_NOTICE         = 0x15,
    CHAT_MSG_CHANNEL_NOTICE_USER    = 0x16,
    CHAT_MSG_AFK                    = 0x17,
    CHAT_MSG_DND                    = 0x18,
    CHAT_MSG_IGNORED                = 0x19,
    CHAT_MSG_SKILL                  = 0x1A,
    CHAT_MSG_LOOT                   = 0x1B,
    CHAT_MSG_MONEY                  = 0x1C,
    CHAT_MSG_OPENING                = 0x1D,
    CHAT_MSG_TRADESKILLS            = 0x1E,
    CHAT_MSG_PET_INFO               = 0x1F,
    CHAT_MSG_COMBAT_MISC_INFO       = 0x20,
    CHAT_MSG_COMBAT_XP_GAIN         = 0x21,
    CHAT_MSG_COMBAT_HONOR_GAIN      = 0x22,
    CHAT_MSG_COMBAT_FACTION_CHANGE  = 0x23,
    CHAT_MSG_BG_SYSTEM_NEUTRAL      = 0x24,
    CHAT_MSG_BG_SYSTEM_ALLIANCE     = 0x25,
    CHAT_MSG_BG_SYSTEM_HORDE        = 0x26,
    CHAT_MSG_RAID_LEADER            = 0x27,
    CHAT_MSG_RAID_WARNING           = 0x28,
    CHAT_MSG_RAID_BOSS_WHISPER      = 0x29,
    CHAT_MSG_RAID_BOSS_EMOTE        = 0x2A,
    CHAT_MSG_FILTERED               = 0x2B,
    CHAT_MSG_BATTLEGROUND           = 0x2C,
    CHAT_MSG_BATTLEGROUND_LEADER    = 0x2D,
    CHAT_MSG_RESTRICTED             = 0x2E,

#ifdef LICH_KING
    CHAT_MSG_BATTLENET              = 0x2F,
    CHAT_MSG_ACHIEVEMENT            = 0x30,
    CHAT_MSG_GUILD_ACHIEVEMENT      = 0x31,
    CHAT_MSG_ARENA_POINTS           = 0x32,
    CHAT_MSG_PARTY_LEADER           = 0x33,

#endif
    MAX_CHAT_MSG_TYPE
};

enum ChatLinkColors : uint32
{
    CHAT_LINK_COLOR_SPELL       = 0xff71d5ff,   // bright blue
    CHAT_LINK_COLOR_TALENT      = 0xff4e96f7,   // blue
#ifdef LICH_KING
    CHAT_LINK_COLOR_TRADE       = 0xffffd000,   // orange
    CHAT_LINK_COLOR_ENCHANT     = 0xffffd000,   // orange
    CHAT_LINK_COLOR_ACHIEVEMENT = 0xffffff00,
    CHAT_LINK_COLOR_GLYPH       = 0xff66bbff,
#endif
};

// Values from ItemPetFood (power of (value-1) used for compare with CreatureFamilyEntry.petDietMask
enum PetDiet : uint32
{
    PET_DIET_MEAT     = 1,
    PET_DIET_FISH     = 2,
    PET_DIET_CHEESE   = 3,
    PET_DIET_BREAD    = 4,
    PET_DIET_FUNGAS   = 5,
    PET_DIET_FRUIT    = 6,
    PET_DIET_RAW_MEAT = 7,
    PET_DIET_RAW_FISH = 8
};

#define MAX_PET_DIET 9

#define CHAIN_SPELL_JUMP_RADIUS 10

enum GuildLogs
{
    GUILD_BANKLOG_MAX_RECORDS   = 25,
    GUILD_EVENTLOG_MAX_RECORDS  = 100,
};

enum AIReaction : uint32
{
    AI_REACTION_ALERT    = 0,                               // pre-aggro (used in client packet handler)
    AI_REACTION_FRIENDLY = 1,                               // (NOT used in client packet handler)
    AI_REACTION_HOSTILE  = 2,                               // sent on every attack, triggers aggro sound (used in client packet handler)
    AI_REACTION_AFRAID   = 3,                               // seen for polymorph (when AI not in control of self?) (NOT used in client packet handler)
    AI_REACTION_DESTROY  = 4,                               // used on object destroy (NOT used in client packet handler)
};

// Diminishing Returns Types
enum DiminishingReturnsType : uint32
{
    DRTYPE_NONE         = 0,                                // this spell is not diminished, but may have limited it's duration to 10s
    DRTYPE_PLAYER       = 1,                                // this spell is diminished only when applied on players
    DRTYPE_ALL          = 2                                 // this spell is diminished in every case
};

enum DiminishingLevels : uint32
{
    DIMINISHING_LEVEL_1             = 0,
    DIMINISHING_LEVEL_2             = 1,
    DIMINISHING_LEVEL_3             = 2,
    DIMINISHING_LEVEL_IMMUNE        = 3
};

// Diminishing Return Groups
enum DiminishingGroup : uint32
{
    // Common Groups
    DIMINISHING_NONE                                  = 0,
    DIMINISHING_CONTROLLED_STUN,                          // Player Controlled stuns
    DIMINISHING_STUN,                                     // By aura proced stuns, usualy chance on hit talents
    DIMINISHING_SLEEP,
    DIMINISHING_CONTROLLED_ROOT,                          // Immobilizing effects from casted spells
    DIMINISHING_ROOT,                                     // Immobilizing effects from triggered spells like Frostbite
    DIMINISHING_FEAR,                                     // Non-warlock fears
    DIMINISHING_MIND_CONTROL,
    // Warlock Specific
    DIMINISHING_HORROR,                                   // Death Coil Diminish only with another Death Coil
    // Shared Class Specific
#ifdef LICH_KING
    DIMINISHING_CYCLONE,
    DIMINISHING_OPENING_STUN,                             // Cheap Shot and Pounce
#else
    DIMINISHING_BLIND_CYCLONE,                            // From 2.3.0
    DIMINISHING_KIDNEY_SHOT,
#endif
    DIMINISHING_DISARM,                                   // From 2.3.0
    DIMINISHING_SILENCE,                                  // From 2.3.0
    DIMINISHING_DISORIENT,                                // Also with Sap, all Knockout mechanics are here
    DIMINISHING_BANISH,
    // Other
    // Don't Diminish, but limit duration to 10s
    DIMINISHING_LIMITONLY,

    DIMINISHING_MAX,
};

enum SummonCategory : uint32
{
    SUMMON_CATEGORY_WILD    = 0,
    SUMMON_CATEGORY_ALLY    = 1,
    SUMMON_CATEGORY_PET     = 2,
    SUMMON_CATEGORY_PUPPET  = 3,
	//LK only from this point
    SUMMON_CATEGORY_VEHICLE = 4,
    SUMMON_CATEGORY_UNK     = 5, // as of patch 3.3.5a only Bone Spike in Icecrown Citadel uses this category
};

//Only ones used on BC: 0,1,2,4,5
enum SummonType
{
    SUMMON_TYPE_NONE        = 0,
    SUMMON_TYPE_PET         = 1,
    SUMMON_TYPE_GUARDIAN    = 2,
    SUMMON_TYPE_MINION      = 3,
    SUMMON_TYPE_TOTEM       = 4,
    SUMMON_TYPE_MINIPET     = 5,
    SUMMON_TYPE_GUARDIAN2   = 6,
    SUMMON_TYPE_WILD2       = 7,
    SUMMON_TYPE_WILD3       = 8,    // Related to phases and DK prequest line (3.3.5a)
    SUMMON_TYPE_VEHICLE     = 9,
    SUMMON_TYPE_VEHICLE2    = 10,   // Oculus and Argent Tournament vehicles (3.3.5a)
    SUMMON_TYPE_LIGHTWELL   = 11,
    SUMMON_TYPE_JEEVES      = 12
};

enum SummonPropertiesId : uint32
{
    SUMMON_PROPERTIES_ID_CRITTER     = 41,
    SUMMON_PROPERTIES_ID_GUARDIAN    = 61,
    SUMMON_PROPERTIES_ID_TOTEM_SLOT1 = 63,
    SUMMON_PROPERTIES_ID_WILD        = 64,
    SUMMON_PROPERTIES_ID_POSESSED    = 65,
    SUMMON_PROPERTIES_ID_DEMON       = 66,
    SUMMON_PROPERTIES_ID_SUMMON      = 67,
    SUMMON_PROPERTIES_ID_TOTEM_SLOT2 = 81,
    SUMMON_PROPERTIES_ID_TOTEM_SLOT3 = 82,
    SUMMON_PROPERTIES_ID_TOTEM_SLOT4 = 83,
    SUMMON_PROPERTIES_ID_TOTEM       = 121,
    SUMMON_PROPERTIES_ID_UNKNOWN3    = 181,
    SUMMON_PROPERTIES_ID_UNKNOWN4    = 187,
    SUMMON_PROPERTIES_ID_UNKNOWN1    = 247,
    SUMMON_PROPERTIES_ID_CRITTER2    = 407,
    SUMMON_PROPERTIES_ID_CRITTER3    = 307,
    SUMMON_PROPERTIES_ID_UNKNOWN5    = 409,
    SUMMON_PROPERTIES_ID_POSESSED3   = 427,
    SUMMON_PROPERTIES_ID_POSESSED2   = 428
};

enum EventId : uint32
{
    EVENT_CHARGE            = 1003,
    EVENT_JUMP              = 1004,

    /// Special charge event which is used for charge spells that have explicit targets
    /// and had a path already generated - using it in PointMovementGenerator will not
    /// create a new spline and launch it
    EVENT_CHARGE_PREPATH    = 1005,

    EVENT_FACE              = 1006,
#ifdef LICH_KING
    EVENT_VEHICLE_BOARD     = 1007,
    EVENT_VEHICLE_EXIT      = 1008,
#endif
    EVENT_ASSIST_MOVE       = 1009,
};

enum MountResult : uint32
{
    MOUNT_RESULT_CANT_MOUNT_UNIT                      = 0x0,
    MOUNT_RESULT_TOO_FAR                              = 0x1,
    MOUNT_RESULT_ALREADY_MOUNTED                      = 0x2,
    MOUNT_RESULT_UNIT_CANT_BE_MOUNTED                 = 0x3,
    MOUNT_RESULT_MOUNT_ISNT_YOUR_PET                  = 0x4,
};

enum ResponseCodes : uint32
{
    RESPONSE_SUCCESS                                       = 0x00,
    RESPONSE_FAILURE                                       = 0x01,
    RESPONSE_CANCELLED                                     = 0x02,
    RESPONSE_DISCONNECTED                                  = 0x03,
    RESPONSE_FAILED_TO_CONNECT                             = 0x04,
    RESPONSE_CONNECTED                                     = 0x05,
    RESPONSE_VERSION_MISMATCH                              = 0x06,

    CSTATUS_CONNECTING                                     = 0x07,
    CSTATUS_NEGOTIATING_SECURITY                           = 0x08,
    CSTATUS_NEGOTIATION_COMPLETE                           = 0x09,
    CSTATUS_NEGOTIATION_FAILED                             = 0x0A,
    CSTATUS_AUTHENTICATING                                 = 0x0B,

    AUTH_OK                                                = 0x0C,
    AUTH_FAILED                                            = 0x0D,
    AUTH_REJECT                                            = 0x0E,
    AUTH_BAD_SERVER_PROOF                                  = 0x0F,
    AUTH_UNAVAILABLE                                       = 0x10,
    AUTH_SYSTEM_ERROR                                      = 0x11,
    AUTH_BILLING_ERROR                                     = 0x12,
    AUTH_BILLING_EXPIRED                                   = 0x13,
    AUTH_VERSION_MISMATCH                                  = 0x14,
    AUTH_UNKNOWN_ACCOUNT                                   = 0x15,
    AUTH_INCORRECT_PASSWORD                                = 0x16,
    AUTH_SESSION_EXPIRED                                   = 0x17,
    AUTH_SERVER_SHUTTING_DOWN                              = 0x18,
    AUTH_ALREADY_LOGGING_IN                                = 0x19,
    AUTH_LOGIN_SERVER_NOT_FOUND                            = 0x1A,
    AUTH_WAIT_QUEUE                                        = 0x1B,
    AUTH_BANNED                                            = 0x1C,
    AUTH_ALREADY_ONLINE                                    = 0x1D,
    AUTH_NO_TIME                                           = 0x1E,
    AUTH_DB_BUSY                                           = 0x1F,
    AUTH_SUSPENDED                                         = 0x20,
    AUTH_PARENTAL_CONTROL                                  = 0x21,
    AUTH_LOCKED_ENFORCED                                   = 0x22,

    REALM_LIST_IN_PROGRESS                                 = 0x23,
    REALM_LIST_SUCCESS                                     = 0x24,
    REALM_LIST_FAILED                                      = 0x25,
    REALM_LIST_INVALID                                     = 0x26,
    REALM_LIST_REALM_NOT_FOUND                             = 0x27,

    ACCOUNT_CREATE_IN_PROGRESS                             = 0x28,
    ACCOUNT_CREATE_SUCCESS                                 = 0x29,
    ACCOUNT_CREATE_FAILED                                  = 0x2A,

    CHAR_LIST_RETRIEVING                                   = 0x2B,
    CHAR_LIST_RETRIEVED                                    = 0x2C,
    CHAR_LIST_FAILED                                       = 0x2D,

    CHAR_CREATE_IN_PROGRESS                                = 0x2E,
    CHAR_CREATE_SUCCESS                                    = 0x2F,
    CHAR_CREATE_ERROR                                      = 0x30,
    CHAR_CREATE_FAILED                                     = 0x31,
    CHAR_CREATE_NAME_IN_USE                                = 0x32,
    CHAR_CREATE_DISABLED                                   = 0x33,
    CHAR_CREATE_PVP_TEAMS_VIOLATION                        = 0x34,
    CHAR_CREATE_SERVER_LIMIT                               = 0x35,
    CHAR_CREATE_ACCOUNT_LIMIT                              = 0x36,
    CHAR_CREATE_SERVER_QUEUE                               = 0x37,
    CHAR_CREATE_ONLY_EXISTING                              = 0x38,
    CHAR_CREATE_EXPANSION                                  = 0x39,

    CHAR_DELETE_IN_PROGRESS                                = 0x3A,
    CHAR_DELETE_SUCCESS                                    = 0x3B,
    CHAR_DELETE_FAILED                                     = 0x3C,
    CHAR_DELETE_FAILED_LOCKED_FOR_TRANSFER                 = 0x3D,
    CHAR_DELETE_FAILED_GUILD_LEADER                        = 0x3E,
    CHAR_DELETE_FAILED_ARENA_CAPTAIN                       = 0x3F,

    CHAR_LOGIN_IN_PROGRESS                                 = 0x40,
    CHAR_LOGIN_SUCCESS                                     = 0x41,
    CHAR_LOGIN_NO_WORLD                                    = 0x42,
    CHAR_LOGIN_DUPLICATE_CHARACTER                         = 0x43,
    CHAR_LOGIN_NO_INSTANCES                                = 0x44,
    CHAR_LOGIN_FAILED                                      = 0x45,
    CHAR_LOGIN_DISABLED                                    = 0x46,
    CHAR_LOGIN_NO_CHARACTER                                = 0x47,
    CHAR_LOGIN_LOCKED_FOR_TRANSFER                         = 0x48,
    CHAR_LOGIN_LOCKED_BY_BILLING                           = 0x49,

    CHAR_NAME_SUCCESS                                      = 0x4A,
    CHAR_NAME_FAILURE                                      = 0x4B,
    CHAR_NAME_NO_NAME                                      = 0x4C,
    CHAR_NAME_TOO_SHORT                                    = 0x4D,
    CHAR_NAME_TOO_LONG                                     = 0x4E,
    CHAR_NAME_INVALID_CHARACTER                            = 0x4F,
    CHAR_NAME_MIXED_LANGUAGES                              = 0x50,
    CHAR_NAME_PROFANE                                      = 0x51,
    CHAR_NAME_RESERVED                                     = 0x52,
    CHAR_NAME_INVALID_APOSTROPHE                           = 0x53,
    CHAR_NAME_MULTIPLE_APOSTROPHES                         = 0x54,
    CHAR_NAME_THREE_CONSECUTIVE                            = 0x55,
    CHAR_NAME_INVALID_SPACE                                = 0x56,
    CHAR_NAME_CONSECUTIVE_SPACES                           = 0x57,
    CHAR_NAME_RUSSIAN_CONSECUTIVE_SILENT_CHARACTERS        = 0x58,
    CHAR_NAME_RUSSIAN_SILENT_CHARACTER_AT_BEGINNING_OR_END = 0x59,
    CHAR_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME            = 0x5A,
};

/// Sanctions function modes. These are fixed since they are inserted and read in database.
enum SanctionType : uint32
{
    SANCTION_BAN_ACCOUNT     = 1,
    SANCTION_BAN_CHARACTER   = 2,
    SANCTION_BAN_IP          = 3,
    SANCTION_MUTE_CHARACTER  = 4,
    SANCTION_MUTE_ACCOUNT    = 5,
    SANCTION_CHAN_BAN        = 6,
};

/// Ban function return codes
enum BanReturn : uint32
{
    BAN_SUCCESS,
    BAN_SYNTAX_ERROR,
    BAN_NOTFOUND,
    BAN_EXISTS,
};

enum ScriptType : uint32
{
    SCRIPT_TYPE_CREATURE    = 0,
    SCRIPT_TYPE_SPELL       = 1
};

enum RemoveMethod : uint8
{
    GROUP_REMOVEMETHOD_DEFAULT  = 0,
    GROUP_REMOVEMETHOD_KICK     = 1,
    GROUP_REMOVEMETHOD_LEAVE    = 2,
#ifdef LICH_KING
    GROUP_REMOVEMETHOD_KICK_LFG = 3
#endif
};

enum WorldStateType
{
    WORLD_STATE_REMOVE              = 0,
    WORLD_STATE_ADD                 = 1
};

enum ActivateTaxiReply : uint32
{
    ERR_TAXIOK                      = 0,
    ERR_TAXIUNSPECIFIEDSERVERERROR  = 1,
    ERR_TAXINOSUCHPATH              = 2,
    ERR_TAXINOTENOUGHMONEY          = 3,
    ERR_TAXITOOFARAWAY              = 4,
    ERR_TAXINOVENDORNEARBY          = 5,
    ERR_TAXINOTVISITED              = 6,
    ERR_TAXIPLAYERBUSY              = 7,
    ERR_TAXIPLAYERALREADYMOUNTED    = 8,
    ERR_TAXIPLAYERSHAPESHIFTED      = 9,
    ERR_TAXIPLAYERMOVING            = 10,
    ERR_TAXISAMENODE                = 11,
    ERR_TAXINOTSTANDING             = 12
};

enum DuelCompleteType : uint32
{
    DUEL_INTERRUPTED = 0,
    DUEL_WON        = 1,
    DUEL_FLED       = 2
};

#define BG_TEAMS_COUNT  2

// indexes of BattlemasterList.dbc
enum BattlegroundTypeId : uint32
{
    BATTLEGROUND_TYPE_NONE      = 0, // None
    BATTLEGROUND_AV             = 1, // Alterac Valley
    BATTLEGROUND_WS             = 2, // Warsong Gulch
    BATTLEGROUND_AB             = 3, // Arathi Basin
    BATTLEGROUND_NA             = 4, // Nagrand Arena
    BATTLEGROUND_BE             = 5, // Blade's Edge Arena
    BATTLEGROUND_AA             = 6, // All Arenas
    BATTLEGROUND_EY             = 7, // Eye of the Storm
    BATTLEGROUND_RL             = 8, // Ruins of Lordaernon
#ifdef LICH_KING
    BATTLEGROUND_SA             = 9, // Strand of the Ancients
    BATTLEGROUND_DS             = 10, // Dalaran Sewers
    BATTLEGROUND_RV             = 11, // Ring of Valor
    BATTLEGROUND_IC             = 30, // Isle of Conquest
    BATTLEGROUND_RB             = 32  // Random Battleground
#endif
};

#ifdef LICH_KING
#define MAX_BATTLEGROUND_TYPE_ID 33
#else
#define MAX_BATTLEGROUND_TYPE_ID 9
#endif

//#define MAX_BATTLEGROUND_QUEUE_RANGES 7                           // for level ranges 10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70+

// handle the queue types and bg types separately to enable joining queue for different sized arenas at the same time
enum BattlegroundQueueTypeId : uint32
{
    BATTLEGROUND_QUEUE_NONE   = 0,
    BATTLEGROUND_QUEUE_AV     = 1,
    BATTLEGROUND_QUEUE_WS     = 2,
    BATTLEGROUND_QUEUE_AB     = 3,
    BATTLEGROUND_QUEUE_EY     = 4,
#ifdef LICH_KING
    BATTLEGROUND_QUEUE_SA       ,
    BATTLEGROUND_QUEUE_IC       ,
    BATTLEGROUND_QUEUE_RB       ,
#endif
    BATTLEGROUND_QUEUE_2v2     ,
    BATTLEGROUND_QUEUE_3v3     ,
    BATTLEGROUND_QUEUE_5v5     ,

    MAX_BATTLEGROUND_QUEUE_TYPES,
};

//MUST TEST ON BC
enum GroupJoinBattlegroundResult
{
    // positive values are indexes in BattlemasterList.dbc
    ERR_GROUP_JOIN_BATTLEGROUND_FAIL        = 0,            // Your group has joined a battleground queue, but you are not eligible (showed for non existing BattlemasterList.dbc indexes)

    ERR_BATTLEGROUND_NONE                   = -1,           // not show anything
    ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS   = -2,           // You cannot join the battleground yet because you or one of your party members is flagged as a Deserter.
#ifdef LICH_KING
    ERR_ARENA_TEAM_PARTY_SIZE               = -3,           // Incorrect party size for this arena.
#else
    ERR_BATTLEGROUND_MIXED_TEAM             = -3,           // Your group is not in the same team
#endif
    ERR_BATTLEGROUND_TOO_MANY_QUEUES        = -4,           // You can only be queued for 2/3 battles at once
    ERR_BATTLEGROUND_CANNOT_QUEUE_FOR_RATED = -5,           // You cannot queue for a rated match while queued for other battles
    ERR_BATTLEDGROUND_QUEUED_FOR_RATED      = -6,           // You cannot queue for another battle while queued for a rated arena match
    ERR_BATTLEGROUND_TEAM_LEFT_QUEUE        = -7,           // Your team has left the arena queue
    //after that, BC client only show "Your group has joined a battleground queue, but you are not eligible". Tested on -8 -9 -10 -11 -12
#ifdef LICH_KING
    ERR_BATTLEGROUND_NOT_IN_BATTLEGROUND    = -8,           // You can't do that in a battleground.
    ERR_BATTLEGROUND_JOIN_XP_GAIN           = -9,           // wtf, doesn't exist in client...
    ERR_BATTLEGROUND_JOIN_RANGE_INDEX       = -10,          // Cannot join the queue unless all members of your party are in the same battleground level range.
    ERR_BATTLEGROUND_JOIN_TIMED_OUT         = -11,          // %s was unavailable to join the queue. (ObjectGuid guid exist in client cache)
    ERR_BATTLEGROUND_JOIN_FAILED            = -12,          // Join as a group failed (ObjectGuid guid doesn't exist in client cache)
    ERR_LFG_CANT_USE_BATTLEGROUND           = -13,          // You cannot queue for a battleground or arena while using the dungeon system.
    ERR_IN_RANDOM_BG                        = -14,          // Can't do that while in a Random Battleground queue.
    ERR_IN_NON_RANDOM_BG                    = -15           // Can't queue for Random Battleground while in another Battleground queue.
#else
    FAKE_ERR_BATTLEGROUND_MIXED_LEVELS      = -100, //LANG_BG_GROUP_MIXED_LEVELS
    FAKE_ERR_BATTLEGROUND_OFFLINE_MEMBER    , //LANG_BG_GROUP_OFFLINE_MEMBER
    FAKE_ERR_BATTLEGROUND_ALREADY_IN_QUEUE  , //LANG_BG_GROUP_MEMBER_ALREADY_IN_QUEUE
    FAKE_ERR_BATTLEGROUND_FROZEN            , //LANG_BG_GROUP_MEMBER_FROZEN
    FAKE_ERR_BATTLEGROUND_TEAM_SIZE         , //LANG_BG_GROUP_TOO_LARGE
#endif
};

enum LineOfSightChecks
{
    LINEOFSIGHT_CHECK_VMAP      = 0x1, // check static floor layout data
    LINEOFSIGHT_CHECK_GOBJECT   = 0x2, // check dynamic game object data

    LINEOFSIGHT_ALL_CHECKS      = (LINEOFSIGHT_CHECK_VMAP | LINEOFSIGHT_CHECK_GOBJECT)
};

enum WeaponAttackType : uint8
{
    BASE_ATTACK   = 0,
    OFF_ATTACK    = 1,
    RANGED_ATTACK = 2,
    MAX_ATTACK    = 3
};

enum CharterTypes
{
    CHARTER_TYPE_NONE                             = 0,
    CHARTER_TYPE_ANY                              = 10,

    GUILD_CHARTER_TYPE                            = 9,
    ARENA_TEAM_CHARTER_2v2_TYPE                   = 2,
    ARENA_TEAM_CHARTER_3v3_TYPE                   = 3,
    ARENA_TEAM_CHARTER_5v5_TYPE                   = 5
};

enum TradeStatus : uint8
{
    TRADE_STATUS_BUSY           = 0,
    TRADE_STATUS_BEGIN_TRADE    = 1,
    TRADE_STATUS_OPEN_WINDOW    = 2,
    TRADE_STATUS_TRADE_CANCELED = 3,
    TRADE_STATUS_TRADE_ACCEPT   = 4,
    TRADE_STATUS_BUSY_2         = 5,
    TRADE_STATUS_NO_TARGET      = 6,
    TRADE_STATUS_BACK_TO_TRADE  = 7,
    TRADE_STATUS_TRADE_COMPLETE = 8,
    TRADE_STATUS_TRADE_REJECTED = 9,
    TRADE_STATUS_TARGET_TO_FAR  = 10,
    TRADE_STATUS_WRONG_FACTION  = 11,
    TRADE_STATUS_CLOSE_WINDOW   = 12,
    TRADE_STATUS_UNK_13         = 13,
    TRADE_STATUS_IGNORE_YOU     = 14,
    TRADE_STATUS_YOU_STUNNED    = 15,
    TRADE_STATUS_TARGET_STUNNED = 16,
    TRADE_STATUS_YOU_DEAD       = 17,
    TRADE_STATUS_TARGET_DEAD    = 18,
    TRADE_STATUS_YOU_LOGOUT     = 19,
    TRADE_STATUS_TARGET_LOGOUT  = 20,
    TRADE_STATUS_TRIAL_ACCOUNT  = 21,                       // Trial accounts can not perform that action
    TRADE_STATUS_ONLY_CONJURED  = 22                        // You can only trade conjured items... (cross realm BG related).
};

#endif
