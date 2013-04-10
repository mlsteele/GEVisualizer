#include "GEVisualizer.h"
#include <boost/foreach.hpp>

void GEVisualizer::ensureFakeData() {
    // locations
    LocationInfo l22;
    l22.locationID = 22;
    l22.notes = "fake location 22";
    locationInfo.push_back(l22);

    // user estimates
    BOOST_FOREACH (UserLocationData& userLocationData, locationData) {
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

    // skeletons
    LocationSkeletonData locationSkeletonData;
    locationSkeletonData.locationID = 22;
    SkeletonData skeletonData;

    // for (int i = 0; i < 24; i++) {
    //     SkeletonJoint j(10 * i, 0, 0, 0.99999);
    //     skeletonData.jointData.push_back(j);
    // }

    // my right hand out and towards sensor 22

    skeletonData.jointData.push_back(SkeletonJoint(-42.922260  , 182.198807   , 1564.243530, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-117.219269 , -6.828606    , 1683.587891, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-115.551750 , -193.178421  , 1722.945557, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-266.406982 , -15.121567   , 1650.567749, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-418.456818 , -121.848244  , 1467.721069, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-433.584045 , -166.177032  , 1109.493042, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(31.968430   , 1.464355     , 1716.608154, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(30.801613   , -200.089020  , 1872.445923, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(189.777084  , -515.598633  , 1775.777954, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-191.734665 , -383.859863  , 1745.052002, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-265.701630 , -805.451599  , 1796.952148, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-193.130524 , -1210.846802 , 1864.930908, 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(-36.033794  , -375.196625  , 1779.554199, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(13.381758   , -807.687683  , 1819.289673, 1.000000));
    skeletonData.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData.jointData.push_back(SkeletonJoint(29.330540   , -1217.922974 , 1909.208618, 0.000000));


    locationSkeletonData.userJointData.push_back(skeletonData);
    userJointData.push_back(locationSkeletonData);
}
