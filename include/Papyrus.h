#include "RE/T/TESForm.h"
#include "RE/T/TypeTraits.h"
#include <EDIDCache.h>
namespace MPL::Papyrus
{
    static inline std::string GetEditorID(RE::StaticFunctionTag*, RE::TESForm* form)
    {
        if(!form)
            return "None";
        auto cached = MPL::Services::EDIDFormID::FormIDCaching::GetSingleton();
        auto formID = form->GetFormID();
        auto edid = cached->LookupFormID(formID);
        return edid;
    }
    static inline RE::TESForm* GetFormByEDID(RE::StaticFunctionTag*, std::string edid)
    {
        auto cached = MPL::Services::EDIDFormID::FormIDCaching::GetSingleton();
        return cached->LookupCachedForm(edid);
    }
    static inline bool Register(RE::BSScript::IVirtualMachine* vm)
    {
        vm->RegisterFunction("GetEditorID", "MMSF", GetEditorID);
        vm->RegisterFunction("GetByEditorID", "MMSF", GetFormByEDID);
        return true;
    }
}  // namespace MPL::Papyrus
