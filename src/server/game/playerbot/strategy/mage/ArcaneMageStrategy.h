#pragma once

#include "GenericMageStrategy.h"

namespace ai
{
    class ArcaneMageStrategy : public GenericMageStrategy
    {
    public:
        ArcaneMageStrategy(PlayerbotAI* ai);

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
        virtual std::string getName() { return "arcane"; }
        virtual ActionList getDefaultActions();
    };

}
