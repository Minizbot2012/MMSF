#include <cstdint>
#include <rfl.hpp>
#include <rfl/msgpack.hpp>
#include <rfl/msgpack/save.hpp>
#include <string>
#include <unordered_map>
namespace MPL::Services::EDIDFormID
{
    struct EdidCache
    {
        std::unordered_map<std::string, RE::FormID> edid_to_formid;
        std::unordered_map<RE::FormID, std::string> formid_to_edid;
    };
    struct CachedDataInternal
    {
        uint32_t allocation_offset = 0x0;
        std::unordered_map<uint64_t, uint32_t> allocation_map;
        uint64_t hash = 0;
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
    enum struct LoadOrderState
    {
        Unchanged,
        Changed,
        NotCalculated,
    };
    class CachingService : public REX::Singleton<CachingService>
    {
    private:
        bool is_init = false;
        std::mutex _lock;
        RE::FormID base_id = 0x0;
        EdidCache edidCaches;
        RE::TESFile* file;
        LoadOrderState load_order_state = LoadOrderState::NotCalculated;

    public:
        void Init();
        RE::FormID Allocate(uint64_t);
        RE::TESForm* CreateForm(std::string, RE::FormType);
        RE::TESForm* LookupCachedForm(std::string);
        RE::FormID LookupEdid(std::string);
        std::string LookupFormID(RE::FormID);
        void CacheForm(std::string, RE::FormID);
        bool LoadOrderChanged();
        uint64_t GetLoadOrderHash();
        uint64_t GetHash(std::string);
    };
}  // namespace MPL::Services::EDIDFormID
