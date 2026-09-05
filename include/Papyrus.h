#pragma once
#include "MMSF_CachingService.h"
#include <MMSF_Core.h>
namespace MPL::Papyrus
{
    static inline std::string GetEditorID(RE::StaticFunctionTag*, RE::TESForm* form)
    {
        if (!form)
            return "None";
        auto cached = MPL::API::MMSF::CachingService::GetSingleton();
        auto formID = form->GetFormID();
        auto edid = cached->LookupFormID(formID);
        return edid;
    }
    static inline RE::TESForm* GetFormByEDID(RE::StaticFunctionTag*, std::string edid)
    {
        auto cached = MPL::API::MMSF::CachingService::GetSingleton();
        return cached->LookupCachedForm(edid);
    }
    static inline bool Register(RE::BSScript::IVirtualMachine* vm)
    {
        vm->RegisterFunction("GetEditorID", "MMSF", GetEditorID);
        vm->RegisterFunction("GetByEditorID", "MMSF", GetFormByEDID);
        return true;
    }
}  // namespace MPL::Papyrus
