#pragma once

#include "../../XMLManager.h"
#include "Vector2.h"

namespace Brokkr
{
    class PositionDataParser final : public XMLParser
    {
    public:

        // Struct of the data needed in the positional work

        explicit PositionDataParser(CoreSystems* pCoreSystems)
            : XMLParser(pCoreSystems)
        {
            //
        }

        virtual ~PositionDataParser() override;
        virtual bool Parse(tinyxml2::XMLDocument& doc) override;
        std::vector<Vector2<float>> ParseLayer(const char* layerName, const char* mapFile) const;
    };
}
