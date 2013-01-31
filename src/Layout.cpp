#include "Layout.h"

#include "XMLReader/tinyxml.h"

bool Layout::loadLayoutFiles(string dataPath, string infoPath) {
    printf("loadLayoutFiles\n");
    this->dataPath = dataPath;
    this->infoPath = infoPath;

    if (loadInfo(dataPath + infoPath)) {
        if (loadSVG(dataPath + svgPath)) {
            printf("loaded layout %s\n", layoutName.c_str());
            return true;
        } else {
            printf("ERR: failed to load svg from %s\n", svgPath.c_str());
        }
    } else {
        printf("ERR: failed to load info from %s\n", infoPath.c_str());
    }


    return false;
}

void Layout::setupProjection() {
    projection.offset.x = -boundingRectRealWorldCoordinates.x + 15;
    projection.offset.y = -boundingRectRealWorldCoordinates.y + 15;
    projection.offset.z = -boundingRectRealWorldCoordinates.z;
    projection.scale.x = 1. / boundingRectScaleCoefficients.x * 1.4;
    projection.scale.y = 1. / boundingRectScaleCoefficients.y * 1.4;
    projection.scale.z = 1. / boundingRectScaleCoefficients.z * 1.4;
}

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

void Layout::render(GEVisualizer& store) {
    ofPushMatrix();
    ofTranslate(projection.offset.x * projection.scale.x, projection.offset.y * projection.scale.y);

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
        svgBoundingRect.width * boundingRectScaleCoefficients.x * projection.scale.x ,
        svgBoundingRect.width * boundingRectScaleCoefficients.y * projection.scale.y );

    // walls
    ofDisableSmoothing();
    for (LINE& wall : wallLines) {
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
        Location* localLocation = extract_streamed_data(locations, locationInfo.locationID);
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

bool Layout::loadInfo(string infoPath){
    printf("loading info from %s\n", infoPath.c_str());

    fstream file;

    file.open( infoPath.c_str(), fstream::in );

    if( !file.is_open() ){
        printf("ERROR: Failed to open file: %s\n", infoPath.c_str());
        return false;
    }

    string word;

    //Read the file header
    file >> word;
    if( word != "LAYOUT_INFO_FILE_V1.2" ){
        file.close();
        printf("ERROR: Failed to read file header!\n");
        return false;
    }

    //Read the layout name
    file >> word;
    if( word != "LayoutName:" ){
        file.close();
        printf("ERROR: Failed to read SVGFilename header!\n");
        return false;
    }
    file >> layoutName;

    //Read the SVG filename
    file >> word;
    if( word != "SVGFilename:" ){
        file.close();
        printf("ERROR: Failed to read SVGFilename header!\n");
        return false;
    }
    file >> svgPath;

    //Read the KeyElementID
    file >> word;
    if( word != "KeyElementID:" ){
        file.close();
        printf("ERROR: Failed to read KeyElementID header!\n");
        return false;
    }
    file >> keyElementID;

    //Read the BoundingRectRealWorldCoordinates
    file >> word;
    if( word != "BoundingRectRealWorldCoordinates:" ){
        file.close();
        printf("ERROR: Failed to read BoundingRectRealWorldCoordinates header!\n");
        return false;
    }
    file >> boundingRectRealWorldCoordinates.x;
    file >> boundingRectRealWorldCoordinates.y;
    file >> boundingRectRealWorldCoordinates.z;

    printf("boundingRectRealWorldCoordinates ( %f , %f , %f )\n",
        boundingRectRealWorldCoordinates.x ,
        boundingRectRealWorldCoordinates.y ,
        boundingRectRealWorldCoordinates.z );

    //Read the KeyElementID
    file >> word;
    if( word != "BoundingRectScaleCoefficients:" ){
        file.close();
        printf("ERROR: Failed to read BoundingRectScale header!\n");
        return false;
    }

    file >> boundingRectScaleCoefficients.x;
    file >> boundingRectScaleCoefficients.y;
    file >> boundingRectScaleCoefficients.z;

    printf("boundingRectScaleCoefficients ( %f , %f , %f )\n",
        boundingRectScaleCoefficients.x ,
        boundingRectScaleCoefficients.y ,
        boundingRectScaleCoefficients.z );

    //Read the FloorLevel
    file >> word;
    if( word != "FloorLevel:" ){
        file.close();
        printf("ERROR: Failed to read FloorLevel header!\n");
        return false;
    }
    file >> floorLevel;

    //Read the number of location nodes
    file >> word;
    if( word != "NumLocationNodes:" ){
        file.close();
        printf("ERROR: Failed to read NumLocationNodes header!\n");
        return false;
    }
    file >> numLocationNodes;

    //Load location node data
    for(unsigned int i=0; i<numLocationNodes; i++){
        Location newLocation;

        //Read the location node header
        file >> word;
        if( word != "LocationNode:" ){
            file.close();
            printf("ERROR: Failed to read LocationNode header!\n");
            return false;
        }
        unsigned int locationIndex;
        file >> locationIndex;

        if( locationIndex != i+1 ){
            printf("Warning: Incorrect location node index at node %i. The index is %i!\n", i+1, locationIndex);
        }

        //Read the location ID header
        file >> word;
        if( word != "LocationID:" ){
            file.close();
            printf("ERROR: Failed to read LocationID header!\n");
            return false;
        }
        file >> newLocation.locationID;

        //Read the location position header
        file >> word;
        if( word != "LocationPosition:" ){
            file.close();
            printf("ERROR: Failed to read LocationPosition header!\n");
            return false;
        }
        file >> newLocation.position.x;
        file >> newLocation.position.y;
        file >> newLocation.position.z;

        // transform positions
        newLocation.position.x = newLocation.position.x * boundingRectScaleCoefficients.x + boundingRectRealWorldCoordinates.x;
        newLocation.position.y = newLocation.position.y * boundingRectScaleCoefficients.y + boundingRectRealWorldCoordinates.y;
        newLocation.position.z = newLocation.position.z * boundingRectScaleCoefficients.z + boundingRectRealWorldCoordinates.z;

        locations.push_back( newLocation );
    }

    file.close();
    return true;
}

bool Layout::loadSVG(string svgPath) {
    printf("loading svg from %s\n", svgPath.c_str());

    TiXmlDocument doc( svgPath.c_str() );
    if (!doc.LoadFile()){
        printf("WARNING: Failed to load file!\n");
        return false;
    }

    TiXmlHandle hDoc(&doc);
    TiXmlElement* pElem;
    TiXmlHandle hRoot(0);

    // block: SVG - check to make sure this is actually an SVG file
    {
        pElem=hDoc.FirstChildElement().Element();
        // should always have a valid root but handle gracefully if it does
        if (!pElem) return false;
        printf("ROOT: %s\n",pElem->Value());

        if( strcmp(pElem->Value(),"svg") != 0 ){
            printf("WARNING: Opened XML file bu this is not an SVG file!\n");
            return false;
        }

        // save this for later
        hRoot=TiXmlHandle(pElem);
    }

    //Search the SVG file for the block we want to parse, this should be in a group node

    // block: windows
    {
        TiXmlElement* pGroupElement = hRoot.FirstChild( "g" ).Element();

        if( pGroupElement == NULL ){
            printf("Failed to find child g\n");
            return false;
        }

        for( pGroupElement; pGroupElement; pGroupElement=pGroupElement->NextSiblingElement() )
        {
            const char *pName=pGroupElement->Attribute("id");
            if(pName) printf("ID: %s\n",pName);

            if( pName ){

                if( strcmp( pName, keyElementID.c_str() ) == 0 ){
                    printf("FOUND KEY Element\n");

                    //Load the main bounding rectanlge
                    TiXmlElement* pElement = pGroupElement->FirstChildElement( "rect" );
                    if( pElement == NULL ){
                        printf("Failed to parse rect from file!\n");
                        return false;
                    }
                    pElement->QueryDoubleAttribute("x", &svgBoundingRect.x);
                    pElement->QueryDoubleAttribute("y", &svgBoundingRect.y);
                    pElement->QueryDoubleAttribute("width", &svgBoundingRect.width);
                    pElement->QueryDoubleAttribute("height", &svgBoundingRect.height);

                    //Search for the wall lines
                    for( pElement; pElement; pElement=pElement->NextSiblingElement() )
                    {
                        if( strcmp(pElement->Value(),"line") == 0 ){
                            LINE newLine;

                            pElement->QueryDoubleAttribute("x1", &newLine.x1);
                            pElement->QueryDoubleAttribute("y1", &newLine.y1);
                            pElement->QueryDoubleAttribute("x2", &newLine.x2);
                            pElement->QueryDoubleAttribute("y2", &newLine.y2);

                            // transform line
                            newLine.x1 = newLine.x1 * boundingRectScaleCoefficients.x + boundingRectRealWorldCoordinates.x;
                            newLine.y1 = newLine.y1 * boundingRectScaleCoefficients.y + boundingRectRealWorldCoordinates.y;
                            newLine.x2 = newLine.x2 * boundingRectScaleCoefficients.x + boundingRectRealWorldCoordinates.x;
                            newLine.y2 = newLine.y2 * boundingRectScaleCoefficients.y + boundingRectRealWorldCoordinates.y;

                            wallLines.push_back( newLine );
                        }

                    }

                }
            }

        }
    }

    return true;
}
