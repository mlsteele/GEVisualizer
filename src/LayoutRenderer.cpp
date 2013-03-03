#include "LayoutRenderer.h"


string format_double_to_string(double n) {
    string s;
    std::ostringstream ss;
    ss << n;
    return ss.str();
}

// returns data or a NULL if none is present in list
template <typename T>
T* extract_streamed_data(vector<T>& list, unsigned int locationID) {
    for (T& thing : list) {
        if (thing.locationID == locationID) {
            return &thing;
        }
    }
    return NULL;
}


void LayoutRenderer::setupProjection(POINT2D screen_px_corner, POINT2D real_corner, double screenPixelsPerMeter) {
    Layout& l = *layout;

    projection.screen_px_corner     = screen_px_corner;
    projection.real_corner          = real_corner;
    projection.screenPixelsPerMeter = screenPixelsPerMeter;

    projection.offset.x = - real_corner.x + (screen_px_corner.x / screenPixelsPerMeter);
    projection.offset.y = - real_corner.y + (screen_px_corner.y / screenPixelsPerMeter);
    projection.scale.x = screenPixelsPerMeter;
    projection.scale.y = screenPixelsPerMeter;

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

// TODO: make this more efficient (less loops)
void LayoutRenderer::recalculateTexture(GEVisualizer& store) {
    const unsigned int w = textureSize[0];
    const unsigned int h = textureSize[1];

    // clear
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            textureData[(j * w + i) * 4 + 3] = 0; // alpha

    int weight_accumulator = 0;
    for (LocationInfo& locationInfo : store.getLocationInfo()) {
        Location* localLocation = extract_streamed_data(layout->locations, locationInfo.locationID);
        if (!localLocation) continue;
        PresenceData* presenceData = extract_streamed_data(store.getPresenceData(), locationInfo.locationID);
        if (!presenceData) continue;
        weight_accumulator += 1;
    }

    // loop and fill against locations
    for (LocationInfo& locationInfo : store.getLocationInfo()) {
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
    pWindow.y = winY;
    pWindow.z = winZ;
    return pWindow;
}

// store is the data store for ge information
// transition is 0 for normal view, positive for offset up, negative for offset down
// transition bounds of [-1, 1] are nominally the points at which a renderer should be disabled
void LayoutRenderer::render(LayoutRenderMode& renderMode, GEVisualizer& dataStore, float transition) {
    // const float rotateX = 20;
    const float rotateX = ofGetElapsedTimef()*10;
    float smoothed_transition = pow(transition, 3);

    ofPushMatrix(); // translation

    // transition
    ofTranslate(0, smoothed_transition * layout->svgBoundingRect.height / layout->pixelsPerMeter * projection.scale.y * 1.7);

    // x,y projection offset
    ofTranslate(
        projection.offset.x * projection.scale.x ,
        projection.offset.y * projection.scale.y );

    ofPushMatrix(); // skew
    // skew rotation
    ofRotateX(rotateX);

    // texture
    ofEnableAlphaBlending(); // required for wall transparency
    if (renderMode.texture) {
        recalculateTexture(dataStore);
        ofSetHexColor(0x3D9BFF);
        texture.loadData(textureData, textureSize[0], textureSize[1], GL_RGBA);
        texture.draw(0, 0, textureSize[0], textureSize[1]);
    }
    ofDisableAlphaBlending();

    ofEnableSmoothing();

    // map boundary
    if (renderMode.structure) {
        ofSetLineWidth(2);
        ofSetHexColor(0x0000FF);
        ofNoFill();
        ofRect(0, 0,
            layout->svgBoundingRect.width  / layout->pixelsPerMeter * projection.scale.x ,
            layout->svgBoundingRect.height / layout->pixelsPerMeter * projection.scale.y );

        // walls
        const float wallHeight = 2.5 * projection.screenPixelsPerMeter;
        for (LINE& wall : layout->wallLines) {
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
        for (LocationInfo& locationInfo : dataStore.getLocationInfo()) {
            Location* localLocation = extract_streamed_data(layout->locations, locationInfo.locationID);
            if (!localLocation) continue;

            PresenceData* presenceData = NULL;
            if (renderMode.presence) {
                 presenceData = extract_streamed_data(dataStore.getPresenceData(), locationInfo.locationID);
            }

            UserLocationData* userLocationData = NULL;
            if (renderMode.userLocation) {
                userLocationData = extract_streamed_data(dataStore.getUserLocationData(), locationInfo.locationID);
                if (userLocationData != NULL)
                    printf("userLocationData collected id %i\n", userLocationData->locationID);
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
            ofCircle(
                localLocation->position.x * projection.scale.x,
                localLocation->position.y * projection.scale.y,
                5 );


            // draw location label
            ofSetHexColor(0x0);
            ofFill();
            POINT3D loc3dpos;
            loc3dpos.x = localLocation->position.x * projection.scale.x;
            loc3dpos.y = localLocation->position.y * projection.scale.y;
            // offset from point
            loc3dpos.x -= 20;
            loc3dpos.y -= 20;
            // POINT3D winCoords = getWinCoords(loc3dpos);
            // printf("winCoords (%f, %f)\n", winCoords.x, winCoords.y);
            ofPushMatrix(); // drawing label
                glLoadIdentity();
                // fontMain->drawString("hello", winCoords.x, winCoords.y);
                fontMain->drawString("hello", 20, 20);
                // fontMain->drawString("(" + lexical_cast<string>(locationInfo.locationID) + ") " + locationInfo.notes, winCoords.x, winCoords.y);
            ofPopMatrix();


            // draw user location estimates
            // TODO: fix rotation
            // TODO: fix smoothing
            // TODO: make prettier
            if (userLocationData != NULL) { 
                // printf("userLocationData @0x%x\n", userLocationData);
                printf("userLocationData->locationID %i \n", userLocationData->locationID);
                printf("userLocationData->userLocationEstimates @0x%x \n", &(userLocationData->userLocationEstimates));
                printf("userlocsize %i\n", userLocationData->userLocationEstimates.size());
                for (UserLocationEstimate& estimate : userLocationData->userLocationEstimates) {
                // for (int i = 0; i < userLocationData->userLocationEstimates.size(); i++) {
                    // UserLocationEstimate& estimate = userLocationData->userLocationEstimates[i];
                    printf("estimate @0x%x\n", &estimate);
                    ofFill();
                    ofSetHexColor(0xE78317);
                    float ex = estimate.x;
                    float ey = estimate.y;
                    ofCircle(
                        (localLocation->position.x + ex / 1000.) * projection.scale.x ,
                        (localLocation->position.y + ey / 1000.) * projection.scale.y ,
                        3 );
                }
            }

            // draw name in non-skew loop
        }
    }

    ofPopMatrix(); // skew

    // map name
    ofPushMatrix();
        ofTranslate(0, -50);
        // map name
        ofSetHexColor(0xFFFFFF);
        // ofDrawBitmapString(layout->layoutName, 5, 17 );
        fontMapNameLabel->drawString(layout->layoutName, 5, 17 );
    ofPopMatrix();

    ofPopMatrix(); // translation
}
