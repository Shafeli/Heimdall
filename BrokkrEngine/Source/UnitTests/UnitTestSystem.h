#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <Core/Core.h>

namespace Brokkr
{
    using TestFunction = std::function<bool()>;

    struct TestCase
    {
        std::string name;
        TestFunction function;
    };

    class UnitTestSystem final : public Brokkr::System
    {
        std::vector<TestCase> m_tests;

    public:
        explicit UnitTestSystem(CoreSystems* pCoreManager)
            : System(pCoreManager)
        {
            //
        }

        void AddTest(const std::string& name, TestFunction func)
        {
            m_tests.push_back({ name, std::move(func) });
        }

        void RunTests()
        {
            int passed = 0, failed = 0;
            std::vector<std::string> passedTests;
            std::vector<std::string> failedTests;

            for (const auto& test : m_tests)
            {
                bool result = test.function();
                if (result)
                {
                    std::cout << "[PASS] " << test.name << "\n";
                    passedTests.push_back(test.name);
                    passed++;
                }
                else
                {
                    std::cout << "[FAIL] " << test.name << "\n";
                    failedTests.push_back(test.name);
                    failed++;
                }
            }

            ///////////////////////////////////////////////////////////////////////
            // Summary Output
            std::cout << "\n===== Test Summary =====\n";
            std::cout << "Total: " << passed + failed << " | Passed: " << passed << " | Failed: " << failed << "\n";

            if (!passedTests.empty())
            {
                std::cout << "\nPassed Tests:\n";
                for (const auto& test : passedTests)
                {
                    std::cout << "  - " << test << "\n";
                }
            }

            if (!failedTests.empty())
            {
                std::cout << "\nFailed Tests:\n";
                for (const auto& test : failedTests)
                {
                    std::cout << "  - " << test << "\n";
                }
            }
            std::cout << "=========================\n";
            /////////////////////////////////////////////////////////////////////////
        }

        virtual void Destroy() override;
    };
}
