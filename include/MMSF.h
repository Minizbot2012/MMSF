#pragma once
#include <MMSF_API.h>
namespace MPL::API
{
    class Interface : public ServiceMap
    {
    public:
        RE::FormID LookupFormIDForEDID(std::string) override;
        std::string LookupEDIDForFormID(RE::FormID) override;
        RE::TESForm* LookupCachedForm(std::string) override;
        RE::TESForm* AllocateForm(std::string, RE::FormType) override;
    };
}  // namespace MPL::API
