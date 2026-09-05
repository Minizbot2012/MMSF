#pragma once
#include <MMSF_API.h>
namespace MPL::API::MMSF
{
    class Shim : public Interface
    {
    public:
        MMSFAPIFeatures GetVersion() override;
        void RegisterService(API::MMSF::IPluginService*) override;
        API::MMSF::IPluginService* QueryService(std::string) override;
    };
}  // namespace MPL::API::MMSF
