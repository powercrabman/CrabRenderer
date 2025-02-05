#include "CrabPch.h"

#include "ILayer.h"
#include "LayerStack.h"

namespace crab
{

LayerStack::LayerStack()
{
    m_layers.reserve(10);
}

LayerStack::~LayerStack()
{
    for (const auto& layer: m_layers)
    {
        layer->OnDetach();
    }
}

void LayerStack::PushLayer(Scope<ILayer>&& in_layer)
{
    const auto& scope = m_layers.emplace_back(std::move(in_layer));
    scope->OnAttach();
}

void LayerStack::PopLayer(const TypeInfo& in_type)
{
    auto it = std::find_if(m_layers.begin(),
                           m_layers.end(),
                           [&in_type](const auto& in_layer)
                           { return in_layer->GetTypeInfo() == in_type; });

    if (it != m_layers.end())
    {
        (*it)->OnDetach();
        m_layers.erase(it);
    }
}

}   // namespace crab