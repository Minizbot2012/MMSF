#include <rfl/msgpack/save.hpp>
#include <unordered_map>
#include <rfl.hpp>
#include <rfl/msgpack.hpp>
namespace MPL::Services::EDIDFormID
{
    struct EdidCache
    {
        std::unordered_map<std::string, RE::FormID> edid_to_formid;
        std::unordered_map<RE::FormID, std::string> formid_to_edid;
    };
    struct FormIDAllocator
    {
        uint32_t offset = 0x0;
        std::unordered_map<std::string, uint32_t> map;
    };
    class FormIDCaching : public REX::Singleton<FormIDCaching>
    {
    private:
        bool is_init = false;
        std::mutex _lock;
        RE::FormID base_id = 0x0;
        FormIDAllocator allocations;
        EdidCache edidCaches;
        RE::TESFile* file;
    public:
        void Init();
        void Save();
        void Load();
        RE::FormID Allocate(std::string);
        RE::TESForm* CreateForm(std::string, RE::FormType);
        RE::TESForm* LookupCachedForm(std::string);
        RE::FormID LookupEdid(std::string);
        std::string LookupFormID(RE::FormID);
        void CacheForm(std::string, RE::FormID);
    };
}  // namespace MPL::Services::EDIDFormID
