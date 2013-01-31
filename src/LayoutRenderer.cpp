#include "LayoutRenderer.h"


// returns data or a NULL if none is present in list
template <typename T>
T* extract_streamed_data(vector<T> list, int locationID) {
    for (T& thing : list) {
        if (thing.locationID == locationID) {
            return &thing;
        }
    }
    return NULL;
}

void LayoutRenderer::setupProjection(POINT2D screen_px_corner, POINT2D real_corner, double screenPixelsPerMeter) {
    Layout& l = *layout;

    projection.offset.x = - real_corner.x + (screen_px_corner.x / screenPixelsPerMeter);
    projection.offset.y = - real_corner.y + (screen_px_corner.y / screenPixelsPerMeter);
    projection.scale.x = screenPixelsPerMeter;
    projection.scale.y = screenPixelsPerMeter;
}

void LayoutRenderer::render(GEVisualizer& store) {
    ofPushMatrix();
    ofTranslate(
        projection.offset.x * projection.scale.x ,
        projection.offset.y * projection.scale.y );

    ofSetLineWidth(1);

    // // test circles (corners)
    // ofSetHexColor(0xFFAA16);
    // ofFill();
    // ofCircle(
    //     boundingRectRealWorldCoordinates.x * projection.scale.x,
    //     boundingRectRealWorldCoordinates.y * projection.scale.y,
    //     10 );

    // boundary
    ofSetHexColor(0x0000FF);
    ofNoFill();
    ofRect(0, 0,
        layout->svgBoundingRect.width  / layout->pixelsPerMeter * projection.scale.x ,
        layout->svgBoundingRect.height / layout->pixelsPerMeter * projection.scale.y );

    // walls
    ofDisableSmoothing();
    for (LINE& wall : layout->wallLines) {
        ofSetHexColor(0x000000);
        ofFill();
        ofLine(
            wall.x1 * projection.scale.x ,
            wall.y1 * projection.scale.y ,
            wall.x2 * projection.scale.x ,
            wall.y2 * projection.scale.y );
    }
    ofEnableSmoothing();

    // locations
    for (LocationInfo& locationInfo : store.getLocationInfo()) {
        Location* localLocation = extract_streamed_data(layout->locations, locationInfo.locationID);
        if (!localLocation) continue;
        PresenceData* presenceData = extract_streamed_data(store.getPresenceData(), locationInfo.locationID);

        switch(presenceData ? presenceData->presenceEstimate : -1) {
            case 0:
                ofSetHexColor(0xD83DFF);
                ofNoFill();
                ofSetLineWidth(2);
                break;
            case 1:
                ofSetHexColor(0xD83DFF);
                ofFill();
                break;
            default:
                printf("presenceEstimate %i\n", presenceData->presenceEstimate);
                ofSetHexColor(0xFF1B1B);
                ofFill();
        }

        ofCircle(
            localLocation->position.x * projection.scale.x,
            localLocation->position.y * projection.scale.y,
            5 );

        ofSetHexColor(0x0);
        ofFill();
        ofDrawBitmapString((string)locationInfo.notes,
            localLocation->position.x * projection.scale.x - 20,
            localLocation->position.y * projection.scale.y - 20);
    }

    ofPopMatrix();
}
