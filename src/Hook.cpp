#include <EDIDCache.h>
#include <Hook.h>
#include <Hooking.h>
namespace MPL::Hooks
{
    struct TESWeather_Load_TESFile_ReadChunkData
    {
        using Target = RE::TESWeather;
        static inline constexpr VariantIndex index = VariantIndex(0x6);
        static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x3CD, 0x403, 0x3CD);
        static bool thunk(RE::TESFile* a_this, void* ptr, std::uint32_t a_chunkSize)
        {
            auto result = func(a_this, ptr, a_chunkSize);
            if (result)
            {
                if (auto str = static_cast<const char* const>(ptr); !std::string(str).empty() && a_this->currentform.formID != 0)
                {
                    logger::info("Caching Weather {}", str);
                    MPL::Services::EDIDFormID::FormIDCaching::GetSingleton()->CacheForm(std::string(str), a_this->currentform.formID);
                }
            }
            return result;
        }
        static void post_hook()
        {
            logger::info("Installed hook at TESWeather::Load");
        }
        static inline REL::Relocation<decltype(thunk)> func;
    };
    struct TESRegion_Load_TESFile_ReadChunkData
    {
        using Target = RE::TESRegion;
        static inline constexpr VariantIndex index = VariantIndex(0x6);
        static inline constexpr REL::VariantOffset offset = REL::VariantOffset(0x104, 0x104, 0x104);
        static bool thunk(RE::TESFile* a_this, void* ptr, std::uint32_t a_chunkSize)
        {
            auto result = func(a_this, ptr, a_chunkSize);
            if (result)
            {
                if (auto str = static_cast<const char* const>(ptr); !std::string(str).empty() && a_this->currentform.formID != 0)
                {
                    logger::info("Caching Region {}", str);
                    MPL::Services::EDIDFormID::FormIDCaching::GetSingleton()->CacheForm(std::string(str), a_this->currentform.formID);
                }
            }
            return result;
        }
        static void post_hook()
        {
            logger::info("Installed hook at TESRegion::Load");
        }
        static inline REL::Relocation<decltype(thunk)> func;
    };
    struct SetFormEditorID
    {
        static bool thunk(RE::TESForm* a_this, const char* a_str)
        {
            if (!std::string(a_str).empty() && !a_this->IsDynamicForm())
            {
                const auto& [map, lock] = RE::TESForm::GetAllFormsByEditorID();
                const RE::BSWriteLockGuard locker{ lock };
                if (map)
                {
                    map->emplace(a_str, a_this);
                }
                logger::info("SFEID {} {}", a_str, RE::FormTypeToString(a_this->GetFormType()));
                MPL::Services::EDIDFormID::FormIDCaching::GetSingleton()->CacheForm(std::string(a_str), a_this->GetFormID());
            }
            return func(a_this, a_str);
        }
        static inline REL::Relocation<decltype(thunk)> func;
        static constexpr VariantIndex index = VariantIndex(0x33);
    };
    void Install()
    {
        stl::write_vfunc<RE::TESWeather, SetFormEditorID>();
        stl::write_vfunc<RE::TESRegion, SetFormEditorID>();
        stl::install_hook<TESWeather_Load_TESFile_ReadChunkData>();
        stl::install_hook<TESRegion_Load_TESFile_ReadChunkData>();
    }
}  // namespace MPL::Hooks
