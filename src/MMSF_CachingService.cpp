#include <MMSF_API.h>
#include <MMSF_CachingService.h>
#include <rfl/Generic.hpp>

namespace MPL::API::MMSF
{
    void CachingService::Initialize()
    {
    }
    void CachingService::Load(rfl::Generic::Object)
    {
    }
    rfl::Generic::Object CachingService::Save()
    {
        return rfl::Generic::Object();
    }
    RE::FormID CachingService::LookupEdid(std::string edid)
    {
        if (!this->edid_to_formid.contains(edid))
        {
            auto frm = RE::TESForm::LookupByEditorID(edid);
            if (frm != nullptr)
            {
                auto id = frm->GetFormID();
                this->CacheForm(edid, id);
            }
        }
        return this->edid_to_formid[edid];
    }
    std::string CachingService::LookupFormID(RE::FormID fid)
    {
        if (this->formid_to_edid.contains(fid))
        {
            return this->formid_to_edid.at(fid);
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
        if (!this->edid_to_formid.contains(edid))
        {
            this->edid_to_formid[edid] = id;
        }
        if (!this->formid_to_edid.contains(id))
        {
            this->formid_to_edid[id] = edid;
        }
    }

}  // namespace MPL::API::MMSF
