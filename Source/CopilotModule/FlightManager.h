/*
   MulticopterSim FlightManager class implementation using Haskell Copilot

   Copyright(C) 2021 Simon D.Levy

   MIT License
*/

#pragma once

#include "../MainModule/FlightManager.hpp"
#include "../MainModule/Dynamics.hpp"
#include "../MainModule/GameInput.hpp"

class FCopilotFlightManager : public FFlightManager {

    private:

        // Joystick (RC transmitter, game controller) or keypad
        GameInput * _gameInput = NULL;

    public:

        FCopilotFlightManager(APawn * pawn, Dynamics * dynamics);

        ~FCopilotFlightManager();

        virtual void getActuators(const double time, double * values) override;

}; // FCopilotFlightManager