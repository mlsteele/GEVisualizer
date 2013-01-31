#pragma once

#include "ofMain.h"
#include "geometry.h"
#include "LocationData.h"
#include "GEVisualizer.h"

// class LayoutInfo {
// public:
//     string svgFilename;
//     string keyElementID;
//     int floorLevel;
//     int numLocations;
//     RECT boundingRect;
//     POINT boundingRectRealWorldCoordinates;
//     POINT boundingRectScaleCoefficients;
// };

typedef struct {
    POINT3D offset = {0, 0, 0};
    POINT3D scale = {1, 1, 1};
} LayoutProjection;

class Layout {
public:
    friend class LayoutRenderer;

    bool loadLayoutFiles(string dataPath, string infoPath);
    void setupProjection();
    void render(GEVisualizer& store);

    string layoutName;
    vector< LINE > wallLines;

private:
    // temporary (for parsing)
    string dataPath;
    string infoPath;
    string svgPath;
    string keyElementID;
    POINT3D boundingRectRealWorldCoordinates;
    POINT3D boundingRectScaleCoefficients;
    int floorLevel;
    int numLocationNodes;
    RECT svgBoundingRect;
    // map id_map;

    // permanent
    LayoutProjection projection;
    vector< Location > locations;

    bool loadSVG(string svgPath);
    bool loadInfo(string infoPath);
};
