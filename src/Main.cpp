#include "MMSF_API.h"
#include "SKSE/Interfaces.h"
#include <Hook.h>
#include <MMSF.h>
#include <Papyrus.h>

MPL::API::MMSF::Interface g_mmsf;
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
        logger::info("Load order hash: {:016X}", MPL::Services::EDIDFormID::CachingService::GetSingleton()->GetLoadOrderHash());
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
        .Version = REL::Version{ 1, 1, 0, 0 },
    .Name = "MMSF"sv,
    .Author = "Mini"sv,
    .SupportEmail = ""sv,
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary);

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
