#include <MMSF_AllocatorService.h>
#include <MMSF_Core.h>
#include <cstdint>
#include <rfl/from_generic.hpp>
#include <rfl/to_generic.hpp>
namespace MPL::API::MMSF
{
    rfl::Generic::Object AllocatorService::Save()
    {
        rfl::Generic::Object obj;
        obj["IdPool"] = rfl::to_generic(this->FormIDPool);
        obj["Map"] = rfl::to_generic(this->allocation_map);
        return obj;
    }
    void AllocatorService::Load(rfl::Generic::Object obj)
    {
        if (auto idPool = rfl::from_generic<MPL::Common::IdPool>(obj["IdPool"]); idPool.has_value())
        {
            this->FormIDPool = idPool.value();
        }
        if (auto map = rfl::from_generic<std::unordered_map<uint64_t, uint32_t>>(obj["Map"]); map.has_value())
        {
            this->allocation_map = map.value();
        }
    }
    void AllocatorService::Initialize()
    {
        auto TDH = RE::TESDataHandler::GetSingleton();
        this->file = const_cast<RE::TESFile*>(TDH->LookupModByName("MMSF.esp"));
        if (this->base_id == 0x0)
            this->base_id = (uint32_t)file->compileIndex << 24;
        if (this->file->IsLight())
            this->base_id |= (uint32_t)file->smallFileCompileIndex << 12;
    }
    RE::FormID AllocatorService::Allocate(uint64_t hash)
    {
        if (this->allocation_map.contains(hash))
        {
            return this->base_id | this->allocation_map.at(hash);
        }
        else
        {
            auto fid = this->FormIDPool.allocate();
            if (this->file->IsLight() && fid > 0xFFF)
            {
                logger::error("MMSF.esp has more than 4095 forms, crashing game for safety (Unmark as ESL to fix).");
                stl::report_and_fail("MMSF.esp is a light plugin with over 4095 forms, crashing game for safety (Unmark as esl to fix).");
            }
            this->allocation_map[hash] = fid;
            return this->base_id | this->allocation_map.at(hash);
        }
    }
    RE::TESForm* AllocatorService::AllocateForm(std::string edid, RE::FormType type)
    {
        auto hash = MPL::Services::ServiceContainer::GetSingleton()->GetHash(edid);
        auto cfc = RE::IFormFactory::GetFormFactoryByType(type);
        RE::TESForm* form = cfc->Create();
        form->SetFormID(this->Allocate(hash), true);
        form->SetFormEditorID(edid.c_str());
        auto* TDH = RE::TESDataHandler::GetSingleton();
        TDH->AddFormToDataHandler(form);
        return form;
    }
}  // namespace MPL::API::MMSF
