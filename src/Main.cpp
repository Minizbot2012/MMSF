#include <Papyrus.h>
#include <Hook.h>
#include <MMSF.h>

MPL::API::Interface g_mmsfIFace;
void APIHandler(SKSE::MessagingInterface::Message* msg)
{
    switch (msg->type)
    {
    case MPL::API::MMSFMessage::kMessage_GetInterface:
        logger::info("API Request Recieved from {}", msg->sender);
        reinterpret_cast<MPL::API::MMSFMessage*>(msg->data)->API = &g_mmsfIFace;
        break;
    default:
        break;
    }
}

SKSEPluginInfo(
    .Version = REL::Version{ 1,0,0,0 },
    .Name = "MMSF"sv,
    .Author = "Mini"sv,
    .SupportEmail = ""sv,
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary
);

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    if(!SKSE::GetMessagingInterface()->RegisterListener(nullptr, APIHandler)) {
        logger::info("Failed to register handler for MMSF");
    }
    SKSE::GetPapyrusInterface()->Register(MPL::Papyrus::Register);
    MPL::Hooks::Install();
    return true;
}
