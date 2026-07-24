#pragma once
#include <MMSF_API.h>
#include <cstdint>
namespace MPL::API::MMSF
{
    class Interface : public ServiceMap
    {
    public:
        uint8_t GetMMSFVersion() override;
        RE::FormID LookupFormIDForEDID(std::string) override;
        std::string LookupEDIDForFormID(RE::FormID) override;
        RE::TESForm* LookupCachedForm(std::string) override;
        RE::TESForm* AllocateForm(std::string, RE::FormType) override;
        bool LoadOrderChanged() override;
        uint64_t GetLoadOrderHash() override;
    };
}  // namespace MPL::API
