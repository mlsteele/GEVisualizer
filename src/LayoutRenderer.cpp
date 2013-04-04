#include "LayoutRenderer.h"
#include <boost/foreach.hpp>
#include "SkeletonRenderer3D.h"

string format_double_to_string(double n) {
    string s;
    std::ostringstream ss;
    ss << n;
    return ss.str();
}

// // returns data or a NULL if none is present in list
// template <typename T>
// T* extract_streamed_data(vector<T>& list, unsigned int locationID) {
//     BOOST_FOREACH (T& thing, list) {
//         if (thing.locationID == locationID) {
//             return &thing;
//         }
//     }
//     return NULL;
// }

// returns data or a NULL if none is present in list
template <typename T>
const T* extract_streamed_data(const vector<T>& list, unsigned int locationID) {
    BOOST_FOREACH (const T& thing, list) {
        if (thing.locationID == locationID) {
            return &thing;
        }
    }
    return NULL;
}

POINT3D getWinCoords(POINT3D& pTransformed) {
    GLdouble x = pTransformed.x;
    GLdouble y = pTransformed.y;
    GLdouble z = pTransformed.z;
    GLdouble winX, winY, winZ;

    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluProject(x, y, z,
        modelview, projection, viewport,
        &winX, &winY, &winZ);

    POINT3D pWindow;
    pWindow.x = winX;
    // hack/tweaked for OpenFrameworks modelview
    pWindow.y = ofGetHeight() - winY;
    pWindow.z = winZ;
    return pWindow;
}

// WARN: method contains unmatched ofPushMatrix()
void pushUnprojectedMatrix(GLfloat rootModelView[], POINT3D p3d) {
    POINT3D winCoords = getWinCoords(p3d);
    ofPushMatrix();
    glLoadMatrixf(rootModelView);
    ofTranslate(winCoords.x, winCoords.y);
}

// LayoutRenderer::~LayoutRenderer() {
//     printf("destroying LayoutRenderer...\n");
//     if(textureData) {
//         printf("deleting texture data\n");
//         delete[] textureData;
//     }
// }



void LayoutRenderer::setupProjection(POINT2D screen_px_corner, POINT2D real_corner, double screenPixelsPerMeter) {
    Layout& l = *layout;

    // assign
    projection.screen_px_corner     = screen_px_corner;
    projection.real_corner          = real_corner;
    projection.screenPixelsPerMeter = screenPixelsPerMeter;

    reloadProjection();

    if (textureData) delete[] textureData;

    textureSize[0] = layout->svgBoundingRect.width  / layout->pixelsPerMeter * projection.scale.x;
    textureSize[1] = layout->svgBoundingRect.height / layout->pixelsPerMeter * projection.scale.y;
    const unsigned int w = textureSize[0];
    const unsigned int h = textureSize[1];
    printf("textureSize -> {%i, %i}\n", textureSize[0], textureSize[1]);

    textureData = new unsigned char [w * h * 4];

    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){
            // RGBA
            textureData[(j * w + i) * 4 + 0] = 255;
            textureData[(j * w + i) * 4 + 1] = 255;
            textureData[(j * w + i) * 4 + 2] = 255;
            textureData[(j * w + i) * 4 + 3] = 255;
        }
    }

    texture.allocate(textureSize[0], textureSize[1], GL_RGBA);
    texture.loadData(textureData, textureSize[0], textureSize[1], GL_RGBA);
}

void LayoutRenderer::reloadProjection() {
    projection.offset.x = - projection.real_corner.x + (projection.screen_px_corner.x / projection.screenPixelsPerMeter);
    projection.offset.y = - projection.real_corner.y + (projection.screen_px_corner.y / projection.screenPixelsPerMeter);
    projection.scale.x = projection.screenPixelsPerMeter;
    projection.scale.y = projection.screenPixelsPerMeter;
}

