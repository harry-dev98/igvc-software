
#include <iostream>

#include "gps.hpp"

#include "OSMC_4wd_driver.hpp"

#include "lidarProc.hpp"
#include "NAV200.hpp"

static const double waypointLat[] = {33.787175};
static const double waypointLon[] = {-84.406264};
static const size_t numPts = 1;

int main()
{
	NAV200 lidar;
	OSMC_4wd_driver motors;

	gps gpsA;
	gpsA.open("/dev/USB0", 38400);

	GPSState state;
	bool stateValid;
	stateValid = gpsA.get_last_state(state);
	while( (!stateValid) )
	{
		std::cout << "Waiting For Satilites" << std::endl;
		usleep(1e6);
		stateValid = gpsA.get_last_state(state);
	}

	//Average position for 10s
	usleep(10e6);
	
	//get new fix
	while( (!stateValid) )
	{
		std::cout << "Waiting For Satilites" << std::endl;
		usleep(2e5);
		stateValid = gpsA.get_last_state(state);
	}

	float goodtheta[NAV200::Num_Points];
	float goodradius[NAV200::Num_Points];
	float runavg_goodradius[NAV200::Num_Points];
	for(size_t i = 0; i < numPts; i++)
	{
		GPSState target;
		target.lat = waypointLat[i];
		target.lon = waypointLon[i];
		double distance = lambert_distance(state, target);

		while( distance > 2.0 )
		{
			while( (!stateValid) )
			{
				std::cout << "Waiting For Satilites" << std::endl;
				usleep(2e5);
				stateValid = gpsA.get_last_state(state);
			}

			distance = lambert_distance(state, target);
			std::cout << "Distance to go: " << distance << " m" << std::endl;
			//get lidar data
			if(!lidar.read())
			{
				std::cerr << "No LIDAR Data, using old data" << std::endl;
			}
			//copy the good points
			size_t numlidarpts = lidar.getValidData(goodtheta, goodradius);
			//running average
			lidarProc::runavg(goodradius, runavg_goodradius, numlidarpts, 30);

			//get vector to first waypoint
			double x = waypointLon[i] - state.lon;
			double y = waypointLat[i] - state.lat;
			double targetvector = atan2(y,x);

			//local 
			double angle_to_target = targetvector - (M_PI_2 - state.courseoverground);

			//can we go the dir we want?
			bool clear = lidarProc::isPathClear(angle_to_target, 1, 1, goodtheta, runavg_goodradius, numlidarpts);

			//if not, find the closest angle
			while(!clear)
			{
				if(angle_to_target >= 0)
				{
					angle_to_target += .05;
					clear = lidarProc::isPathClear(angle_to_target, 1, 1, goodtheta, runavg_goodradius, numlidarpts);
				}
				else
				{
					angle_to_target -= .05;
					clear = lidarProc::isPathClear(angle_to_target, 1, 1, goodtheta, runavg_goodradius, numlidarpts);
				}
			}
			std::cout << "Angle to go (post lidar): " << angle_to_target << " rad" << std::endl;
			//got the angle we decided
			motors.set_vel_vec(sin(angle_to_target),cos(angle_to_target));
		}
	}
}