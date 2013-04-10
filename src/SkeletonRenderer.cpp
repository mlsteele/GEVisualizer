#include "SkeletonRenderer.h"
#include <boost/foreach.hpp>
#include <boost/function.hpp>

namespace SkeletonRenderer{

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
const int skeleton_num_connection = 23;
const int skeleton_connections[skeleton_num_connection][2] = {
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

vector<vector<int> > generateChains() {
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

const vector<vector<int> > skeleton_chains = generateChains();

// returns -1 if no more valid nodes
int chain_next_valid(const vector<int>& chain, const vector< SkeletonJoint >& joints, int first_index) {
    for (int i = first_index; i < chain.size(); i++) {
        if (joints[chain[i]].x != 0) return i;
    }

    return -1;
}


POINT3D render2D_point_pretransform(POINT3D& i) {
    POINT3D o;
    o.x = i.x;
    o.y = -i.y;
    o.z = 0;
    return o;
}

POINT3D render3D_point_pretransform(POINT3D& i) {
    POINT3D o;
    o.x = i.x;
    o.y = -i.y;
    o.z = 0;
    return o;
}

void renderBase(const SkeletonData& skel, const RenderMode& renderMode,
                boost::function< POINT3D(POINT3D&) > point_pretransform, double screenPixelsPerMeter, ofTrueTypeFont& font) {


    if (renderMode.joints) {
        for (int i = 0; i < skel.jointData.size(); i++) {
            const SkeletonJoint& j = skel.jointData[i];
            ofFill();
            ofSetHexColor(0x9311E9);

            if (j.x == 0) continue;

            POINT3D j_point = {j.x / 1000., j.y / 1000., j.z / 1000.};
            POINT3D render_point = point_pretransform(j_point);

            ofSphere(
                render_point.x * screenPixelsPerMeter,
                render_point.y * screenPixelsPerMeter,
                render_point.z * screenPixelsPerMeter,
                screenPixelsPerMeter / 20. );

            ofSetHexColor(0x000000);
            if (renderMode.node_label_indices) {
                // TODO: project onto screen correctly
                font.drawString(ofToString(i),
                render_point.x * screenPixelsPerMeter,
                render_point.y * screenPixelsPerMeter );
            }
            if (renderMode.node_label_locations || renderMode.node_label_location_index == i) {
                string s = "x: " + ofToString(j.x) + "\ny: " + ofToString(j.y);

                font.drawString(s,
                    render_point.x * screenPixelsPerMeter,
                    render_point.y * screenPixelsPerMeter );
            }
        }
    }

    if (renderMode.sticks) {
        for (int i = 0; i < skeleton_num_connection; i++) {
            const SkeletonJoint& a = skel.jointData[skeleton_connections[i][0]];
            const SkeletonJoint& b = skel.jointData[skeleton_connections[i][1]];

            // skip joints which are not seen (default to 0)
            if (a.x == 0 || b.x == 0) continue;

            POINT3D a_point = {a.x / 1000., a.y / 1000., a.z / 1000.};
            POINT3D b_point = {b.x / 1000., b.y / 1000., b.z / 1000.};
            POINT3D a_render_point = point_pretransform(a_point);
            POINT3D b_render_point = point_pretransform(b_point);

            ofFill();
            ofSetHexColor(0x4D169E);
            ofLine(
                a_render_point.x * screenPixelsPerMeter ,
                a_render_point.y * screenPixelsPerMeter ,
                a_render_point.z * screenPixelsPerMeter ,
                b_render_point.x * screenPixelsPerMeter ,
                b_render_point.y * screenPixelsPerMeter ,
                b_render_point.z * screenPixelsPerMeter );
        }
    }

    if (renderMode.chains) {
        BOOST_FOREACH (vector<int> chain, skeleton_chains) {
            int i_a = chain_next_valid(chain, skel.jointData, 0);
            if (i_a == -1) continue; // skip chain if no valid nodes
            
            for(int i_b = chain_next_valid(chain, skel.jointData, i_a+1);
                i_a != -1 && i_b != -1;
                i_a = i_b, i_b = chain_next_valid(chain, skel.jointData, i_a+1))
            {
                const SkeletonJoint& a = skel.jointData[chain[i_a]];
                const SkeletonJoint& b = skel.jointData[chain[i_b]];

                POINT3D a_point = {a.x / 1000., a.y / 1000., a.z / 1000.};
                POINT3D b_point = {b.x / 1000., b.y / 1000., b.z / 1000.};
                POINT3D a_render_point = point_pretransform(a_point);
                POINT3D b_render_point = point_pretransform(b_point);

                ofFill();
                ofSetHexColor(0x4D169E);
                ofLine(
                    a_render_point.x * screenPixelsPerMeter ,
                    a_render_point.y * screenPixelsPerMeter ,
                    a_render_point.z * screenPixelsPerMeter ,
                    b_render_point.x * screenPixelsPerMeter ,
                    b_render_point.y * screenPixelsPerMeter ,
                    b_render_point.z * screenPixelsPerMeter );
            }
        }
    }

    // BOOST_FOREACH (const SkeletonJoint& j, skel.jointData) {
    //     printf("joint (%f, %f, %f)     confidence: %s\n", j.x, j.y, j.z, j.confidence);
    // }
}


void render2D(const SkeletonData& skel, const RenderMode& renderMode, const Projection2D& projection,
              ofTrueTypeFont& font, bool clear) {

    if (renderMode.backdrop) {
        if (clear) {
            // fill background
            ofFill();
            ofSetHexColor(0xFFFFFF);
            ofRect(projection.view_rect);
            ofNoFill();
            ofSetHexColor(0x000000);
            ofRect(projection.view_rect);
        }

        // Draw only in box.
        glScissor(
            (int) projection.view_rect.x ,
            (int) ofGetHeight() - projection.view_rect.y - projection.view_rect.height ,
            (int) projection.view_rect.width ,
            (int) projection.view_rect.height );
        glEnable(GL_SCISSOR_TEST);
    }

    ofPushMatrix();

    // x,y projection setup
    ofTranslate(
        -projection.real_center.x * projection.screenPixelsPerMeter,
        -projection.real_center.y * projection.screenPixelsPerMeter,
        0 );
    ofTranslate(
        projection.view_rect.x + projection.view_rect.width / 2.,
        projection.view_rect.y + projection.view_rect.height / 2.,
        0 );
    // NOTE: zoom stays at 1 so that pixelsPerMeter doesn't effect feature drawing size (how big circles are, etc.)

    renderBase(skel, renderMode, render2D_point_pretransform, projection.screenPixelsPerMeter, font);

    // cleanup
    ofPopMatrix();
    glDisable(GL_SCISSOR_TEST);
}


void render3D(const SkeletonData& skel, const RenderMode& renderMode, const Projection3D& projection,
              ofTrueTypeFont& font) {
    ofPushMatrix();

    // x,y projection setup
    // ofTranslate(
    //     -projection.real_center.x * projection.screenPixelsPerMeter,
    //     -projection.real_center.y * projection.screenPixelsPerMeter,
    //     0 );
    ofTranslate(
        projection.locationRoot.x ,
        projection.locationRoot.y ,
        projection.locationRoot.z );
    // NOTE: zoom stays at 1 so that pixelsPerMeter doesn't effect feature drawing size (how big circles are, etc.)

    renderBase(skel, renderMode, render3D_point_pretransform, projection.screenPixelsPerMeter, font);

    // cleanup
    ofPopMatrix();
    glDisable(GL_SCISSOR_TEST);
}



} // namespace
