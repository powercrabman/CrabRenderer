#pragma once

namespace crab
{

class RenderResourceIDGenerator
{
public:
    ResourceIDType GenerateID()
    {
        ASSERT(m_idCounter != RENDERER_RESOURCE_NULL_ID
                   && m_idCounter != RENDERER_RESOURCE_UNKNOWN_ID,
               "RenderResourceIDGenerator::GenerateID: ID overflow.");

        return m_idCounter++;
    }

private:
    ResourceIDType m_idCounter = 0;
};

}   // namespace crab