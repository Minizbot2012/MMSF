#include <MMSF.h>
#include <Caching.h>
namespace MPL::API::MMSF
{
    uint8_t Interface::GetMMSFVersion() {
        return 1;
    }
    std::string Interface::LookupEDIDForFormID(RE::FormID fid) {
        auto caching = Services::EDIDFormID::CachingService::GetSingleton();
        return caching->LookupFormID(fid);
    }
    RE::FormID Interface::LookupFormIDForEDID(std::string edid) {
        auto caching = Services::EDIDFormID::CachingService::GetSingleton();
        return caching->LookupEdid(edid);
    }
    RE::TESForm* Interface::LookupCachedForm(std::string edid) {
        auto caching = Services::EDIDFormID::CachingService::GetSingleton();
        return caching->LookupCachedForm(edid);
    }
    RE::TESForm* Interface::AllocateForm(std::string editorId, RE::FormType type) {
        auto caching = Services::EDIDFormID::CachingService::GetSingleton();
        return caching->CreateForm(editorId, type);
    }
    bool Interface::LoadOrderChanged() {
        auto caching = Services::EDIDFormID::CachingService::GetSingleton();
        return caching->LoadOrderChanged();
    }
    uint64_t Interface::GetLoadOrderHash() {
        auto caching = Services::EDIDFormID::CachingService::GetSingleton();
        return caching->GetLoadOrderHash();
    }
}  // namespace MPL::API
