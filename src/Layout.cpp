#include "Layout.h"

#include "XMLReader/tinyxml.h"

bool Layout::loadLayoutFiles(string dataPath, string infoPath) {
    this->dataPath = dataPath;
    this->infoPath = infoPath;

    if (loadInfo(infoPath))
        if (loadSVG(svgPath))
            return true;
        else
            printf("ERR: failed to load svg from %s\n", svgPath.c_str());
    else
        printf("ERR: failed to load info from %s\n", infoPath.c_str());

    return false;
}

// void Layout::render() {
//     printf("ERR: not implemented\n");
// }

bool Layout::loadInfo(string infoPath){

    fstream file;

    file.open( infoPath.c_str(), fstream::in );

    if( !file.is_open() ){
        printf("ERROR: Failed to open file: %s\n", infoPath.c_str());
        return false;
    }

    string word;

    //Read the file header
    file >> word;
    if( word != "LAYOUT_INFO_FILE_V1.1" ){
        file.close();
        printf("ERROR: Failed to read file header!\n");
        return false;
    }

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
        file >> newLocation.ge_id;

        //Read the location position header
        file >> word;
        if( word != "LocationPosition:" ){
            file.close();
            printf("ERROR: Failed to read LocationPosition header!\n");
            return false;
        }
        file >> newLocation.positionX;
        file >> newLocation.positionY;
        file >> newLocation.positionZ;

        // transform positions
        newLocation.positionX = newLocation.positionX * boundingRectScaleCoefficients.x + boundingRectRealWorldCoordinates.x;
        newLocation.positionY = newLocation.positionY * boundingRectScaleCoefficients.y + boundingRectRealWorldCoordinates.y;
        newLocation.positionZ = newLocation.positionZ * boundingRectScaleCoefficients.z + boundingRectRealWorldCoordinates.z;

        locations.push_back( newLocation );
    }

    file.close();
    return true;
}

bool Layout::loadSVG(string svgPath) {
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
