#include <MMSF.h>
#include <EDIDCache.h>
namespace MPL::API
{
    std::string Interface::LookupEDIDForFormID(RE::FormID fid) {
        auto caching = Services::EDIDFormID::FormIDCaching::GetSingleton();
        return caching->LookupFormID(fid);
    }
    RE::FormID Interface::LookupFormIDForEDID(std::string edid) {
        auto caching = Services::EDIDFormID::FormIDCaching::GetSingleton();
        return caching->LookupEdid(edid);
    }
    RE::TESForm* Interface::LookupCachedForm(std::string edid) {
        auto caching = Services::EDIDFormID::FormIDCaching::GetSingleton();
        return caching->LookupCachedForm(edid);
    }
    RE::TESForm* Interface::AllocateForm(std::string editorId, RE::FormType type) {
        auto caching = Services::EDIDFormID::FormIDCaching::GetSingleton();
        return caching->CreateForm(editorId, type);
    }
}  // namespace MPL::API
