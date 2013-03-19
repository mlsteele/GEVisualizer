#include "GEVisualizer.h"

void GEVisualizer::ensureFakeData() {
    for (UserLocationData& userLocationData: locationData) {
        if (userLocationData.locationID == 11){
            {
                UserLocationEstimate fake_estimate;
                fake_estimate.userID = 0;
                fake_estimate.x = 400;
                fake_estimate.y = 4000;
                fake_estimate.z = 0;
                fake_estimate.estimationLikelihood = 0.995;

                userLocationData.userLocationEstimates.push_back(fake_estimate);
            }

            {
                UserLocationEstimate fake_estimate;
                fake_estimate.userID = 0;
                fake_estimate.x = 200;
                fake_estimate.y = 2500;
                fake_estimate.z = 0;
                fake_estimate.estimationLikelihood = 0.995;

                userLocationData.userLocationEstimates.push_back(fake_estimate);
            }
        }

        if (userLocationData.locationID == 22){
            UserLocationEstimate fake_estimate;
            fake_estimate.userID = 0;
            fake_estimate.x = -1000;
            fake_estimate.y = 1600;
            fake_estimate.z = 0;
            fake_estimate.estimationLikelihood = 0.995;

            userLocationData.userLocationEstimates.push_back(fake_estimate);
        }

        if (userLocationData.locationID == 9){
            UserLocationEstimate fake_estimate;
            fake_estimate.userID = 0;
            fake_estimate.x = 1800;
            fake_estimate.y = 400;
            fake_estimate.z = 0;
            fake_estimate.estimationLikelihood = 0.995;

            userLocationData.userLocationEstimates.push_back(fake_estimate);
        }

        if (userLocationData.locationID == 10){
            {
                UserLocationEstimate fake_estimate;
                fake_estimate.userID = 0;
                fake_estimate.x = 3000;
                fake_estimate.y = 1000;
                fake_estimate.z = 0;
                fake_estimate.estimationLikelihood = 0.995;

                userLocationData.userLocationEstimates.push_back(fake_estimate);
            }

            {
                UserLocationEstimate fake_estimate;
                fake_estimate.userID = 0;
                fake_estimate.x = -200;
                fake_estimate.y = 1300;
                fake_estimate.z = 0;
                fake_estimate.estimationLikelihood = 0.995;

                userLocationData.userLocationEstimates.push_back(fake_estimate);
            }
        }
    }
}