/*
// TODO: make this more efficient (less loops)
void LayoutRenderer::recalculateTexture(GEVisualizer& store) {
    const unsigned int w = textureSize[0];
    const unsigned int h = textureSize[1];

    // clear
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            textureData[(j * w + i) * 4 + 3] = 0; // alpha

    int weight_accumulator = 0;
    BOOST_FOREACH (LocationInfo& locationInfo, store.getLocationInfo()) {
        Location* localLocation = extract_streamed_data(layout->locations, locationInfo.locationID);
        if (!localLocation) continue;
        PresenceData* presenceData = extract_streamed_data(store.getPresenceData(), locationInfo.locationID);
        if (!presenceData) continue;
        weight_accumulator += 1;
    }

    // loop and fill against locations
    BOOST_FOREACH (LocationInfo& locationInfo, store.getLocationInfo()) {
        Location* localLocation = extract_streamed_data(layout->locations, locationInfo.locationID);
        if (!localLocation) continue;
        PresenceData* presenceData = extract_streamed_data(store.getPresenceData(), locationInfo.locationID);
        if (!presenceData) continue;

        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++){
                double screen_x_mtrs = projection.real_corner.x + i / projection.screenPixelsPerMeter;
                double screen_y_mtrs = projection.real_corner.y + j / projection.screenPixelsPerMeter;

                // double interest_x = projection.real_corner.x + ofGetMouseX() / projection.screenPixelsPerMeter - projection.offset.x;
                // double interest_y = projection.real_corner.y + ofGetMouseY() / projection.screenPixelsPerMeter - projection.offset.y;
                double interest_x = localLocation->position.x;
                double interest_y = localLocation->position.y;
                // if (i == j && i == 0) printf("%.5f, %.5f\n", interest_x, interest_y);

                const double two_sigma_squared = 2*10*10;
                const double weight = presenceData->presenceLikelihood;
                // const double weight = 1;
                auto distr_x = weight * exp(-pow((screen_x_mtrs - interest_x), 2.) / two_sigma_squared);
                auto distr_y = weight * exp(-pow((screen_y_mtrs - interest_y), 2.) / two_sigma_squared);

                // RGBA
                // textureData[(j * w + i) * 4 + 0] = 255;
                // textureData[(j * w + i) * 4 + 1] = 255;
                // textureData[(j * w + i) * 4 + 2] = 255;
                textureData[(j * w + i) * 4 + 3] += distr_x * distr_y * 255 / weight_accumulator;
            }
        }
    }
}
*/

void LayoutRenderer::mouseTestRecalculateTexture() {
    const unsigned int w = textureSize[0];
    const unsigned int h = textureSize[1];

    // loop and fill against locations
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++){
            double screen_x_mtrs = projection.real_corner.x + i / projection.screenPixelsPerMeter;
            double screen_y_mtrs = projection.real_corner.y + j / projection.screenPixelsPerMeter;

            double interest_x = projection.real_corner.x + ofGetMouseX() / projection.screenPixelsPerMeter - projection.offset.x;
            double interest_y = projection.real_corner.y + ofGetMouseY() / projection.screenPixelsPerMeter - projection.offset.y;
            // double interest_x = localLocation->position.x;
            // double interest_y = localLocation->position.y;
            // if (i == j && i == 0) printf("%.5f, %.5f\n", interest_x, interest_y);

            const double two_sigma_squared = 2*10*10;
            const double weight = 1;
            // const double weight = 1;
            auto distr_x = weight * exp(-pow((screen_x_mtrs - interest_x), 2.) / two_sigma_squared);
            auto distr_y = weight * exp(-pow((screen_y_mtrs - interest_y), 2.) / two_sigma_squared);

            // RGBA
            // textureData[(j * w + i) * 4 + 0] = 255;
            // textureData[(j * w + i) * 4 + 1] = 255;
            // textureData[(j * w + i) * 4 + 2] = 255;
            textureData[(j * w + i) * 4 + 3] = distr_x * distr_y * 255;
        }
    }
}

