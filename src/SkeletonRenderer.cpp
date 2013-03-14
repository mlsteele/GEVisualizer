#include "SkeletonRenderer.h"

void SkeletonRenderer::setupProjection(POINT2D screen_px_corner, POINT3D real_corner, double screenPixelsPerMeter) {
    projection.screen_px_corner     = screen_px_corner;
    projection.real_corner          = real_corner;
    projection.screenPixelsPerMeter = screenPixelsPerMeter;

    projection.offset.x = - real_corner.x + (screen_px_corner.x / screenPixelsPerMeter);
    projection.offset.y = - real_corner.y + (screen_px_corner.y / screenPixelsPerMeter);
    projection.offset.z = - real_corner.z + (screen_px_corner.y / screenPixelsPerMeter);
    projection.scale.x = screenPixelsPerMeter;
    projection.scale.y = screenPixelsPerMeter;
    projection.scale.z = screenPixelsPerMeter;
}

void SkeletonRenderer::render(SkeletonRenderMode& renderMode, const SkeletonData& skel) {
    ofPushMatrix();

    // x,y projection offset
    ofTranslate(
        projection.offset.x * projection.scale.x ,
        projection.offset.y * projection.scale.y ,
        projection.offset.z * projection.scale.z );

    // scale
    ofScale(projection.dyn.zoomFactor, projection.dyn.zoomFactor, projection.dyn.zoomFactor);

    // pan
    ofTranslate(projection.dyn.pan.x, projection.dyn.pan.y);

    if (renderMode.joints) {
        for (const SkeletonJoint& j : skel.jointData) {
            ofFill();
            ofSetHexColor(0x9311E9);

            if (j.x == 0) continue;

            ofSphere(
                -j.x / 1000. * projection.scale.x ,
                -j.y / 1000. * projection.scale.y ,
                0    / 1000. * projection.scale.z ,
                6 );
        }
    }

    if (renderMode.sticks) {
        for (int i = 0; i < skeleton_num_connection; i++) {
            const SkeletonJoint& a = skel.jointData[skeleton_connections[i][0]];
            const SkeletonJoint& b = skel.jointData[skeleton_connections[i][1]];

            // skip joints which are not seen (default to 0)
            if (a.x == 0 || b.x == 0) continue;

            ofFill();
            ofSetHexColor(0x4D169E);
            ofLine(
                -a.x / 1000. * projection.scale.x ,
                -a.y / 1000. * projection.scale.y ,
                -b.x / 1000. * projection.scale.x ,
                -b.y / 1000. * projection.scale.y );
        }
    }

    ofPopMatrix();
}

void SkeletonRenderer::print(SkeletonData& skel) {
    printf("how many joints on skeleton of user %i? %i\n", skel.userID, skel.jointData.size());
    for (const SkeletonJoint& j : skel.jointData) {
        printf("joint (%f, %f, %f)     confidence: %s\n", j.x, j.y, j.z, j.confidence);
    }
}
