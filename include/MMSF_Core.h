#pragma once
#include "MMSF_API.h"
#include <cstdint>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>
#include <unordered_map>
namespace MPL::Services
{
    class ServiceContainer : public REX::Singleton<ServiceContainer>
    {
    private:
        std::mutex _lock;
        std::unordered_map<std::string, API::MMSF::IPluginService*> service_map;
    public:
        void Init();
        void Save();
        uint64_t GetHash(std::string);
        API::MMSF::IPluginService* QueryService(std::string&);
        void RegisterService(API::MMSF::IPluginService*);
    };
}  // namespace MPL::Services
