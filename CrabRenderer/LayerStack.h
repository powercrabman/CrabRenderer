#pragma once

namespace crab
{

class ILayer;

class LayerStack
{
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Scope<ILayer>&& in_layer);
    void PopLayer(const TypeInfo& in_type);

    std::vector<Scope<ILayer>>::iterator begin() { return m_layers.begin(); }
    std::vector<Scope<ILayer>>::iterator end() { return m_layers.end(); }

    std::vector<Scope<ILayer>>::reverse_iterator rbegin() { return m_layers.rbegin(); }
    std::vector<Scope<ILayer>>::reverse_iterator rend() { return m_layers.rend(); }

private:
    std::vector<Scope<ILayer>> m_layers;
};

}   // namespace crab
