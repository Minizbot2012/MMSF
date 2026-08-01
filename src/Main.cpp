#include <MMSF_API.h>
#include <MMSF.h>
#include <Hook.h>
#include <Papyrus.h>
#include <Plugin.h>
#include <SKSE/Interfaces.h>

MPL::API::MMSF::Shim g_mmsf;
void APIHandler(SKSE::MessagingInterface::Message* msg)
{
    switch (msg->type)
    {
    case MPL::API::MMSF::MMSFMessage::kMessage_GetInterface:
        reinterpret_cast<MPL::API::MMSF::MMSFMessage*>(msg->data)->API = &g_mmsf;
        break;
    }
}
void MessageHandler(SKSE::MessagingInterface::Message* msg)
{
    switch (msg->type)
    {
    case SKSE::MessagingInterface::kDataLoaded:
        MPL::Services::EDIDFormID::CachedData::GetSingleton()->Save();
        break;
    case SKSE::MessagingInterface::kSaveGame:
    case SKSE::MessagingInterface::kNewGame:
        MPL::Services::EDIDFormID::CachedData::GetSingleton()->Save();
        break;
    default:
        break;
    }
}

SKSEPluginInfo(
    .Version = REL::Version{ MPL::Plugin::MAJOR, MPL::Plugin::MINOR, MPL::Plugin::PATCH, 0 },
    .Name = MPL::Plugin::PROJECT,
    .Author = "Mini"sv,
    .SupportEmail = ""sv,
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary
);

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    logger::info("Game version : {}", a_skse->RuntimeVersion().string());
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
    SKSE::GetMessagingInterface()->RegisterListener(nullptr, APIHandler);
    if(!SKSE::GetPapyrusInterface()->Register(MPL::Papyrus::Register)) {
        logger::info("Failed to register Papyrus functions");
    }
    MPL::Hooks::Install();
    return true;
}
