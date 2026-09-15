#include <CompatibilityChecker.h>
#include <MMSF_API.h>
#include <rfl/Generic.hpp>

namespace MPL::API::MMSF
{
    class CompatService : public MPL::API::MMSF::ICompatService
    {
    private:
        CompatService() = default;
        ~CompatService() = default;

    public:
        static CompatService* GetSingleton()
        {
            static CompatService instance;
            return &instance;
        }
        void Initialize() override
        {
            CompatibilityChecker::Initialize();
        }
        rfl::Generic::Object Save() override
        {
            return rfl::Generic::Object();
        }
        void Load(const rfl::Generic::Object) override
        {
        }
        std::uint8_t GetVersion() override
        {
            return 1;
        }
        std::string GetName() override
        {
            return "Compat";
        }
        bool GetProviderSettings(const char* a_id, bool* a_detailedLogging, bool* a_notifications) override
        {
            try
            {
                if (!a_id || !*a_id)
                {
                    return false;
                }
                const auto settings =
                    CompatibilityChecker::LoadManifest(a_id);
                if (!settings)
                {
                    return false;
                }
                if (a_detailedLogging)
                {
                    *a_detailedLogging = settings->detailedLogging;
                }
                if (a_notifications)
                {
                    *a_notifications = settings->notifications;
                }
                return true;
            } catch (const std::exception& error)
            {
                logger::error(
                    "[MMSF API] GetProviderSettings failed: {}",
                    error.what());
            } catch (...)
            {
                logger::error(
                    "[MMSF API] GetProviderSettings failed with an unknown exception");
            }
            return false;
        }
        bool UpdateProviderSettings(
            const char* a_id,
            const std::int8_t a_detailedLogging,
            const std::int8_t a_notifications) override
        {
            try
            {
                if (!a_id || !*a_id)
                {
                    return false;
                }
                const auto detailed =
                    a_detailedLogging < 0 ?
                        std::nullopt :
                        std::optional<bool>(a_detailedLogging != 0);
                const auto notifications =
                    a_notifications < 0 ?
                        std::nullopt :
                        std::optional<bool>(a_notifications != 0);
                return CompatibilityChecker::UpdateManifestSettings(
                    a_id,
                    detailed,
                    notifications);
            } catch (const std::exception& error)
            {
                logger::error(
                    "[Luma API] UpdateProviderSettings failed: {}",
                    error.what());
            } catch (...)
            {
                logger::error(
                    "[Luma API] UpdateProviderSettings failed with an unknown exception");
            }
            return false;
        }
    };
}  // namespace MPL::MMSF::API::Compat
