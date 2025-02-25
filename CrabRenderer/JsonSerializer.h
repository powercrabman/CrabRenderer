#pragma once

#include <nlohmann/json.hpp>
#include <fstream>

namespace crab
{

template <typename T>
class JsonSerializer
{
public:
    JsonSerializer() = default;
    ~JsonSerializer() = default;

    void Serialize(const T& in_data)
    {
        m_js = in_data;
    }

    void Deserialize(T& out_data)
    {
        out_data = m_js.get<T>();
    }

    void SaveToFile(const std::filesystem::path& in_filePath, uint32 in_dump = 0) const
    {
        std::ofstream ofs(in_filePath);
        ofs << m_js.dump(in_dump);
    }

    void LoadFromFile(const std::filesystem::path& in_filePath)
    {
        std::ifstream ifs(in_filePath);
        ifs >> m_js;
    }

private:
	using Json = nlohmann::json;
    Json m_js;
};

}