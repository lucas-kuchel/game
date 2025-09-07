#include <Program/Application.hpp>

#include <Debug/Exception.hpp>
#include <Debug/Logging.hpp>

int main()
{
    try
    {
        Program::Application application;

        while (application.ShouldUpdate())
        {
            application.Update();
        }

        return 0;
    }
    catch (const Debug::Exception& exception)
    {
        std::string_view message = exception.GetMessage();
        std::uint32_t code = exception.GetCode();

        Debug::LogError("RUNTIME EXCEPTION:\n{}", message);

        return code;
    }
    catch (...)
    {
        Debug::LogError("UNKNOWN RUNTIME EXCEPTION");

        return 1;
    }
}