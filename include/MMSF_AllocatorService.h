#pragma once
#include <IdPool.h>
#include <MMSF_API.h>
namespace MPL::API::MMSF
{
    class AllocatorService : public IFormAllocator
    {
    private:
        AllocatorService() = default;
        RE::FormID Allocate(uint64_t);
        RE::FormID base_id = 0x0;
        RE::TESFile* file;
        std::unordered_map<uint64_t, RE::TESForm*> formgen_map;
        std::unordered_map<uint64_t, uint32_t> allocation_map;
        MPL::Common::IdPool FormIDPool;

    public:
        static AllocatorService* GetSingleton()
        {
            static AllocatorService instance;
            return &instance;
        }
        uint8_t GetVersion() override;
        std::string GetName() override;
        void Initialize() override;
        rfl::Generic::Object Save() override;
        void Load(rfl::Generic::Object) override;
        RE::TESForm* CreateForm(std::string, RE::FormType) override;
    };
}  // namespace MPL::API::MMSF
