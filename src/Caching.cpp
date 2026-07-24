#include <Caching.h>
#include <cstdint>
#include <mutex>
#include <xxhash.h>
namespace MPL::Services::EDIDFormID
{
    void CachingService::Init()
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
        auto load = rfl::ubjson::load<CachedDataInternal>("MMSFCache.bin");
        if (load.has_value())
        {
            this->data = load.value();
        }
    }
    void CachedData::Save()
    {
        rfl::ubjson::save("MMSFCache.bin", this->data);
    }
    CachedData::~CachedData()
    {
        Save();
    }
    RE::FormID CachingService::Allocate(uint64_t hash)
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
            if (this->file->IsLight() && cache->data.allocation_offset >= 4096)
            {
                logger::error("MMSF.esp has more than 4096 forms, crashing game for safety (Unmark as ESL to fix).");
                stl::report_and_fail("MMSF.esp is a light plugin with over 4096 forms, crashing game for safety (Unmark as esl to fix).");
            }
            cache->data.allocation_map[hash] = cache->data.allocation_offset;
            cache->data.allocation_offset++;
            return this->base_id | cache->data.allocation_map.at(hash);
        }
    }
    RE::TESForm* CachingService::CreateForm(std::string edid, RE::FormType type)
    {
        auto hash = this->GetHash(edid);
        if(this->edidCaches.edid_to_formid.contains(edid))
        {
            return this->LookupCachedForm(edid);
        }
        auto cfc = RE::IFormFactory::GetFormFactoryByType(type);
        RE::TESForm* form = cfc->Create();
        form->SetFormID(this->Allocate(hash), false);
        form->SetFormEditorID(edid.c_str());
        form->SetFile(this->file);
        this->CacheForm(edid, form->GetFormID());
        return form;
    }
    RE::FormID CachingService::LookupEdid(std::string edid)
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
    std::string CachingService::LookupFormID(RE::FormID fid)
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
    RE::TESForm* CachingService::LookupCachedForm(std::string edid)
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
    void CachingService::CacheForm(std::string edid, RE::FormID id)
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
    uint64_t CachingService::GetLoadOrderHash()
    {
        if (this->load_order_state == LoadOrderState::NotCalculated)
        {
            XXH3_state_t* state = XXH3_createState();
            XXH3_64bits_reset(state);
            for (auto tdh_file : RE::TESDataHandler::GetSingleton()->files)
            {
                XXH3_64bits_update(state, tdh_file->GetFilename().data(), tdh_file->GetFilename().size());
                XXH3_64bits_update(state, &tdh_file->filesize, sizeof(tdh_file->filesize));
                XXH3_64bits_update(state, &tdh_file->formCount, sizeof(tdh_file->formCount));
                XXH3_64bits_update(state, &tdh_file->masterCount, sizeof(tdh_file->masterCount));
            }
            uint64_t hash = XXH3_64bits_digest(state);
            XXH3_freeState(state);
            if (hash != CachedData::GetSingleton()->data.hash)
            {
                CachedData::GetSingleton()->data.hash = hash;
                this->load_order_state = LoadOrderState::Changed;
            }
            else
            {
                this->load_order_state = LoadOrderState::Unchanged;
            }
            return hash;
        }
        else
        {
            return CachedData::GetSingleton()->data.hash;
        }
    }
    bool CachingService::LoadOrderChanged()
    {
        if (this->load_order_state == LoadOrderState::NotCalculated)
            this->GetLoadOrderHash();
        return this->load_order_state == LoadOrderState::Changed;
    }

    uint64_t CachingService::GetHash(std::string str)
    {
        return XXH3_64bits(str.data(), str.size());
    }
}  // namespace MPL::Services::EDIDFormID
