#include <EDIDCache.h>
#include <mutex>
#include <rfl/msgpack/load.hpp>
#include <rfl/msgpack/save.hpp>
namespace MPL::Services::EDIDFormID
{
    void FormIDCaching::Init()
    {
        auto TDH = RE::TESDataHandler::GetSingleton();
        this->file = const_cast<RE::TESFile*>(TDH->LookupModByName("MMSF.esp"));
        if (this->base_id == 0x0)
            this->base_id = (uint32_t)file->compileIndex << 24 | (uint32_t)file->smallFileCompileIndex << 12;
        Load();
    }
    void FormIDCaching::Load()
    {
        auto load = rfl::msgpack::load<FormIDAllocator>("MMSF-FormIDCache.bin");
        if (load.has_value())
        {
            this->allocations = load.value();
        }
    }
    void FormIDCaching::Save()
    {
        rfl::msgpack::save("MMSF-FormIDCache.bin", this->allocations);
    }
    RE::FormID FormIDCaching::Allocate(std::string edid)
    {
        std::lock_guard _guard(this->_lock);
        if (!this->is_init)
        {
            Init();
        }
        if (this->allocations.map.contains(edid))
        {
            return this->base_id | this->allocations.map.at(edid);
        }
        else
        {
            this->allocations.map[edid] = this->allocations.offset;
            this->allocations.offset++;
            return this->base_id | this->allocations.map.at(edid);
        }
    }
    RE::TESForm* FormIDCaching::CreateForm(std::string edid, RE::FormType type)
    {
        auto cfc = RE::IFormFactory::GetFormFactoryByType(type);
        RE::TESForm* form = cfc->Create();
        form->SetFormID(this->Allocate(edid), false);
        form->SetFormEditorID(edid.c_str());
        form->SetFile(this->file);
        this->CacheForm(edid, form->GetFormID());
        Save();
        return form;
    }
    RE::FormID FormIDCaching::LookupEdid(std::string edid)
    {
        if (!this->edidCaches.edid_to_formid.contains(edid))
        {
            auto frm = RE::TESForm::LookupByEditorID(edid);
            auto id = frm->GetFormID();
            this->edidCaches.edid_to_formid[edid] = id;
            this->edidCaches.formid_to_edid[id] = edid;
        }
        return this->edidCaches.edid_to_formid[edid];
    }
    std::string FormIDCaching::LookupFormID(RE::FormID fid)
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
                CacheForm(edid, fid);
                return edid;
            }
            else
            {
                return "ERR";
            }
        }
    }
    RE::TESForm* FormIDCaching::LookupCachedForm(std::string edid)
    {
        auto frm = RE::TESForm::LookupByEditorID(edid);
        if (frm != nullptr)
        {
            this->CacheForm(edid, frm->formID);
        }
        else
        {
            return RE::TESForm::LookupByID(this->LookupEdid(edid));
        }
        return frm;
    }
    void FormIDCaching::CacheForm(std::string edid, RE::FormID id)
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
}  // namespace MPL::Services::EDIDFormID
