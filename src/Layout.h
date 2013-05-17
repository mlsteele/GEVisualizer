/*
    Layout

    Stores wall positions and location positions.
    Loads data from svg and info txt files.
*/

#pragma once

#include "ofMain.h"
#include "geometry.h"
#include "LocationData.h"
#include "GEVisualizer.h"

class Layout {
public:
    friend class LayoutRenderer;

    bool loadLayoutFiles(string dataPath, string infoPath);

    string layoutName;
    vector< LINE > wallLines;

private:
    // temporary (for parsing)
    string dataPath;
    string infoPath;
    string svgPath;
    string keyElementID;
    POINT3D boundingRectRealWorldCoordinates;
    double pixelsPerMeter;
    int floorLevel;
    int numLocationNodes;
    RECT svgBoundingRect;
    // map id_map;

    // permanent
    vector< Location > locations;

    bool loadSVG(string svgPath);
    bool loadInfo(string infoPath);
};
