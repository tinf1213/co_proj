#include "mem/cache/replacement_policies/lfu_rp.hh"

#include <cassert>
#include <memory>

#include "params/LFURP.hh"

LFURP::LFURP(const Params *p)
    : BaseReplacementPolicy(p)
{
}

void
LFURP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    // Reset reference count
    std::static_pointer_cast<LFUReplData>(replacement_data)->refCount = 0;
    std::static_pointer_cast<LFUReplData>(
        replacement_data)->lastTouchTick = Tick(0);
}

void
LFURP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Update reference count
    std::static_pointer_cast<LFUReplData>(replacement_data)->refCount++;
    // Update last touch timestamp
    std::static_pointer_cast<LFUReplData>(
        replacement_data)->lastTouchTick = curTick();
}

void
LFURP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Reset reference count
    std::static_pointer_cast<LFUReplData>(replacement_data)->refCount = 1;
    std::static_pointer_cast<LFUReplData>(
        replacement_data)->lastTouchTick = curTick();
}

ReplaceableEntry*
LFURP::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    // Visit all candidates to find victim
    ReplaceableEntry* victim = candidates[0];
    for (const auto& candidate : candidates) {
        // Update victim entry if necessary
        if (std::static_pointer_cast<LFUReplData>(
                    candidate->replacementData)->refCount <
                std::static_pointer_cast<LFUReplData>(
                    victim->replacementData)->refCount) {
            victim = candidate;
        }
        else if(std::static_pointer_cast<LFUReplData>(
                    candidate->replacementData)->refCount ==
                std::static_pointer_cast<LFUReplData>(
                    victim->replacementData)->refCount){
            if(std::static_pointer_cast<LFUReplData>(
                    candidate->replacementData)->lastTouchTick <
                std::static_pointer_cast<LFUReplData>(
                    victim->replacementData)->lastTouchTick){
                victim = candidate;
            }
        }
    }

    return victim;
}

std::shared_ptr<ReplacementData>
LFURP::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new LFUReplData());
}

LFURP*
LFURPParams::create()
{
    return new LFURP(this);
}
