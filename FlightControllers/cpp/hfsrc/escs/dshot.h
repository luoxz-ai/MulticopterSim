/*
   Copyright (c) 2023 Simon D. Levy

   This file is part of Hackflight.

   Hackflight is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   Hackflight is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along with
   Hackflight. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <vector>

#include <dshot_stm32.h>

#include "esc.h"

class DshotEsc : public Esc {

    private:

        const uint32_t STARTUP_USEC = 5000000;

        Stm32Dshot * m_dshot;

    public:

        DshotEsc(Stm32Dshot * dshot)
        {
            m_dshot = dshot;
        }

        virtual void write(float motorvals[]) override
        {
            m_dshot->write(motorvals);
        }

        virtual bool isReady(const uint32_t usec) override 
        {
            static bool ready;
            static uint32_t prev;
            if (usec-prev > STARTUP_USEC) {
                prev = usec;
                ready = true;
            }
            return ready;
        }
}; 
