#include <Services.h>
#include <MMSF_API.h>
#include <MMSF.h>
namespace MPL::API::MMSF
{
    MMSFAPIFeatures Shim::GetVersion()
    {
        return MMSF::EmbedVersion(0x1) | MMSFAPIFeatures::kCaching | MMSFAPIFeatures::kAllocator;
    }
    std::string Shim::LookupEDIDForFormID(RE::FormID fid)
    {
        auto caching = MPL::Services::ServiceContainer::GetSingleton();
        return caching->LookupFormID(fid);
    }
    RE::FormID Shim::LookupFormIDForEDID(std::string edid)
    {
        auto caching = MPL::Services::ServiceContainer::GetSingleton();
        return caching->LookupEdid(edid);
    }
    RE::TESForm* Shim::LookupCachedForm(std::string edid)
    {
        auto caching = MPL::Services::ServiceContainer::GetSingleton();
        return caching->LookupCachedForm(edid);
    }
    RE::TESForm* Shim::AllocateForm(std::string editorId, RE::FormType type)
    {
        auto caching = MPL::Services::ServiceContainer::GetSingleton();
        return caching->CreateForm(editorId, type);
    }
}  // namespace MPL::API::MMSF
