#pragma once

#include "ofMain.h"
#include "LocationData.h"

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
    double x = 0;
    double y = 0;
    double z = 0;
} POINT3D;

typedef struct {
    double x = 0;
    double y = 0;
    double width = 0;
    double height = 0;
} RECT;

typedef struct {
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
} LINE;

class Layout {
public:
    bool loadLayoutFiles(string dataPath, string infoPath);
    void render();

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
    vector< LINE > wallLines;
    vector< Location > locations;
    vector< LocationStream > locationStreams;

    bool loadSVG(string svgPath);
    bool loadInfo(string infoPath);
};
