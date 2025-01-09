#pragma once

namespace Brokkr
{
    class DeleteRuleOfFive
    {
    public:
        // Default constructor
        DeleteRuleOfFive() = default;

        // Delete copy constructor
        DeleteRuleOfFive(const DeleteRuleOfFive&) = delete;

        // Delete copy assignment operator
        DeleteRuleOfFive& operator=(const DeleteRuleOfFive&) = delete;

        // Delete move constructor
        DeleteRuleOfFive(DeleteRuleOfFive&&) noexcept = delete;

        // Delete move assignment operator
        DeleteRuleOfFive& operator=(DeleteRuleOfFive&&) noexcept = delete;

        // Default destructor
        virtual ~DeleteRuleOfFive() = default;
    };
}
