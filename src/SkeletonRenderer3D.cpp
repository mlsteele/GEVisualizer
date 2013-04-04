#include "SkeletonRenderer3D.h"
#include <boost/foreach.hpp>

// indices of connected skeleton joints
// [0] Head
// [1] Neck
// [2] Torso
// [3] Waist
// [4] LeftCollar
// [5] LeftShoulder
// [6] LeftElbow
// [7] LeftWrist
// [8] LeftHand
// [9] LeftFingertip
// [10] RightCollar
// [11] RightShoulder
// [12] RightElbow
// [13] RightWrist
// [14] RightHand
// [15] RightFingertip
// [16] LeftHip
// [17] LeftKnee
// [18] LeftAnkle
// [19] LeftFoot
// [20] RightHip
// [21] Right Knee
// [22] RightAnkle
// [23] RightFoot

// connections used for stick drawing
static const int skeleton_num_connection = 23;
static const int skeleton_connections[skeleton_num_connection][2] = {
// center
{ 0      , 1 },
{ 1      , 2 },
{ 2      , 3 },
// symmetrical top left },
{ 1      , (4) },
{ (4)    , (5) },
{ (5)    , (6) },
{ (6)    , (7) },
{ (7)    , (8) },
{ (8)    , (9) },
// symmetrical top right },
{ 1      , (4+6) },
{ (4+6)  , (5+6) },
{ (5+6)  , (6+6) },
{ (6+6)  , (7+6) },
{ (7+6)  , (8+6) },
{ (8+6)  , (9+6) },
// symmetrical bottom left },
{ 3      , (16) },
{ (16)   , (17) },
{ (17)   , (18) },
{ (18)   , (19) },
// symmetrical bottom right },
{ 3      , (16+4) },
{ (16+4) , (17+4) },
{ (17+4) , (18+4) },
{ (18+4) , (19+4) }};


// chains used for chain drawing (see constructor)
// 0, 1, 2, 3                            // head to waist
// 2, 4, 5, 6, 7, 8, 9                   // left arm
// 2, 4+6, 5+6, 6+6, 7+6, 8+6, 9+6       // right arm
// 3, 16, 17, 18, 19                     // left leg
// 3, 16+4, 17+4, 18+4, 19+4             // right leg

static vector<vector<int> > generateChains() {
    vector<int> head_chain;
    head_chain.push_back(0);
    head_chain.push_back(1);
    head_chain.push_back(2);
    head_chain.push_back(3);

    vector<int> left_arm_chain;
    left_arm_chain.push_back(1);
    left_arm_chain.push_back(4);
    left_arm_chain.push_back(5);
    left_arm_chain.push_back(6);
    left_arm_chain.push_back(7);
    left_arm_chain.push_back(8);
    left_arm_chain.push_back(9);

    vector<int> right_arm_chain;
    right_arm_chain.push_back(1);
    right_arm_chain.push_back(4+6);
    right_arm_chain.push_back(5+6);
    right_arm_chain.push_back(6+6);
    right_arm_chain.push_back(7+6);
    right_arm_chain.push_back(8+6);
    right_arm_chain.push_back(9+6);

    vector<int> left_leg_chain;
    left_leg_chain.push_back(3);
    left_leg_chain.push_back(16);
    left_leg_chain.push_back(17);
    left_leg_chain.push_back(18);
    left_leg_chain.push_back(19);
    
    vector<int> right_leg_chain;
    right_leg_chain.push_back(3);
    right_leg_chain.push_back(16+4);
    right_leg_chain.push_back(17+4);
    right_leg_chain.push_back(18+4);
    right_leg_chain.push_back(19+4);
    

    vector<vector<int> > skeleton_chains;
    skeleton_chains.push_back(head_chain);
    skeleton_chains.push_back(left_arm_chain);
    skeleton_chains.push_back(right_arm_chain);
    skeleton_chains.push_back(left_leg_chain);
    skeleton_chains.push_back(right_leg_chain);
    return skeleton_chains;
};

static const vector<vector<int> > skeleton_chains = generateChains();

// returns -1 if no more valid nodes
static int chain_next_valid(const vector<int>& chain, const vector< SkeletonJoint >& joints, int first_index) {
    for (int i = first_index; i < chain.size(); i++) {
        if (joints[chain[i]].x != 0) return i;
    }

    return -1;
}

void SkeletonRenderer3D::render3D(SkeletonRenderMode3D& renderMode, const SkeletonData& skel, const POINT3D& location_root, const LayoutRenderer* layoutRenderer) {
    ofPushMatrix();

    if (renderMode.joints) {
        for (int i = 0; i < skel.jointData.size(); i++) {
            const SkeletonJoint& j = skel.jointData[i];

            if (j.x == 0) continue;

            POINT3D joint_3d_original_axes;
            joint_3d_original_axes.x = location_root.x - j.x / 1000. * layoutRenderer->projection.scale.x;
            joint_3d_original_axes.y = location_root.y - j.y / 1000. * layoutRenderer->projection.scale.y;
            joint_3d_original_axes.z = location_root.z - j.z / 1000. * layoutRenderer->projection.scale.y;

            POINT3D joint_3d;
            joint_3d.x = joint_3d_original_axes.x;
            joint_3d.y = joint_3d_original_axes.y;
            joint_3d.z = joint_3d_original_axes.z;

            ofFill();
            ofSetHexColor(0x9311E9);
            ofSphere(
                joint_3d.x ,
                joint_3d.y ,
                joint_3d.z ,
                layoutRenderer->projection.screenPixelsPerMeter / 20. );

            ofSetHexColor(0x000000);
        }
    }

    ofPopMatrix();
}
