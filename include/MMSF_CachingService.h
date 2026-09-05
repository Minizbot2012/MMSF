#pragma once
#include <MMSF_API.h>
namespace MPL::API::MMSF
{
    class CachingService : public IEDIDCache
    {
    private:
        std::unordered_map<std::string, RE::FormID> edid_to_formid;
        std::unordered_map<RE::FormID, std::string> formid_to_edid;
        CachingService() = default;

    public:
        static CachingService* GetSingleton()
        {
            static CachingService instance;
            return &instance;
        }
        void Initialize() override;
        rfl::Generic::Object Save();
        void Load(rfl::Generic::Object) override;
        RE::FormID LookupEdid(std::string) override;
        std::string LookupFormID(RE::FormID) override;
        RE::TESForm* LookupCachedForm(std::string) override;
        void CacheForm(std::string, RE::FormID) override;
    };
}  // namespace MPL::API::MMSF
