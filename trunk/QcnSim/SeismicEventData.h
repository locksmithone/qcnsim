/**
 * @author Marcos Portnoi
 * @date November 2013
 *
 * @copyright Copyright (C) 2013 University of Delaware.
 * @copyright QCNSim uses elements of TARVOS simulator, Copyright (C) 2005, 2006, 2007 Marcos Portnoi.
 * @par 
 * This file is part of QCNSim.  QCNSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.<br>
 * QCNSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.<br>
 * You should have received a copy of the GNU Lesser General Public License
 * along with QCNSim.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Entity.h"
#include <sstream>
#include <string>

/**
 * @brief Seismic Event Data class.
 *
 * @par Description
 * The seismic event will containt ID of sensor, longitude and latitude of sensor registering the event, magnitude of earthquake as observer by sensor,
 * distance from sensor to earthquake hypocenter (in Km), time of event, region ID to which the sensor belongs.
 */
class SeismicEventData: public Entity {
public:
	unsigned int qcnExplorerSensorId; //!< Unique sensor ID.
	double eventTime; //!< Absolute occurrence time of event when measured or observed by sensor.
	double latitude; //!< Latitude of sensor registering the event.
	double longitude; //!< Longitude of sensor registering the event.
	double magnitude; //!< Magnitude of earthquake as measured by sensor.
	double distance; //!< Hypocentral distance between sensor and earthquake hypocenter.
	unsigned int regionId; //!< Unique region ID to which the sensor belongs, according to QCNExplorer.

	SeismicEventData();
	SeismicEventData(unsigned int qcnExplorerSensorId, double latitude, double longitude, double magnitude, double eventTime, double distance, unsigned int regionId);
	SeismicEventData(const std::string &dataLine, char delimiter);
};