#include <Services.h>
#include <cstdint>
#include <mutex>
#include <xxhash.h>
namespace MPL::Services
{
    void ServiceContainer::Init()
    {
        auto TDH = RE::TESDataHandler::GetSingleton();
        this->file = const_cast<RE::TESFile*>(TDH->LookupModByName("MMSF.esp"));
        if (this->base_id == 0x0)
            this->base_id = (uint32_t)file->compileIndex << 24;
        if (this->file->IsLight())
            this->base_id |= (uint32_t)file->smallFileCompileIndex << 12;
        this->is_init = true;
    }
    CachedData::CachedData()
    {
        auto load = rfl::flexbuf::load<CachedDataInternal>("./Data/SKSE/MMSFCache.bin");
        if (load.has_value())
        {
            this->data = load.value();
        }
    }
    void CachedData::Save()
    {
        rfl::flexbuf::save("./Data/SKSE/MMSFCache.bin", this->data);
    }
    CachedData::~CachedData()
    {
        Save();
    }
    RE::FormID ServiceContainer::Allocate(uint64_t hash)
    {
        std::lock_guard _guard(this->_lock);
        auto cache = CachedData::GetSingleton();
        if (!this->is_init)
        {
            Init();
        }
        if (cache->data.allocation_map.contains(hash))
        {
            return this->base_id | cache->data.allocation_map.at(hash);
        }
        else
        {
            auto fid = cache->data.FormIDPool.allocate();
            if (this->file->IsLight() && fid > 0xFFF)
            {
                logger::error("MMSF.esp has more than 4095 forms, crashing game for safety (Unmark as ESL to fix).");
                stl::report_and_fail("MMSF.esp is a light plugin with over 4095 forms, crashing game for safety (Unmark as esl to fix).");
            }
            cache->data.allocation_map[hash] = fid;
            return this->base_id | cache->data.allocation_map.at(hash);
        }
    }
    RE::TESForm* ServiceContainer::CreateForm(std::string edid, RE::FormType type)
    {
        auto hash = this->GetHash(edid);
        auto cfc = RE::IFormFactory::GetFormFactoryByType(type);
        RE::TESForm* form = cfc->Create();
        form->SetFormID(this->Allocate(hash), true);
        form->SetFormEditorID(edid.c_str());
        auto* TDH = RE::TESDataHandler::GetSingleton();
        TDH->AddFormToDataHandler(form);
        return form;
    }
    RE::FormID ServiceContainer::LookupEdid(std::string edid)
    {
        if (!this->edidCaches.edid_to_formid.contains(edid))
        {
            auto frm = RE::TESForm::LookupByEditorID(edid);
            if (frm != nullptr)
            {
                auto id = frm->GetFormID();
                this->CacheForm(edid, id);
            }
        }
        return this->edidCaches.edid_to_formid[edid];
    }
    std::string ServiceContainer::LookupFormID(RE::FormID fid)
    {
        if (this->edidCaches.formid_to_edid.contains(fid))
        {
            return this->edidCaches.formid_to_edid.at(fid);
        }
        else
        {
            std::string edid = RE::TESForm::LookupByID(fid)->GetFormEditorID();
            if (!edid.empty())
            {
                this->CacheForm(edid, fid);
                return edid;
            }
            else
            {
                return "ERR";
            }
        }
    }
    RE::TESForm* ServiceContainer::LookupCachedForm(std::string edid)
    {
        auto frm = RE::TESForm::LookupByEditorID(edid);
        if (frm != nullptr)
        {
            this->CacheForm(edid, frm->formID);
        }
        else
        {
            frm = RE::TESForm::LookupByID(this->LookupEdid(edid));
        }
        return frm;
    }
    void ServiceContainer::CacheForm(std::string edid, RE::FormID id)
    {
        if (!this->edidCaches.edid_to_formid.contains(edid))
        {
            this->edidCaches.edid_to_formid[edid] = id;
        }
        if (!this->edidCaches.formid_to_edid.contains(id))
        {
            this->edidCaches.formid_to_edid[id] = edid;
        }
    }

    uint64_t ServiceContainer::GetHash(std::string str)
    {
        return XXH3_64bits(str.data(), str.size());
    }
}  // namespace MPL::Services::EDIDFormID