// store is the data store for ge information
// transition is 0 for normal view, positive for offset up, negative for offset down
// transition bounds of [-1, 1] are nominally the points at which a renderer should be disabled
void LayoutRenderer::render(LayoutRenderMode& renderMode, GEVisualizer& dataStore, float transition) {
    GLfloat rootModelView[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, rootModelView);

    const float w = layout->svgBoundingRect.width  / layout->pixelsPerMeter * projection.scale.x;
    const float y = layout->svgBoundingRect.height / layout->pixelsPerMeter * projection.scale.y;
    // const float projection.xRotation = ofGetElapsedTimef()*10;
    float smoothed_transition = pow(transition, 3);

    // reset finding hoverClosestLocationID
    hoverClosestLocationExists = false;
    hoverClosestLocationID = -1;
    hoverClosestLocationDistance = 0;

    ofPushMatrix(); // translation

    // transition
    ofTranslate(0, smoothed_transition * layout->svgBoundingRect.height / layout->pixelsPerMeter * projection.scale.y * 1.7);

    // x,y projection offset
    ofTranslate(
        projection.offset.x * projection.scale.x ,
        projection.offset.y * projection.scale.y );

    ofPushMatrix(); // skew
    // skew rotation
    ofRotateX(projection.dyn.xRotation);

    // scale
    ofScale(projection.dyn.zoomFactor, projection.dyn.zoomFactor, projection.dyn.zoomFactor);

    // rotate around center
    ofTranslate(w/2, y/2);
    ofRotateZ(projection.dyn.zRotation);
    ofTranslate(-w/2, -y/2);

    // pan
    ofTranslate(projection.dyn.pan.x, projection.dyn.pan.y);

    // texture
    // TODO: reenable texture
    ofEnableAlphaBlending(); // required for wall transparency
    if (renderMode.texture) {
        // recalculateTexture(dataStore);
        ofSetHexColor(0x3D9BFF);
        // texture.loadData(textureData, textureSize[0], textureSize[1], GL_RGBA);
        // texture.draw(0, 0, textureSize[0], textureSize[1]);
    }
    ofDisableAlphaBlending();

    ofEnableSmoothing();

    // map boundary
    if (renderMode.structure) {
        ofSetLineWidth(2);
        ofSetHexColor(0x0000FF);
        ofNoFill();
        ofRect(0, 0, w, y);

        // walls
        const float wallHeight = 2.5 * projection.screenPixelsPerMeter;
        BOOST_FOREACH (LINE& wall, layout->wallLines) {
            ofSetHexColor(0xB3B3B3);
            glColor4f(0, 0, 0, 0.2);
            ofFill();
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_BLEND);

            glBegin(GL_QUADS);
                glVertex3f(
                    wall.x1 * projection.scale.x ,
                    wall.y1 * projection.scale.y ,
                    0 );
                glVertex3f(
                    wall.x2 * projection.scale.x ,
                    wall.y2 * projection.scale.y ,
                    0 );
                glVertex3f(
                    wall.x2 * projection.scale.x ,
                    wall.y2 * projection.scale.y ,
                    wallHeight );
                glVertex3f(
                    wall.x1 * projection.scale.x ,
                    wall.y1 * projection.scale.y ,
                    wallHeight );
            glEnd();

            // bottom line
            glColor4f(0, 0, 0, 0.3);
            ofSetLineWidth(3);
            ofLine(
                wall.x1 * projection.scale.x ,
                wall.y1 * projection.scale.y ,
                wall.x2 * projection.scale.x ,
                wall.y2 * projection.scale.y );

            // top line
            glColor4f(0, 0, 0, 0.2);
            ofPushMatrix();
            ofTranslate(0, 0, wallHeight);
            ofSetLineWidth(1);
            ofLine(
                wall.x1 * projection.scale.x ,
                wall.y1 * projection.scale.y ,
                wall.x2 * projection.scale.x ,
                wall.y2 * projection.scale.y );
            ofPopMatrix();
        }
    }

    // locations locuses
    if (renderMode.locations) {
        BOOST_FOREACH (const LocationInfo& locationInfo, dataStore.getLocationInfo()) {
            const Location* localLocation = extract_streamed_data(layout->locations, locationInfo.locationID);
            if (!localLocation) continue;

            const PresenceData* presenceData = NULL;
            if (renderMode.presence) {
                 presenceData = extract_streamed_data(dataStore.getPresenceData(), locationInfo.locationID);
                 // presenceData = NULL;
            }

            const UserLocationData* userLocationData = NULL;
            if (renderMode.userLocation) {
                userLocationData = extract_streamed_data(dataStore.getUserLocationData(), locationInfo.locationID);
                // userLocationData = NULL;
            }

            const LocationSkeletonData* locationSkeletons = NULL;
            if (renderMode.skeletons) {
                locationSkeletons = extract_streamed_data(dataStore.getUserJointData(), locationInfo.locationID);
            }

            POINT3D loc3dpos;
            loc3dpos.x = localLocation->position.x * projection.scale.x;
            loc3dpos.y = localLocation->position.y * projection.scale.y;
            POINT3D locWinPos = getWinCoords(loc3dpos);
            POINT3D mousePos = (POINT3D){ofGetMouseX(), ofGetMouseY(), 0};
            float mouseDist = sqrt(pow(mousePos.x - locWinPos.x, 2) + pow(mousePos.y - locWinPos.y, 2));

            // find hoverClosestLocationID
            // printf("current closest exists(%i) id(%i) dist(%f)\n", hoverClosestLocationExists, hoverClosestLocationID, hoverClosestLocationDistance);
            // printf("loc(%i) dist %f\n", locationInfo.locationID, mouseDist);
            if (!hoverClosestLocationExists || mouseDist < hoverClosestLocationDistance) {
                // printf("yes\n");
                hoverClosestLocationExists = true;
                hoverClosestLocationID = locationInfo.locationID;
                hoverClosestLocationDistance = mouseDist;
            }

            // draw presence indication
            if (presenceData) {
                ofSetHexColor(0xD83DFF); // purple
                if (presenceData->presenceEstimate > 0) {
                    ofFill();
                } else {
                    ofNoFill();
                    ofSetLineWidth(2);
                }
                // draw text in non-skew loop
            } else {
                ofSetHexColor(0xFF1B1B); // red
                ofFill();
            }

            // draw location circle
            ofCircle(loc3dpos.x, loc3dpos.y, 5);

            // hovering beam rings
            // ofNoFill();
            // ofSetHexColor(0x5DCAE6);
            // glEnable(GL_BLEND);
            // for (float z = 1; z < 100; z += 10) {
            //     glColor4f(0, 204/255., 255/255., (100 - z) / 200);
            //     ofCircle(loc3dpos.x, loc3dpos.y, z, 2);
            // }

            // draw location label
            if (mouseDist < 40) {
                pushUnprojectedMatrix(rootModelView, loc3dpos);
                    ofSetHexColor(0x000000);
                    ofFill();
                    fontMain->drawString("(" + lexical_cast<string>(locationInfo.locationID) + ") " + locationInfo.notes,
                        -20, -20 );
                    ofSetHexColor(0x202020);
                    // fontMain->drawString("theta: " + lexical_cast<string>(localLocation->rotation.theta / M_PI * 180.),
                    //     -20, -12 );
                ofPopMatrix();
            }


            // draw user location estimates
            // TODO: fix smoothing
            // TODO: make prettier
            if (userLocationData != NULL) {
                BOOST_FOREACH (const UserLocationEstimate& estimate, userLocationData->userLocationEstimates) {
                    ofFill();
                    ofSetHexColor(0xE78317);
                    float theta = localLocation->rotation.theta;
                    // note these are switched to match screen to world
                    float ex = estimate.y;
                    float ey = estimate.x;

                    // rotate & scale
                    float tex = ex * cos(theta) + ey * sin(theta);
                    float tey = -ex * sin(theta) + ey * cos(theta);
                    tex *= projection.scale.x / 1000.;
                    tey *= projection.scale.x / 1000.;

                    ofCircle(loc3dpos.x + tex, loc3dpos.y + tey, 4);

                    // render label
                    // if (mouseDist < 40) {
                    //     pushUnprojectedMatrix(rootModelView, loc3dpos);
                    //     ofSetHexColor(0x774510);
                    //     string x_str = lexical_cast<string>(ex);
                    //     string y_str = lexical_cast<string>(ey);
                    //     string uid_str = lexical_cast<string>(estimate.userID);

                    //     fontMain->drawString("(" + x_str + ", " + y_str + ")", tex, tey + 10 );
                    //     fontMain->drawString("uid: " + uid_str, tex, tey + 20 );
                    //     ofPopMatrix();
                    // }
                }
            }

            // draw skeletons
            if (locationSkeletons != NULL) {
                BOOST_FOREACH (const SkeletonData& skel, locationSkeletons->userJointData) {
                    SkeletonRenderMode3D rm;
                    SkeletonRenderer3D().render3D(rm, skel, loc3dpos, this);
                }
            }

            // // draw pdata location estimates
            // if (renderMode.userLocation) {
            //     BOOST_FOREACH (auto& pair, dataStore.getUserPData()) {
            //         const UserPData& pUser = pair.second;
            //         if (!pUser.hasLastSnapshot) continue;
            //         if (pUser.lastLocationID != locationInfo.locationID) continue;
            //         const UserLocationEstimate& estimate = pUser.lastEstimate;

            //         ofFill();
            //         ofColor userColor;
            //         userColor.setHsb(276 * 255 / 360, 255, 255);

            //         ofSetColor(userColor);
            //         float theta = localLocation->rotation.theta;
            //         // note these are switched to match screen to world
            //         float ex = estimate.y;
            //         float ey = estimate.x;

            //         // rotate & scale
            //         float tex = ex * cos(theta) + ey * sin(theta);
            //         float tey = -ex * sin(theta) + ey * cos(theta);
            //         tex *= projection.scale.x / 1000.;
            //         tey *= projection.scale.x / 1000.;

            //         unsigned int age = ofGetElapsedTimeMillis() - pUser.lastSnapshotTime;
            //         tex += pUser.vx * age / 500;
            //         tey += pUser.vy * age / 500;

            //         ofCircle(loc3dpos.x + tex, loc3dpos.y + tey, 2);

            //         // render label
            //         if (mouseDist < 40) {
            //             pushUnprojectedMatrix(rootModelView, loc3dpos);
            //             ofSetHexColor(0x774510);
            //             string x_str = lexical_cast<string>(ex);
            //             string y_str = lexical_cast<string>(ey);
            //             string uid_str = lexical_cast<string>(estimate.userID);
            //             string age_str = lexical_cast<string>(age);
            //             string vx_str = lexical_cast<string>(pUser.vx);
            //             string vy_str = lexical_cast<string>(pUser.vy);

            //             fontMain->drawString("(" + x_str + ", " + y_str + ")", tex, tey + 10 );
            //             fontMain->drawString("uid: " + uid_str, tex, tey + 24 );
            //             fontMain->drawString("age: " + age_str, tex, tey + 38 );
            //             fontMain->drawString("vel: " + vx_str + ", " + vy_str, tex, tey + 52 );
            //             ofPopMatrix();
            //         }
            //     }
            // }

            // draw name in non-skew loop
        }
    }

    ofPopMatrix(); // skew

    // map name
    // ofPushMatrix();
    //     ofTranslate(0, -50);
    //     // map name
    //     // ofDrawBitmapString(layout->layoutName, 5, 17 );
    //     ofSetHexColor(0x000000);
    //     fontMapNameLabel->drawString(layout->layoutName, 5, 17 );
    // ofPopMatrix();

    ofPopMatrix(); // translation
}
