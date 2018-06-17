
#include "../../playerbot.h"
#include "WarlockMultipliers.h"
#include "TankWarlockStrategy.h"

using namespace ai;

class GenericWarlockStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GenericWarlockStrategyActionNodeFactory()
    {
        creators["summon voidwalker"] = &summon_voidwalker;
        creators["summon felguard"] = &summon_felguard;
    }
private:
    static ActionNode* summon_voidwalker(PlayerbotAI* ai)
    {
        return new ActionNode ("summon voidwalker",
            /*P*/ {},
            /*A*/ NextAction::array({ new NextAction("drain soul") }),
            /*C*/ {});
    }
    static ActionNode* summon_felguard(PlayerbotAI* ai)
    {
        return new ActionNode ("summon felguard",
            /*P*/ {},
            /*A*/ NextAction::array({ new NextAction("summon voidwalker") }),
            /*C*/ {});
    }
};

TankWarlockStrategy::TankWarlockStrategy(PlayerbotAI* ai) : GenericWarlockStrategy(ai)
{
    actionNodeFactories.Add(new GenericWarlockStrategyActionNodeFactory());
}

ActionList TankWarlockStrategy::getDefaultActions()
{
    return NextAction::array({ new NextAction("shoot", 10.0f) });
}

void TankWarlockStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericWarlockStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "no pet",
        NextAction::array({ new NextAction("summon felguard", 50.0f) })));

}
