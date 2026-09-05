#include <MMSF.h>
#include <MMSF_API.h>
#include <MMSF_Core.h>
namespace MPL::API::MMSF
{
    MMSFAPIFeatures Shim::GetVersion()
    {
        return MMSF::EmbedVersion(0x2) | MMSFAPIFeatures::kCaching | MMSFAPIFeatures::kAllocator | MMSFAPIFeatures::kCoreService;
    }
    void Shim::RegisterService(API::MMSF::IPluginService* service)
    {
        auto serviceContainer = MPL::Services::ServiceContainer::GetSingleton();
        serviceContainer->RegisterService(service);
    }
    API::MMSF::IPluginService* Shim::QueryService(std::string& name)
    {
        auto serviceContainer = MPL::Services::ServiceContainer::GetSingleton();
        return serviceContainer->QueryService(name);
    }
}  // namespace MPL::API::MMSF
