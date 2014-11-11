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

#include "SeismicEventData.h"

/**
 * @brief Seismic Event Data class constructor with two parameter.
 *
 * @details
 * This constructor tokenizes the parameter and fills member variables, using the delimiter.
 * Note that the order of tokenizing must be obeyed in the line; the order is:
 * qcnExplorerSensorID
 * latitude
 * longitude
 * magnitude
 * eventTime
 * distance
 * regionId.
 *
 * @param dataLine
 * @param delimiter
 */
SeismicEventData::SeismicEventData(const std::string &dataLine, char delimiter) {
	std::string strQcnExplorerSensorId; //<! Unique sensor ID.
	std::string strLatitude; //<! Latitude of sensor registering the event.
	std::string strLongitude; //<! Longitude of sensor registering the event.
	std::string strMagnitude; //<! Magnitude of earthquake as measured by sensor.
	std::string strEventTime; //<! Absolute occurrence time of event when measured or observed by sensor.
	std::string strDistance; //<! Hypocentral distance between sensor and earthquake hypocenter.
	std::string strRegionID; //<! Unique region ID to which the sensor belongs, according to QCNExplorer.
	std::stringstream dataLineStream(dataLine); // Get a stringstream object out of the data line.

	// Now extract fields into strings and fill member variables.
	// QcnExplorerSensorId.
	std::getline(dataLineStream, strQcnExplorerSensorId, delimiter);
	this->qcnExplorerSensorId = std::stoul(strQcnExplorerSensorId);
	// Latitude.
	std::getline(dataLineStream, strLatitude, delimiter);
	this->latitude = std::stod(strLatitude);
	// Longitude.
	std::getline(dataLineStream, strLongitude, delimiter);
	this->longitude = std::stod(strLongitude);
	// Magnitude.
	std::getline(dataLineStream, strMagnitude, delimiter);
	this->magnitude = std::stod(strMagnitude);
	// EventTime.
	std::getline(dataLineStream, strEventTime, delimiter);
	this->eventTime = std::stod(strEventTime);
	// Distance.
	std::getline(dataLineStream, strDistance, delimiter);
	this->distance = std::stod(strDistance);
	// RegionId.
	std::getline(dataLineStream, strRegionID, delimiter);
	this->regionId = std::stoul(strRegionID);
}


	
/**
 * @brief Seismic Event Data class constructor with all parameters
 *
 * @details 
 * This constructor fills member variables from parameters.
 *
 * @param qcnExplorerSensorId Unique sensor ID.
 * @param latitude Latitude of sensor registering the event.
 * @param longitude Longitude of sensor registering the event.
 * @param magnitude Magnitude of earthquake as measured by sensor.
 * @param eventTime Absolute occurrence time of event when measured or observed by sensor.
 * @param distance Hypocentral distance between sensor and earthquake hypocenter.
 * @param regionID Unique region ID to which the sensor belongs, according to QCNExplorer.
 */
SeismicEventData::SeismicEventData(unsigned int qcnExplorerSensorId, double latitude, double longitude, double magnitude, double eventTime, double distance, unsigned int regionId): 
	qcnExplorerSensorId(qcnExplorerSensorId), latitude(latitude), longitude(longitude), magnitude(magnitude), eventTime(eventTime), distance(distance), regionId(regionId) {
}

/**
 * @brief Default constructor.
 */
SeismicEventData::SeismicEventData() {
}