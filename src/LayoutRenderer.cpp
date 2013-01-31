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

void LayoutRenderer::render(GEVisualizer& store) {
    ofPushMatrix();
    ofTranslate(
        layout->projection.offset.x * layout->projection.scale.x ,
        layout->projection.offset.y * layout->projection.scale.y );

    ofSetLineWidth(1);

    // // test circles (corners)
    // ofSetHexColor(0xFFAA16);
    // ofFill();
    // ofCircle(
    //     boundingRectRealWorldCoordinates.x * projection.scale.x,
    //     boundingRectRealWorldCoordinates.y * projection.scale.y,
    //     10 );

    // boundary
    ofSetHexColor(0x000000);
    ofNoFill();
    ofRect(0, 0,
        layout->svgBoundingRect.width * layout->boundingRectScaleCoefficients.x * layout->projection.scale.x ,
        layout->svgBoundingRect.width * layout->boundingRectScaleCoefficients.y * layout->projection.scale.y );

    // walls
    ofDisableSmoothing();
    for (LINE& wall : layout->wallLines) {
        ofSetHexColor(0x000000);
        ofFill();
        ofLine(
            wall.x1 * layout->projection.scale.x ,
            wall.y1 * layout->projection.scale.y ,
            wall.x2 * layout->projection.scale.x ,
            wall.y2 * layout->projection.scale.y );
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
            localLocation->position.x * layout->projection.scale.x,
            localLocation->position.y * layout->projection.scale.y,
            5 );

        ofSetHexColor(0x0);
        ofFill();
        ofDrawBitmapString((string)locationInfo.notes,
            localLocation->position.x * layout->projection.scale.x - 20,
            localLocation->position.y * layout->projection.scale.y - 20);
    }

    ofPopMatrix();
}
