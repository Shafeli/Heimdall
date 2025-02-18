#pragma once
#include "Agent/AgentController.h"
#include "XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

class GameComponentsReg
{
public:
    static void ComponentReg(Brokkr::EntityXMLParser* parser)
    {
        AgentController::RegisterCreationFunction(parser);
    }
};
