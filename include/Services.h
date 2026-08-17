#pragma once
#include <cstdint>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>
#include <unordered_map>
namespace MPL::Services
{
    struct EdidCache
    {
        std::unordered_map<std::string, RE::FormID> edid_to_formid;
        std::unordered_map<RE::FormID, std::string> formid_to_edid;
    };
    struct IdPool
    {
        std::map<uint32_t, uint32_t> seg = {
            { 1, 0xFFFFFF - 1 }
        };
        uint32_t allocate()
        {
            auto it = seg.begin();
            uint32_t start = it->first;
            uint32_t len = it->second;
            if (len > 1)
            {
                seg[start + 1] = len - 1;
            }
            seg.erase(it);
            return start;
        }
        void release(uint32_t id)
        {
            auto next_it = seg.upper_bound(id);
            auto prev_it = (next_it != seg.begin()) ? std::prev(next_it) : seg.end();
            bool merge_prev = (prev_it != seg.end() && prev_it->first + prev_it->second == id);
            bool merge_next = (next_it != seg.end() && id + 1 == next_it->first);
            if (merge_prev && merge_next)
            {
                prev_it->second += 1 + next_it->second;
                seg.erase(next_it);
            }
            else if (merge_prev)
            {
                prev_it->second += 1;
            }
            else if (merge_next)
            {
                uint32_t next_len = next_it->second;
                seg.erase(next_it);
                seg[id] = next_len + 1;
            }
            else
            {
                seg[id] = 1;
            }
        }
    };
    struct CachedDataInternal
    {
        IdPool FormIDPool;
        std::unordered_map<uint64_t, uint32_t> allocation_map;
    };
    class CachedData
    {
    protected:
        CachedData();

    public:
        ~CachedData();
        inline static CachedData* GetSingleton()
        {
            static CachedData instance;
            return &instance;
        }
        void Save();
        CachedDataInternal data;
    };
    class ServiceContainer : public REX::Singleton<ServiceContainer>
    {
    private:
        bool is_init = false;
        std::mutex _lock;
        EdidCache edidCaches;
        RE::FormID base_id = 0x0;
        RE::TESFile* file;
        std::unordered_map<uint64_t, RE::TESForm*> formgen_map;

    public:
        void Init();
        RE::FormID Allocate(uint64_t);
        RE::TESForm* CreateForm(std::string, RE::FormType);
        RE::TESForm* LookupCachedForm(std::string);
        RE::FormID LookupEdid(std::string);
        std::string LookupFormID(RE::FormID);
        void CacheForm(std::string, RE::FormID);
        uint64_t GetHash(std::string);
    };
}  // namespace MPL::Services::EDIDFormID
