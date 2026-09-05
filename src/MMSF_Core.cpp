#include <MMSF_Core.h>
#include <cstdint>
#include <rfl/Generic.hpp>
#include <rfl/flexbuf/load.hpp>
#include <rfl/flexbuf/save.hpp>
#include <rpcndr.h>
#include <xxhash.h>
namespace MPL::Services
{
    void ServiceContainer::Init()
    {
        if (auto tmp = rfl::flexbuf::load<rfl::Generic::Object>("Data/SKSE/MMSF.bin"); tmp.has_value())
        {
            auto bigObj = tmp.value();
            for (auto [name, service] : service_map)
            {
                if (bigObj.at(name).to_object().has_value())
                {
                    service->Load(bigObj[name].to_object().value());
                }
            }
        }
    }

    void ServiceContainer::Save()
    {
        rfl::Generic::Object bigObj;
        for (auto [name, service] : service_map)
        {
            bigObj[name] = service->Save();
        }
        rfl::flexbuf::save("Data/SKSE/MMSF.bin", bigObj);
    }
    uint64_t ServiceContainer::GetHash(std::string str)
    {
        return XXH3_64bits(str.data(), str.size());
    }
    void ServiceContainer::RegisterService(API::MMSF::IPluginService* service)
    {
        if (!this->service_map.contains(service->GetName()))
        {
            this->service_map[service->GetName()] = service;
            service->Initialize();
        }
    }
    API::MMSF::IPluginService* ServiceContainer::QueryService(std::string& name)
    {
        if (this->service_map.contains(name))
        {
            return this->service_map[name];
        }
        return nullptr;
    }
}  // namespace MPL::Services
