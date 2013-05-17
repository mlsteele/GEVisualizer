// Fake data for debugging visualizer

#include "GEVisualizer.h"
#include <boost/foreach.hpp>

void GEVisualizer::ensureFakeData() {
    // locations
    LocationInfo l22;
    l22.locationID = 22;
    l22.notes = "fake location 22";
    locationInfo.push_back(l22);

    LocationInfo l9;
    l9.locationID = 9;
    l9.notes = "fake location 9";
    locationInfo.push_back(l9);

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
    // my right hand out and towards sensor 22
    SkeletonData skeletonData_rhand;
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-42.922260  , 182.198807   , 1564.243530, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-117.219269 , -6.828606    , 1683.587891, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-115.551750 , -193.178421  , 1722.945557, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-266.406982 , -15.121567   , 1650.567749, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-418.456818 , -121.848244  , 1467.721069, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-433.584045 , -166.177032  , 1109.493042, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(31.968430   , 1.464355     , 1716.608154, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(30.801613   , -200.089020  , 1872.445923, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(189.777084  , -515.598633  , 1775.777954, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-191.734665 , -383.859863  , 1745.052002, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-265.701630 , -805.451599  , 1796.952148, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-193.130524 , -1210.846802 , 1864.930908, 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(-36.033794  , -375.196625  , 1779.554199, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(13.381758   , -807.687683  , 1819.289673, 1.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(0.000000    , 0.000000     , 0.000000   , 0.000000));
    skeletonData_rhand.jointData.push_back(SkeletonJoint(29.330540   , -1217.922974 , 1909.208618, 0.000000));

    SkeletonData skeletonData_rhand_2;
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -42.922260  , 10 + 182.198807   , 10 + 1564.243530, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -117.219269 , 10 + -6.828606    , 10 + 1683.587891, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -115.551750 , 10 + -193.178421  , 10 + 1722.945557, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -266.406982 , 10 + -15.121567   , 10 + 1650.567749, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -418.456818 , 10 + -121.848244  , 10 + 1467.721069, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -433.584045 , 10 + -166.177032  , 10 + 1109.493042, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 31.968430   , 10 + 1.464355     , 10 + 1716.608154, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 30.801613   , 10 + -200.089020  , 10 + 1872.445923, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 189.777084  , 10 + -515.598633  , 10 + 1775.777954, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -191.734665 , 10 + -383.859863  , 10 + 1745.052002, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -265.701630 , 10 + -805.451599  , 10 + 1796.952148, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -193.130524 , 10 + -1210.846802 , 10 + 1864.930908, 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + -36.033794  , 10 + -375.196625  , 10 + 1779.554199, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 13.381758   , 10 + -807.687683  , 10 + 1819.289673, 1.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 0.000000    , 10 + 0.000000     , 10 + 0.000000   , 0.000000));
    skeletonData_rhand_2.jointData.push_back(SkeletonJoint(100 + 29.330540   , 10 + -1217.922974 , 10 + 1909.208618, 0.000000));

    // skeletons
    LocationSkeletonData locationSkeletonData;

    locationSkeletonData.locationID = 22;
    locationSkeletonData.userJointData.push_back(skeletonData_rhand);
    userJointData.push_back(locationSkeletonData);

    locationSkeletonData.locationID = 9;
    locationSkeletonData.userJointData.push_back(skeletonData_rhand);
    locationSkeletonData.userJointData.push_back(skeletonData_rhand_2);
    userJointData.push_back(locationSkeletonData);
}
