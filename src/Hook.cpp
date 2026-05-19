#include "Hooking.h"
#include <EDIDCache.h>
#include <Hook.h>
namespace MPL::Hooks
{
    struct TESWeather_Load_TESFile_ReadChunkData
    {
        static inline constexpr REL::RelocationID relocation = REL::RelocationID(19965, 20396, 19965);
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
        static inline constexpr REL::RelocationID relocation = REL::RelocationID(16198, 16442);
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
    void Install()
    {
        stl::install_hook<TESWeather_Load_TESFile_ReadChunkData>();
        stl::install_hook<TESRegion_Load_TESFile_ReadChunkData>();
    }
}  // namespace MPL::Hooks
