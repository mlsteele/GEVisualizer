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

    ofEnableSmoothing();

    // map name
    ofSetHexColor(0x0);
    // ofDrawBitmapString(layout->layoutName, 5, 17 );
    mainFont->drawString(layout->layoutName, 5, 17 );

    // boundary
    ofSetLineWidth(2);
    ofSetHexColor(0x0000FF);
    ofNoFill();
    ofRect(0, 0,
        layout->svgBoundingRect.width  / layout->pixelsPerMeter * projection.scale.x ,
        layout->svgBoundingRect.height / layout->pixelsPerMeter * projection.scale.y );

    // walls
    ofSetLineWidth(3);
    for (LINE& wall : layout->wallLines) {
        ofSetHexColor(0xB3B3B3);
        ofFill();
        ofLine(
            wall.x1 * projection.scale.x ,
            wall.y1 * projection.scale.y ,
            wall.x2 * projection.scale.x ,
            wall.y2 * projection.scale.y );
    }

    // locations
    // TODO: if MODE
    for (LocationInfo& locationInfo : store.getLocationInfo()) {
        Location* localLocation = extract_streamed_data(layout->locations, locationInfo.locationID);
        if (!localLocation) continue;
        PresenceData* presenceData = extract_streamed_data(store.getPresenceData(), locationInfo.locationID);

        if (presenceData) {
            ofSetHexColor(0xD83DFF); // purple
            if (presenceData->presenceEstimate > 0) {
                ofFill();
            } else {
                ofNoFill();
                ofSetLineWidth(2);
            }
        } else {
            ofSetHexColor(0xFF1B1B); // red
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
