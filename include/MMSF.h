#pragma once
#include <MMSF_API.h>
namespace MPL::API::MMSF
{
    class Shim : public Interface
    {
    public:
        MMSFAPIFeatures GetVersion() override;
        RE::FormID LookupFormIDForEDID(std::string) override;
        std::string LookupEDIDForFormID(RE::FormID) override;
        RE::TESForm* LookupCachedForm(std::string) override;
        RE::TESForm* AllocateForm(std::string, RE::FormType) override;

    };
}  // namespace MPL::API
