#include "SKSE/API.h"
#include "SKSE/Interfaces.h"
#include <Hook.h>
#include <MMSF.h>
#include <Papyrus.h>

MPL::API::Interface g_mmsf;
void MessageHandler(SKSE::MessagingInterface::Message* msg)
{
    switch (msg->type)
    {
    case MPL::API::MMSFMessage::kMessage_GetInterface:
        logger::info("API Request Recieved from {}", msg->sender);
        reinterpret_cast<MPL::API::MMSFMessage*>(msg->data)->API = &g_mmsf;
        break;
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
    if (!SKSE::GetMessagingInterface()->RegisterListener(nullptr, MessageHandler))
    {
        logger::info("Failed to register handler for MMSF");
    }
    if(!SKSE::GetMessagingInterface()->RegisterListener(MessageHandler)) {
        logger::info("Failed to register listener for SKSE");
    }
    if(!SKSE::GetPapyrusInterface()->Register(MPL::Papyrus::Register)) {
        logger::info("Failed to register Papyrus functions");
    }
    MPL::Hooks::Install();
    return true;
}
